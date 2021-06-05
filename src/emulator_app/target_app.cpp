#if defined(__linux__)
#include <sys/mman.h>        // mmap
#else
//#include "windows/syscall.h" // SYS_ numbers
#include "windows/posix.h"
#endif // __linux__
#include <boost/align/aligned_alloc.hpp>

#include "target_app.hpp"
#include "emulator/dump/formatter_txt.hpp"

namespace postrisc {

TargetSingleApp::TargetSingleApp(Decoder& decoder, IDoomInterface& doom_)
    : IManagedAddressSpace<TargetSingleApp>(12, decoder)
    , doom_interface(doom_)
{
    start_time = std::chrono::steady_clock::now();
}

TargetSingleApp::TargetSingleApp(size_t pagesize, Decoder& decoder, IDoomInterface& doom_)
    : IManagedAddressSpace<TargetSingleApp>(pagesize, decoder)
    , doom_interface(doom_)
{
}

TargetSingleApp::~TargetSingleApp(void)
{
}

uintptr_t
TargetSingleApp::PageSizeRoundDown(uintptr_t addr) const
{
    return addr & ~(get_page_size() - 1);
}

uintptr_t
TargetSingleApp::PageSizeRoundUp(uintptr_t addr) const
{
    return PageSizeRoundDown(addr + get_page_size() - 1);
}

const std::span<std::byte>
TargetSingleApp::virtual_alloc(size_t size)
{
    size = PageSizeRoundUp(size);
    return { reinterpret_cast<std::byte*>(boost::alignment::aligned_alloc(get_page_size(), size)), size };
}

void TargetSingleApp::virtual_free(const std::span<std::byte> data)
{
    boost::alignment::aligned_free(data.data());
}

/*==================================================================================*/
// ELF start stack
/*==================================================================================*/

static char const *push_string(u8 *&top, size_t &size, char const *string)
{
    size_t len = strlen(string) + 1;
    if (len > size) {
        LOG_ERROR(LOG_LOADER) << "can't push string to stack: " << string;
        return nullptr;
    }
    top -= len;
    size -= len;
    while (size % sizeof(uintptr_t)) { --top; --size; }
    memcpy(top, string, len);
    return reinterpret_cast<char const*>(top);
}

static void push_ptr(u8 *&top, size_t &size, char const *ptr)
{
    top -= sizeof(ptr);
    size -= sizeof(ptr);
    *reinterpret_cast<uintptr_t*>(top) = reinterpret_cast<uintptr_t>(ptr);
}


template<typename ELF>
u8*
TargetSingleApp::prepare_stack(const std::span<std::byte> stack, int argc, char **argv, char **env, ElfInfo &elf_info)
{
    LOG_DEBUG(LOG_DUMP) << "prepare initial stack: size=" << stack.size();

    elf_info.stack_bottom = std::bit_cast<uintptr_t>(stack.data());
    elf_info.stack_top    = std::bit_cast<uintptr_t>(stack.data()) + stack.size();

    AddHostMemory(stack.data(), stack.size(), VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_FINAL);

    u8 *top = reinterpret_cast<u8*>(stack.data()) + stack.size();
    LOG_DEBUG(LOG_DUMP)
        << "stack: " << fmt::hex(std::bit_cast<uintptr_t>(stack.data()))
        << ".." << fmt::hex(std::bit_cast<uintptr_t>(top));

    size_t len = stack.size();

    dbgs() << "guest env:\n";
    size_t stack_envc = 0;
    char const *stack_env[256];
    while (env[stack_envc]) {
        dbgs() << "env[" << stack_envc << "]=" << env[stack_envc] << fmt::endl;
        if (stack_envc >= sizearray(stack_env)) {
            LOG_ERROR(LOG_LOADER) << "stack_env too few";
            return nullptr;
        }
        stack_env[stack_envc] = push_string(top, len, env[stack_envc]);
        stack_envc++;
    }

    dbgs() << "guest args: argc=" << argc << fmt::endl;
    size_t stack_argc = 0;
    char const *stack_argv[256];
    while (argv[stack_argc]) {
        dbgs() << "argv[" << stack_argc << "]=" << argv[stack_argc] << fmt::endl;
        if (stack_argc >= sizearray(stack_argv)) {
            LOG_ERROR(LOG_LOADER) << "stack_argv too few";
            return nullptr;
        }
        stack_argv[stack_argc] = push_string(top, len, argv[stack_argc]);
        stack_argc++;
    }

    // 16-byte random (16-byte aligned, might be less)
    top -= 16;
    len -= 16;
    while (len % 16) { --top; --len; }
    u8 *random = top;
    memset(random, 0x7f, 16);

    size_t const rest = sizeof(uintptr_t) * ((stack_argc + 1) + (stack_envc + 1) + 1);

    char const *platform = push_string(top, len, "postrisc");

    // align auxv 16-byte
    while (std::bit_cast<uintptr_t>(top - rest) % 16) { --top; --len; }

#define PUSH_AUXV(TYPE, VALUE)                                            \
    do {                                                                  \
        top -= sizeof(typename ELF::ELF_AUXV_T);                          \
        len -= sizeof(typename ELF::ELF_AUXV_T);                          \
        typename ELF::ELF_AUXV_T *tmp =                                   \
            reinterpret_cast<typename ELF::ELF_AUXV_T*>(top);             \
        tmp->a_type = TYPE;                                               \
        tmp->a_un.a_val = (uintptr_t)(VALUE);                             \
    } while(0)


    PUSH_AUXV(AT_NULL, 0);                  // end of auxv
    PUSH_AUXV(AT_SYSINFO_EHDR, 0);          // address of VDSO - we have no such in emulator
    PUSH_AUXV(AT_BASE, 0);                  // The base address of the program interpreter (dynamic linker),
                                            // we have no VDSO such in emulator
    PUSH_AUXV(AT_PHDR,  elf_info.ph_base);  // The address of the program headers of the executable
    PUSH_AUXV(AT_PHENT, elf_info.ph_size);  // The size of program header entry.
    PUSH_AUXV(AT_PHNUM, elf_info.ph_num);   // The number of program headers.
    PUSH_AUXV(AT_MINSIGSTKSZ, 2048);
    PUSH_AUXV(AT_PAGESZ, get_page_size());
    PUSH_AUXV(AT_CLKTCK, 100);
    PUSH_AUXV(AT_UID, getuid());
    PUSH_AUXV(AT_EUID, geteuid());
    PUSH_AUXV(AT_GID, getgid());
    PUSH_AUXV(AT_EGID, getegid());
    PUSH_AUXV(AT_SECURE, 0);
    PUSH_AUXV(AT_FLAGS, 0);
    PUSH_AUXV(AT_HWCAP, ~0ULL);
    PUSH_AUXV(AT_HWCAP2, 0x2);
    PUSH_AUXV(AT_EXECFN, stack_argv[0]);
    PUSH_AUXV(AT_ENTRY, elf_info.entry);
    PUSH_AUXV(AT_RANDOM, random);
    PUSH_AUXV(AT_PLATFORM, platform);

    push_ptr(top, len, NULL);
    for (size_t i = 0; i < stack_envc; i++) {
        push_ptr(top, len, stack_env[i]);
    }
    push_ptr(top, len, NULL);
    for (size_t i = 0; i < stack_argc; i++) {
        push_ptr(top, len, stack_argv[i]);
    }
    push_ptr(top, len, reinterpret_cast<char const*>((intptr_t)argc));

    size_t const dumped_offset = PageSizeRoundDown(len);
    LOG_DEBUG(LOG_DUMP)
        << "dumped_offset=" << dumped_offset
        << " size=" << stack.size()
        << " len=" << len;

    std::byte *dump_base = stack.data() + dumped_offset;
    LOG_DEBUG(LOG_DUMP) << "\n" << fmt::hex_data(dump_base, stack.size() - dumped_offset, std::bit_cast<uintptr_t>(dump_base));

    get_decoder().add_symbol(std::bit_cast<uintptr_t>(stack.data()), stack.size(), "!STACK");

    return top;
}

void
TargetSingleApp::prepare_backstore(const std::span<std::byte> backstore)
{
    LOG_DEBUG(LOG_DUMP) << "prepare backstore: size=" << backstore.size();
    AddHostMemory(backstore.data(), backstore.size(), VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_BACKING_STORE | VM_PAGE_RIGHT_FINAL);

    get_decoder().add_symbol(std::bit_cast<uintptr_t>(backstore.data()), backstore.size(), "!BACKSTORE");
}

static void dump_protection(u64 protection, std::ostream& out)
{
    out << ((protection & VM_PAGE_RIGHT_READ          ) ? 'R' : 'r')
        << ((protection & VM_PAGE_RIGHT_WRITE         ) ? 'W' : 'w')
        << ((protection & VM_PAGE_RIGHT_EXECUTE       ) ? 'X' : 'x')
        << ((protection & VM_PAGE_RIGHT_BACKING_STORE ) ? 'B' : 'b')
        << ((protection & VM_PAGE_RIGHT_FINAL         ) ? 'F' : 'f')
        << ((protection & VM_PAGE_RIGHT_PROMOTE       ) ? 'P' : 'p');
}

void
TargetSingleApp::dump_memory(std::ostream& out, unsigned flags) const
{
    out << "\nmemory dump\n";
    for (auto const & x : m_HostAccessMap) {
        u64 base = x.first * get_page_size() * pages_per_block;
        out << "page block: " << fmt::hex(base) << std::endl;
        for (auto const & y : x.second.data) {
            if (y) {
                out << fmt::hex(base) << " " << fmt::hex(y) << " ";
                dump_protection(y, out);
                out << std::endl;
                if (flags)
                    out << fmt::hex_data(reinterpret_cast<u8 const*>(base), get_page_size(), base);
            }
            base += get_page_size();
        }
    }
}

int
TargetSingleApp::AddHostMemory(void const *p, size_t size, u64 protection)
{
    uintptr_t base = reinterpret_cast<uintptr_t>(p);
    if (base & (get_page_size()-1)) {
        LOG_ERROR(LOG_PLATFORM) << "base=" << base;
        return -1;
    }
    if (size & (get_page_size()-1)) {
        LOG_ERROR(LOG_PLATFORM) << "size=" << size;
        return -1;
    }
    for (uintptr_t x = base; x < base + size; x += get_page_size()) {
        LOG_DEBUG(LOG_PLATFORM)
            << "emulator vmap: " << fmt::hex(x)
            << " protection=" << fmt::hex(protection);
        u64 page = x / get_page_size();
        m_HostAccessMap[page / pages_per_block].data[page % pages_per_block] = protection;
    }
    return 0;
}

int
TargetSingleApp::RemoveHostMemory(void *p, size_t size)
{
    uintptr_t base = reinterpret_cast<uintptr_t>(p);
    if (base & (get_page_size()-1)) {
        LOG_ERROR(LOG_PLATFORM) << "base=0x" << fmt::hex(base);
        return -1;
    }
    if (size & (get_page_size()-1)) {
        LOG_ERROR(LOG_PLATFORM) << "base=0x" << fmt::hex(base) << " size=" << size;
        //return -1;
    }
    for (uintptr_t x = base; x < base + size; x += get_page_size()) {
        LOG_DEBUG(LOG_PLATFORM) << "emulator vmap: " << fmt::hex(x);
        u64 page = x / get_page_size();
        m_HostAccessMap[page / pages_per_block].data[page % pages_per_block] = 0;
    }
    return 0;
}

void TargetSingleApp::report_page_fault(u64 address) const
{
    LOG_ERROR(LOG_PAGETABLE) << "can't find map entry: " << fmt::hex(address) << std::endl;
    dump_memory(dbgs(), 0);
}

void TargetSingleApp::report_access_fault(u64 address) const
{
    LOG_ERROR(LOG_PAGETABLE) << "invalid access rights:"
        << " address=" << fmt::hex(address);
    //    << " protection=" << protection;
}

int
TargetSingleApp::StartEmulation(EmulatorConfiguration const & configuration,
    int argc, char**argv, char**env)
{
    LOG_DEBUG(LOG_DUMP) << "single app emulation";

    if (argc < 1) {
        LOG_ERROR(LOG_LOADER) << "use with filename arg";
        return -1;
    }

    size_t const pagesize = get_page_size();
    ElfInfo elf_info = {};
    std::span<std::byte> image;
    if (load_elf_module(argv[0], elf_info, image) < 0) {
        LOG_ERROR(LOG_LOADER) << "load_elf_module failed";
        return -1;
    }
    dbgs() << "pid=" << getpid() << " pagesize=" << pagesize << fmt::endl;

    const std::span<std::byte> stack = virtual_alloc(4 * get_page_size());
    if (!stack.data()) {
        LOG_ERROR(LOG_LOADER) << "stack allocation failed";
        return -1;
    }
    auto const stack_top = prepare_stack<ELF64>(stack, argc, argv, env, elf_info);
    if (!stack_top) {
        LOG_ERROR(LOG_LOADER) << "prepare_stack failed";
        return -1;
    }

    const std::span<std::byte> backstore = virtual_alloc(4 * get_page_size());
    if (!backstore.data()) {
        LOG_ERROR(LOG_LOADER) << "backstore allocation failed";
        return -1;
    }
    prepare_backstore(backstore);

    LOG_DEBUG(LOG_DUMP) << "loading completed";

    u64 const sp = reinterpret_cast<uintptr_t>(stack_top);
    u64 const bsp = reinterpret_cast<uintptr_t>(stack.data());
    u64 const bp = reinterpret_cast<uintptr_t>(backstore.data());
    u64 const tp = 0;

    u32 const core_id = 0;
    Core core("core0", core_id, configuration.get_config_flags());
    core.reset(core_id);
    core.FlushTranslationBuffer();

    u64 const ip = elf_info.entry;
    // FIXME: starting eip should be valid
    u64 const eip = elf_info.text_base + elf_info.text_size;
    CHECK_STATUS(core.PrepareThread(bsp, sp, bp, tp, eip));
    core.SetIp(ip);

    dbgs() << "core init, start...\n"
              "===============================================\n";

    CStatus status = core.execute_until_interruption(*this);

    dbgs() << "===============================================\n"
              "core stop\n";

    if (status != CStatus::halt) {
        LOG_OUTPUT(LOG_LOADER) << "emulator exception " << status << fmt::endl;
    }
    dbgs() << "emulator stopped\n";

    dbgs() << "frames: " << m_frames_num;
    if (m_frames_num > 0) {
        dbgs() << " insn/frame: " << (core.GetPerfInsn() / m_frames_num);
    }
    dbgs() << fmt::endl;

    DumpFormatterTxt formatter(get_decoder(), dbgs());
    core.dump_statistic(formatter);
    WITH_DEBUG(LOG_EXCEPTION | LOG_DUMP) {
        dump_memory(dbgs(), 1);
    }

    virtual_free(stack);
    virtual_free(backstore);
    virtual_free(image);
    return 0;
}

void
TargetSingleApp::dump(std::ostream& UNUSED(out))
{
}

} // namespace postrisc
