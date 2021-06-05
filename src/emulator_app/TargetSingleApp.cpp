#if defined(__linux__)
#include <sys/mman.h>        // mmap
#else
//#include "windows/syscall.h" // SYS_ numbers
#include "windows/posix.h"
#endif // __linux__
#include <boost/align/aligned_alloc.hpp>
#include <boost/range/adaptor/reversed.hpp>

#include "TargetSingleApp.hpp"

namespace postrisc {

TargetSingleApp::TargetSingleApp(Decoder& decoder, DoomInterface& doom_)
    : ManagedAddressSpace<TargetSingleApp>(12, decoder)
    , doom_interface(doom_)
{
    start_time = std::chrono::steady_clock::now();
}

TargetSingleApp::TargetSingleApp(size_t pagesize, Decoder& decoder, DoomInterface& doom_)
    : ManagedAddressSpace<TargetSingleApp>(pagesize, decoder)
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
TargetSingleApp::prepare_stack(const std::span<std::byte> stack, int argc, char **argv, const std::map<std::string, std::string>& env, ElfInfo& elf_info)
{
    dbgs() << "<h2>prepare usual stack: " << stack.size() << " bytes</h2>\n";

    elf_info.stack_bottom = std::bit_cast<uintptr_t>(stack.data());
    elf_info.stack_top    = std::bit_cast<uintptr_t>(stack.data()) + stack.size();

    AddHostMemory(stack.data(), stack.size(), VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_FINAL);

    u8 *top = reinterpret_cast<u8*>(stack.data()) + stack.size();
    LOG_DEBUG(LOG_DUMP)
        << "stack: " << fmt::hex(std::bit_cast<uintptr_t>(stack.data()))
        << ".." << fmt::hex(std::bit_cast<uintptr_t>(top));

    size_t len = stack.size();

    dbgs() << "<h2>guest environment</h2><ul>\n";
    std::vector<char const *> stack_env;
    stack_env.reserve(env.size());

    for (const auto& e : env) {
        std::string s = e.first + "=" + e.second;
        dbgs() << "<li>" << s << "</li>\n";
        stack_env.push_back(push_string(top, len, s.c_str()));
    }
    dbgs() << "</ul>\n";

    dbgs() << "<h2>guest args</h2>"
              "<pre>argc=" << argc << fmt::endl;
    std::vector<char const *> stack_argv;
    for (size_t stack_argc = 0; argv[stack_argc]; stack_argc++) {
        dbgs() << "argv[" << stack_argc << "]=" << argv[stack_argc] << fmt::endl;
        stack_argv.push_back(push_string(top, len, argv[stack_argc]));
    }
    dbgs() << "</pre>\n";

    // 16-byte random (16-byte aligned, might be less)
    top -= 16;
    len -= 16;
    while (len % 16) { --top; --len; }
    u8 *random = top;
    memset(random, 0x7f, 16);

    // env pointers + NULL, argv pointers + NULL, argc
    size_t const rest = sizeof(uintptr_t) * ((stack_env.size() + 1) + (stack_argv.size() + 1) + 1);

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
    PUSH_AUXV(AT_PHDR,  elf_info.ph_base);  // The address of the program headers of the executable. It is not ph-base if SO is loaded as exe
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
    for (auto const x : boost::adaptors::reverse(stack_env)) {
        push_ptr(top, len, x);
    }
    push_ptr(top, len, NULL);
    for (auto const x : boost::adaptors::reverse(stack_argv)) {
        push_ptr(top, len, x);
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
    dbgs() << "<h2>prepare backstore: size=" << backstore.size() << " bytes</h2>\n";
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
    for (auto const& x : m_HostAccessMap) {
        u64 base = x.first * get_page_size() * pages_per_block;
        out << "page block: " << fmt::hex(base) << std::endl;
        for (auto const& y : x.second.data) {
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
    uintptr_t const base = reinterpret_cast<uintptr_t>(p);
    auto const page_size = get_page_size();
    if (!util::is_aligned(base, page_size)) {
        LOG_ERROR(LOG_PLATFORM) << "base=" << base;
        return -1;
    }
    if (!util::is_aligned(size, page_size)) {
        LOG_ERROR(LOG_PLATFORM) << "size=" << size;
        return -1;
    }
    for (uintptr_t x = base; x < base + size; x += page_size) {
        LOG_DEBUG(LOG_PLATFORM)
            << "emulator vmap: " << fmt::hex(x)
            << " protection=" << fmt::hex(protection);
        u64 const page = x / page_size;
        m_HostAccessMap[page / pages_per_block].data[page % pages_per_block] = protection;
    }
    return 0;
}

int
TargetSingleApp::RemoveHostMemory(void *p, size_t size)
{
    uintptr_t const base = reinterpret_cast<uintptr_t>(p);
    auto const page_size = get_page_size();
    if (!util::is_aligned(base, page_size)) {
        LOG_ERROR(LOG_PLATFORM) << "base=0x" << fmt::hex(base);
        return -1;
    }
    if (!util::is_aligned(size, page_size)) {
        LOG_ERROR(LOG_PLATFORM) << "base=0x" << fmt::hex(base) << " size=" << size;
        //return -1;
    }
    for (uintptr_t x = base; x < base + size; x += page_size) {
        LOG_DEBUG(LOG_PLATFORM) << "emulator vmap: " << fmt::hex(x);
        u64 const page = x / page_size;
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
TargetSingleApp::StartEmulation(const Configuration& configuration, int argc, char**argv, const std::map<std::string, std::string>& env)
{
    LOG_DEBUG(LOG_DUMP) << "single app emulation";
    size_t const pagesize = get_page_size();

    if (argc < 1) {
        LOG_ERROR(LOG_LOADER) << "use with filename arg";
        return -1;
    }
    dbgs() << "<h1>Execute: " << argv[0] << "</h1>\n"
          << "<p>pid=" << getpid() << " pagesize=" << pagesize << "</p>\n";

    ElfInfo elf_info = {};
    std::span<std::byte> image;
    if (load_elf_module(argv[0], elf_info, image) < 0) {
        LOG_ERROR(LOG_LOADER) << "load_elf_module failed";
        return -1;
    }

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
    core.set_itc(configuration.get_instruction_limit());

    u64 const ip = elf_info.entry;
    // FIXME: starting eip should be valid
    u64 const eip = elf_info.text_base + elf_info.text_size;
    CHECK_RESULT(core.PrepareThread(bsp, sp, bp, tp, eip));
    core.SetIp(ip);

    dbgs() << "<h2>Start...</h2><pre>\n";

    Result status = core.execute_until_interruption(*this);

    dbgs() << "</pre><h2>Stop</h2>\n";

    if (status != Result::halt) {
        LOG_OUTPUT(LOG_LOADER) << "emulator exception " << status << std::endl;
    }

    auto const frames_num = doom_interface.getFramesNum();
    dbgs() << "<pre>frames: " << frames_num;
    if (frames_num > 0) {
        dbgs() << "\ninstructions per frame: " << ((double)core.GetPerfInsn() / frames_num)
               << "\ninstructions per pixel: " << ((double)core.GetPerfInsn() / frames_num / (DoomInterface::width * DoomInterface::height));
        if (core.GetPerfTime() > 0) {
            double const runtime = 1.0e-9 * core.GetPerfTime(); // from nanoseconds to seconds
            dbgs() << "\nframes per second: " << (frames_num / runtime);
        }
    }
    dbgs() << "</pre>\n";

    DumpFormatter formatter(get_decoder(), dbgs(), true);
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
