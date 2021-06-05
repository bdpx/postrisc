#if defined(__linux__)

#include <unistd.h>          // sbrk, lseek, pipe2
#include <sys/uio.h>         // readv, writev
#include <sys/mman.h>        // mmap
#include <sys/stat.h>        // mkdir
#include <sys/wait.h>        // wait4
#include <fcntl.h>           // open
#include <time.h>            // clock_gettime, nanosleep

#include <sys/syscall.h>     // SYS_ numbers

#else
#include "windows/syscall.h" // SYS_ numbers
#include "windows/posix.h"
#endif // __linux__

#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "emulator/Core.hpp"
#include "disassembler/Decoder.hpp"
#include "TargetSingleApp.hpp"

namespace postrisc {

u64 TargetSingleApp::system_dump_value(u64 line, u64 _file, u64 _func, u64 value)
{
    char const *file = reinterpret_cast<char const *>(_file);
    char const *func = reinterpret_cast<char const *>(_func);
    dbgs() << "DUMP: " << file << ":" << func << ":" << line << " " << fmt::hex(value) << std::endl;
    return 0;
}

u64 TargetSingleApp::system_dump_memory(u64 line, u64 _file, u64 _func, u64 _addr, u64 size)
{
    char const *file = reinterpret_cast<char const *>(_file);
    char const *func = reinterpret_cast<char const *>(_func);
    dbgs() << "DUMPMEM: " << file << ":" << func << ":" << line << " size=" << size << std::endl;
    u8 const *addr = reinterpret_cast<u8 const *>(_addr);
    fmt::hexdump(addr, size, _addr, dbgs());
    dbgs() << std::endl;
    return 0;
}

u64 TargetSingleApp::system_clock_gettime(u64 clk_id, u64 pres)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_clock_gettime: id=" << clk_id;
    return clock_gettime(clk_id, reinterpret_cast<struct timespec *>(pres));
}

u64 TargetSingleApp::system_nanosleep(u64 req, u64 rem)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_nanosleep: req=" << fmt::hex(req) << " rem=" << fmt::hex(rem);
    int result = nanosleep(reinterpret_cast<const struct timespec *>(req), reinterpret_cast<struct timespec *>(rem));
    if (result < 0) {
        LOG_DEBUG(LOG_SYSCALL) << "SYS_nanosleep failed: " << strerror(errno);
        return -1LL;
    }
    return result;
}

u64 TargetSingleApp::system_open(u64 _pathname, u64 flags, u64 mode)
{
    char const *pathname = reinterpret_cast<char const *>(_pathname);
    LOG_DEBUG(LOG_SYSCALL) << "SYS_open: pathname=" << pathname;
    int fd = open(pathname, flags, mode);
    if (fd < 0) {
        LOG_DEBUG(LOG_SYSCALL) << "SYS_open failed: " << errno;
        return -1LL;
    }
    LOG_DEBUG(LOG_SYSCALL) << "SYS_open: fd=" << fd;
    return fd;
}

u64 TargetSingleApp::system_lseek(u64 fd, u64 offset, u64 whence)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_lseek:"
        << " fd=" << fd
        << " offset=" << (off_t)offset
        << " whence=" << whence;
    off_t result = lseek((int)fd, (off_t)offset, whence);
    if (result == (off_t) -1) {
        LOG_DEBUG(LOG_SYSCALL) << "SYS_lseek failed: " << errno;
        return -1LL;
    }
    return result;
}

u64 TargetSingleApp::system_read(u64 fd, u64 addr, u64 size)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_read: fd=" << (int)fd << " addr" << fmt::hex(addr) << " size=" << size;
    u64 result = read((int)fd, reinterpret_cast<void*>(addr), size);
    if ((intptr_t)result < 0) {
        LOG_DEBUG(LOG_SYSCALL) << "SYS_read failed: " << errno;
        result = -1LL;
    }
    return result;
}

u64 TargetSingleApp::system_readv(u64 fd, u64 piovec, u64 iovcnt)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_readv: fd=" << (int)fd << " piovec" << fmt::hex(piovec) << " iovcnt=" << iovcnt;
    u64 result = readv((int)fd, reinterpret_cast<const struct iovec *>(piovec), iovcnt);
    if ((intptr_t)result < 0) {
        LOG_DEBUG(LOG_SYSCALL) << "SYS_readv failed: " << errno;
        result = -1LL;
    }
    return result;
}

u64 TargetSingleApp::system_write(u64 fd, u64 addr, u64 size)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_write";
    u64 result = write((int)fd, reinterpret_cast<void*>(addr), size);
    if ((intptr_t)result < 0) {
        LOG_DEBUG(LOG_SYSCALL) << "SYS_write failed: " << errno;
        result = -1LL;
    }
    return result;
}

u64 TargetSingleApp::system_writev(u64 fd, u64 piovec, u64 iovcnt)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_writev";
    u64 result = writev((int)fd, reinterpret_cast<const struct iovec *>(piovec), (int)iovcnt);
    if ((intptr_t)result < 0) {
        LOG_DEBUG(LOG_SYSCALL) << "SYS_writev failed: " << errno;
        result = -1LL;
    }
    return result;
}

u64 TargetSingleApp::system_close(u64 fd)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_close: fd=" << fd;
    if (close(fd) < 0) {
        LOG_DEBUG(LOG_SYSCALL) << "SYS_close failed " << errno;
        return -1LL;
    }
    return 0;
}

u64 TargetSingleApp::system_brk(u64 increment)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_brk increment=" << fmt::hex(increment);
    // return some random nonzero value, ignore attempts to increase limit
    return U64C(0x10000000);
}

u64 TargetSingleApp::system_exit_group(u64 status)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_exit_group: status=" << status;
    return 0;
}

u64 TargetSingleApp::system_exit(u64 status)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_exit: status=" << status;
    return 0;
}

u64 TargetSingleApp::system_ioctl(u64 fd, u64 request,
    u64 UNUSED(param1), u64 UNUSED(param2), u64 UNUSED(param3), u64 UNUSED(param4))
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_ioctl: fd=" << fd << " request=" << request;
    return 0;
}

u64 TargetSingleApp::system_mmap(u64 addr, u64 length, u64 prot, u64 flags, u64 fd, u64 offset)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_mmap: addr=" << fmt::hex(addr)
                           << " len=" << length
                           << " prot=" << prot
                           << " fd=" << (int)fd
                           << " offset=" << offset;
    length = PageSizeRoundUp(length);
    // u64 result = mmap((int)fd, (const struct iovec *)r2/*iov*/, (int)r3/*iovcnt*/);

    void *base = mmap(reinterpret_cast<void*>(addr), (size_t)length, TargetProtectionToHost(prot), flags, (int)fd, offset);
    if (base == MAP_FAILED) {
        LOG_DEBUG(LOG_SYSCALL) << "mmap failed: " << strerror(errno);
        return -1LL;
    }
    AddHostMemory(base, length, TargetProtectionToEmulator(prot));
    LOG_DEBUG(LOG_SYSCALL) << "SYS_mmap: result=" << fmt::hex((u64)base);
    return (u64)base;
}

u64 TargetSingleApp::system_munmap(u64 addr, u64 length)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_munmap: addr=" << fmt::hex(addr) << " len=" << length;
    if (munmap(reinterpret_cast<void*>(addr), length) < 0) {
        LOG_DEBUG(LOG_SYSCALL) << "munmap failed " << strerror(errno);
        return -1LL;
    } else {
        RemoveHostMemory(reinterpret_cast<void*>(addr), length);
    }
    return 0;
}

u64 TargetSingleApp::system_mprotect(u64 addr, u64 len, u64 prot)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_mprotect: addr=" << fmt::hex(addr)
                           << " len=" << fmt::hex(len) << " prot=" << prot;
    len = PageSizeRoundUp(len);
    if (mprotect(reinterpret_cast<void*>(addr), len, TargetProtectionToHost(prot)) < 0) {
        LOG_DEBUG(LOG_SYSCALL) << "SYS_mprotect failed " << errno;
        return -1LL;
    }
    AddHostMemory(reinterpret_cast<void*>(addr), len, TargetProtectionToEmulator(prot));
    return 0;
}

u64 TargetSingleApp::system_set_tid_address(u64 tidptr)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_set_tid_address: tidptr=" << fmt::hex(tidptr);
    pid_t tid = gettid();
    LOG_DEBUG(LOG_SYSCALL) << "SYS_set_tid_address: tid=" << tid;
    return tid;
}

u64 TargetSingleApp::system_mkdir(u64 _pathname, u64 mode)
{
    const char *pathname = reinterpret_cast<const char *>(_pathname);
    LOG_DEBUG(LOG_SYSCALL) << "SYS_mkdir: " << pathname;
#if defined(__linux__)
    if (mkdir(pathname, mode) < 0) {
#else
    if (mkdir(pathname) < 0) {
#endif
        LOG_DEBUG(LOG_SYSCALL) << "mkdir failed " << errno;
        return -1LL;
    }
    return 0;
}

u64 TargetSingleApp::system_rt_sigaction(u64 signum, u64 UNUSED(psigaction_act), u64 UNUSED(psigaction_oldact))
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_rt_sigaction: signum=" << signum;
    return 0;
}

u64 TargetSingleApp::system_rt_sigprocmask(u64 how, u64 UNUSED(psigset), u64 UNUSED(poldsigset))
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_rt_sigprocmask: how=" << how;
    return 0;
}

u64 TargetSingleApp::system_pipe2(u64 pfildes, u64 flags)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_pipe2: fildes=" << fmt::hex(pfildes) << " how=" << flags;
    return -1LL;

    /*
    int *fildes = (int *)pfildes;
    if (pipe2(fildes, flags) < 0) {
        LOG_DEBUG(LOG_SYSCALL) << "pipe2 failed " << errno;
        return -1LL;
    }
    return 0;
    */
}

u64 TargetSingleApp::system_wait4(u64 pid, u64 stat_addr, u64 options, u64 rusage_addr)
{
    LOG_DEBUG(LOG_SYSCALL) << "SYS_wait4:"
        << " pid=" << pid
        << " stat_addr=" << fmt::hex(stat_addr)
        << " options=" << options
        << " rusage_addr=" << fmt::hex(rusage_addr);

    pid_t result = wait4((pid_t)pid, reinterpret_cast<int *>(stat_addr), (int)options, reinterpret_cast<struct rusage *>(rusage_addr));
    if (result < 0) return -1LL;
    return result;
}

#define ARGS_0 /* nothing*/
#define ARGS_1 r1
#define ARGS_2 r1, r2
#define ARGS_3 r1, r2, r3
#define ARGS_4 r1, r2, r3, r4
#define ARGS_5 r1, r2, r3, r4, r5
#define ARGS_6 r1, r2, r3, r4, r5, r6

#define HANDLE_SYSCALL(COUNT, NAME)                    \
    case SYS_ ## NAME:                                 \
        LOG_DEBUG(LOG_SYSCALL) << "syscall: " #NAME;   \
        result = system_ ## NAME (ARGS_ ## COUNT);     \
        break;

int TargetSingleApp::DoSyscall(unsigned code, u64& result_place,
    u64 r1, u64 r2, u64 r3, u64 r4, u64 r5, u64 r6)
{
    LOG_DEBUG(LOG_SYSCALL) << "syscall: N=" << code
        << ' '<< fmt::hex(r1)
        << ' '<< fmt::hex(r2)
        << ' '<< fmt::hex(r3)
        << ' '<< fmt::hex(r4)
        << ' '<< fmt::hex(r5)
        << ' '<< fmt::hex(r6);

    syscall_force_fail = false;

    u64 result;
    switch (code) {
        HANDLE_SYSCALL(2, clock_gettime)
        HANDLE_SYSCALL(1, close)
        HANDLE_SYSCALL(1, set_tid_address)
        HANDLE_SYSCALL(1, brk)
        HANDLE_SYSCALL(3, read)
        HANDLE_SYSCALL(3, readv)
        HANDLE_SYSCALL(3, write)
        HANDLE_SYSCALL(6, ioctl)
        HANDLE_SYSCALL(3, writev)
        HANDLE_SYSCALL(3, lseek)
        HANDLE_SYSCALL(6, mmap)
        HANDLE_SYSCALL(2, munmap)
        HANDLE_SYSCALL(3, mprotect)
        HANDLE_SYSCALL(2, mkdir)
        HANDLE_SYSCALL(2, nanosleep)
        HANDLE_SYSCALL(3, open)
        HANDLE_SYSCALL(3, rt_sigaction)
        HANDLE_SYSCALL(3, rt_sigprocmask)
        HANDLE_SYSCALL(2, pipe2)
        HANDLE_SYSCALL(4, wait4)
        HANDLE_SYSCALL(1, exit_group)
        HANDLE_SYSCALL(1, exit)
        // debug syscalls
        HANDLE_SYSCALL(4, dump_value)
        HANDLE_SYSCALL(5, dump_memory)
        // doom syscalls
        HANDLE_SYSCALL(2, doom_Init)
        HANDLE_SYSCALL(4, doom_DrawFrame)
        HANDLE_SYSCALL(1, doom_SleepMs)
        HANDLE_SYSCALL(0, doom_GetTicksMs)
        HANDLE_SYSCALL(2, doom_GetKey);
        HANDLE_SYSCALL(1, doom_SetWindowTitle)
        default:
            LOG_ERROR(LOG_EXCEPTION | LOG_SYSCALL) << "unknown syscall: N=" << code
                    << ' ' << fmt::hex(r1)
                    << ' ' << fmt::hex(r2)
                    << ' ' << fmt::hex(r3)
                    << ' ' << fmt::hex(r4)
                    << ' ' << fmt::hex(r5)
                    << ' ' << fmt::hex(r6);
            return -1;
    }

    if (code == SYS_exit || code == SYS_exit_group) {
        dbgs() << "<pre>exit detected " << code << " status=" << r1 << "</pre>" << std::endl;
        return -1;
    }

    if (syscall_force_fail) {
        dbgs() << "syscall " << code << " failed and forces exit" << std::endl;
        return -1;
    }

    result_place = (static_cast<i64>(result) != -1LL) ? result : -((errno) & 0xfff);
    return 0;
}

} // namespace postrisc
