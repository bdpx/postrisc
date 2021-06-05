#pragma once

#include <map>

#include <boost/align/aligned_allocator.hpp>
#include "config/emulator_configuration.hpp"
#include "doom/doom_interface.hpp"

#include "emulator/IManagedAddressSpace.hpp"
#include "elf_loader.hpp"
#include "emulator/core.hpp"

#define SYS_dump_value           1973
#define SYS_dump_memory          1974
#define SYS_doom_Init            2001
#define SYS_doom_DrawFrame       2002
#define SYS_doom_SleepMs         2003
#define SYS_doom_GetTicksMs      2004
#define SYS_doom_GetKey          2005
#define SYS_doom_SetWindowTitle  2006

namespace postrisc {

template<class T>
using
memory_aligned_vector = std::vector<T, boost::alignment::aligned_allocator<T, 64 * 1024>>;

template<size_t N>
class CHostAccessBlock {
public:
    using vm_t = util::default_initializer<u32, 0>;
public:
    std::array<vm_t, N> data;
};

class TargetSingleApp final : public IManagedAddressSpace<TargetSingleApp> {
public:
    TargetSingleApp(Decoder& decoder, const IDoomInterface& doom);
    TargetSingleApp(size_t pagesize, Decoder& decoder, const IDoomInterface& doom);
    ~TargetSingleApp(void) override;

    STANDARD_ADDRESS_SPACE_INTERFACE

    int DoSyscall(unsigned code, u64 &result, u64 r1, u64 r2, u64 r3, u64 r4, u64 r5, u64 r6) override;
    int AddHostMemory(void const *p, size_t size, u64 protection);
    int RemoveHostMemory(void *p, size_t size);
    int StartEmulation(const EmulatorConfiguration& configuration, int argc, char**argv, char**env);
    void dump(std::ostream& out);

    template<class helper>
    std::tuple<CStatus, CPhysAddress> TranslateAddress(Core &core,
        const CVirtAddress va, u64 requested_rights, u64 unexpected_rights) const;

private:
    static int TargetProtectionToHost(int protection);
    static u64 TargetProtectionToEmulator(int protection);

    uintptr_t PageSizeRoundDown(uintptr_t addr) const;
    uintptr_t PageSizeRoundUp(uintptr_t addr) const;

    const std::span<std::byte> virtual_alloc(size_t size);
    void virtual_free(const std::span<std::byte> data);

    int load_program(const std::span<const std::byte> original_file, ElfInfo& elf_info, std::span<std::byte>& image);
    int load_elf_module(char const *file_name, ElfInfo& elf_info, std::span<std::byte>& image);

    template<typename ELF> size_t ComputeMapSize(
        const std::span<const typename ELF::ELF_PHDR> phdr);
    template<typename ELF> int load_program(
        const std::span<const std::byte> original_file, ElfInfo& elf_info, std::span<std::byte>& image);
    template<typename ELF> int map_protect(
        const std::span<const std::byte> image, const std::span<const typename ELF::ELF_PHDR> program_headers);
    template<typename ELF> int map_image(
        const std::span<std::byte> mapped_image, const std::span<const std::byte> original_file,
        const std::span<const typename ELF::ELF_PHDR> program_headers);
    template<typename ELF> int parse_symtab(
        const std::span<const typename ELF::ELF_SHDR> shdr, const typename ELF::ELF_SHDR& section,
        const typename ELF::ELF_SHDR& str_section, const std::span<const char> shstrtab,
        const std::span<const std::byte> original_file, const std::span<const std::byte> image, Decoder& tempDecoder);
    template<typename ELF> int parse_code(
        const typename ELF::ELF_SHDR& section, const std::span<const std::byte> image, Decoder& tempDecoder);
    template<typename ELF> int parse_dynamic(
        const typename ELF::ELF_SHDR& section,
        const std::span<const std::byte> original_file, const std::span<const std::byte> image, ElfInfo& elf_info);
    template<typename ELF> int parse_rel(
        const typename ELF::ELF_SHDR& section, const std::span<const std::byte> original_file, const std::span<std::byte> image);
    template<typename ELF> int parse_rela(
        const typename ELF::ELF_SHDR& section, const std::span<const std::byte> original_file, const std::span<std::byte> image);
    template<typename ELF> u8* prepare_stack(
        const std::span<std::byte> stack, int argc, char **argv, char **env, ElfInfo& elf_info);

    void prepare_backstore(const std::span<std::byte> data);
    void dump_memory(std::ostream&, unsigned flags) const;
    void report_page_fault(u64 address) const;
    void report_access_fault(u64 address) const;

    u64 system_brk(u64 addr);
    u64 system_close(u64 fd);
    u64 system_clock_gettime(u64 clk_id, u64 pres);
    u64 system_exit_group(u64 status);
    u64 system_exit(u64 status);
    u64 system_ioctl(u64 fd, u64 request, u64 param1, u64 param2, u64 param3, u64 param4);
    u64 system_lseek(u64 fd, u64 offset, u64 whence);
    u64 system_mkdir(u64 pathname, u64 mode);
    u64 system_mmap(u64 addr, u64 length, u64 prot, u64 flags, u64 fd, u64 offset);
    u64 system_munmap(u64 addr, u64 length);
    u64 system_mprotect(u64 addr, u64 len, u64 prot);
    u64 system_nanosleep(u64 req, u64 rem);
    u64 system_open(u64 pathname, u64 flags, u64 mode);
    u64 system_pipe2(u64 pfildes, u64 flags);
    u64 system_read(u64 fd, u64 addr, u64 size);
    u64 system_readv(u64 fd, u64 piovec, u64 iovcnt);
    u64 system_rt_sigaction(u64 signum, u64 psigaction_act, u64 psigaction_oldact);
    u64 system_rt_sigprocmask(u64 how, u64 psigset, u64 poldsigset);
    u64 system_set_tid_address(u64 tidptr);
    u64 system_wait4(u64 pid, u64 stat_addr, u64 options, u64 rusage_addr);
    u64 system_write(u64 fd, u64 addr, u64 size);
    u64 system_writev(u64 fd, u64 piovec, u64 iovcnt);
    // debug syscalls
    u64 system_dump_value(u64 line, u64 file, u64 func, u64 value);
    u64 system_dump_memory(u64 line, u64 file, u64 func, u64 addr, u64 size);
    // doom syscalls
    u64 system_doom_Init(u64 resx, u64 resy);
    u64 system_doom_DrawFrame(u64 screenbuffer, u64 resx, u64 resy);
    u64 system_doom_SleepMs(u64 ms);
    u64 system_doom_GetTicksMs(void);
    u64 system_doom_GetKey(u64 ppressed, u64 pcdoomKey);
    u64 system_doom_SetWindowTitle(u64 pctitle);

private:
    bool syscall_force_fail = false;

    // group pages mapping
    static const size_t pages_per_block = 1024;
    std::map<uintptr_t, CHostAccessBlock<pages_per_block>>   m_HostAccessMap;

    const IDoomInterface& doom_interface;
    std::chrono::steady_clock::time_point start_time;
    size_t   m_frames_num = 0;
};

template<class helper>
ALWAYS_INLINE
std::tuple<CStatus, CPhysAddress>
TargetSingleApp::TranslateAddress(Core& UNUSED(core), const CVirtAddress va,
    u64 requested_rights, u64 unexpected_rights) const
{
    LOG_DEBUG(LOG_PAGETABLE)
        << "address=" << va
        << " rights=" << fmt::hex(requested_rights);

    u64 const address = va.get_address();
    u64 const page = get_page_number(address);

    auto it = m_HostAccessMap.find(page / pages_per_block);
    assert(!m_HostAccessMap.empty());
    if (unlikely(it == m_HostAccessMap.end())) {
        report_page_fault(address);
        return { helper::invalid_address(), CPhysAddress(0) };
    }

    // check access
    auto const protection = it->second.data[page % pages_per_block];
    if (unlikely((protection & (requested_rights | unexpected_rights))
                 != requested_rights)) {
        report_access_fault(address);
        return { helper::access_right(), CPhysAddress(0) };
    }

    return { CStatus::continue_execution, CPhysAddress(address) };
}

#define SINGLEAPP_READ_TYPED_HANDLER(TYPE)                                   \
inline CStatus                                                               \
TargetSingleApp::bus_read_##TYPE(const CPhysAddress pa, TYPE& data) const {  \
    /* LOG_DEBUG(LOG_PLATFORM) << "address=" << pa; */                       \
    u64 const address = pa.get_address();                                    \
    TYPE const *host_address = reinterpret_cast<TYPE const *>(address);      \
    data = *host_address;                                                    \
    return CStatus::continue_execution;                                      \
}

SINGLEAPP_READ_TYPED_HANDLER(u8)
SINGLEAPP_READ_TYPED_HANDLER(u16)
SINGLEAPP_READ_TYPED_HANDLER(u32)
SINGLEAPP_READ_TYPED_HANDLER(u64)
SINGLEAPP_READ_TYPED_HANDLER(u128)

#define SINGLEAPP_WRITE_TYPED_HANDLER(TYPE)                                  \
inline CStatus                                                               \
TargetSingleApp::bus_write_##TYPE(const CPhysAddress pa, TYPE data) {        \
    /* LOG_DEBUG(LOG_PLATFORM) << "address=" << pa; */                       \
    u64 const address = pa.get_address();                                    \
    TYPE *host_address = reinterpret_cast<TYPE *>(address);                  \
    *host_address = data;                                                    \
    return CStatus::continue_execution;                                      \
}

SINGLEAPP_WRITE_TYPED_HANDLER(u8)
SINGLEAPP_WRITE_TYPED_HANDLER(u16)
SINGLEAPP_WRITE_TYPED_HANDLER(u32)
SINGLEAPP_WRITE_TYPED_HANDLER(u64)
SINGLEAPP_WRITE_TYPED_HANDLER(u128)

inline CStatus
TargetSingleApp::bus_read(const CPhysAddress pa, size_t len, void *bytes) const
{
    //LOG_DEBUG(LOG_PLATFORM) << "address=" << pa << " len=" << len;
    u64 const address = pa.get_address();
    u8 const *host_address = reinterpret_cast<u8 const *>(address);
    memcpy(bytes, host_address, len);
    return CStatus::continue_execution;
}

inline CStatus
TargetSingleApp::bus_write(const CPhysAddress pa, size_t len, const void *bytes)
{
    //LOG_DEBUG(LOG_PLATFORM) << "address=" << pa << " len=" << len;
    u64 const address = pa.get_address();
    u8 *host_address = reinterpret_cast<u8 *>(address);
    memcpy(host_address, bytes, len);
    return CStatus::continue_execution;
}

inline CStatus
TargetSingleApp::bus_complex_operation(const CPhysAddress pa, size_t UNUSED(len), IMemoryOperation& op)
{
    //LOG_DEBUG(LOG_PLATFORM) << "address=" << pa << " len=" << len;
    u64 const address = pa.get_address();
    u8 *host_address = reinterpret_cast<u8 *>(address);
    return op.complex_operation(host_address);
}

} // namespace postrisc
