#if !defined(__linux__)

#include <windows.h>
#include <errno.h>
#include <io.h>
#include <direct.h>

#include "posix.h"
#include "util/logger.hpp"
#include "util/fmt.hpp"

#ifndef FILE_MAP_EXECUTE
#define FILE_MAP_EXECUTE    0x0020
#endif /* FILE_MAP_EXECUTE */

namespace postrisc {

int getpid(void)
{
    return GetCurrentProcessId();
}

int gettid(void)
{
    return GetCurrentThreadId();
}

int getuid()
{
    return 1000;
}
int geteuid()
{
    return 1000;
}
int getgid()
{
    return 1000;
}

int getegid()
{
    return 1000;
}

static int __map_mman_error(const DWORD err, const int UNUSED(deferr))
{
    if (err == 0)
        return 0;
    //TODO: implement
    return err;
}

static DWORD __map_mmap_prot_page(const int prot)
{
    DWORD protect = 0;
    
    if (prot == PROT_NONE)
        return PAGE_READONLY; //protect;
        
    if ((prot & PROT_EXEC) != 0) {
        protect = (prot & PROT_WRITE) ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_READ;
    } else {
        protect = (prot & PROT_WRITE) ? PAGE_READWRITE : PAGE_READONLY;
    }
    
    return protect;
}

static DWORD __map_mmap_prot_file(const int prot)
{
    DWORD desiredAccess = 0;
    
    if (prot == PROT_NONE)
        return desiredAccess;
        
    if ((prot & PROT_READ) != 0)
        desiredAccess |= FILE_MAP_READ;
    if ((prot & PROT_WRITE) != 0)
        desiredAccess |= FILE_MAP_WRITE;
    if ((prot & PROT_EXEC) != 0)
        desiredAccess |= FILE_MAP_EXECUTE;
    
    return desiredAccess;
}

void* mmap(void *addr, size_t len, int prot, int flags, int fildes, size_t off)
{
    LOG_ERROR(LOG_LOADER) << "mmap: addr=" << fmt::hex(addr) << " off=0x" << fmt::hex(off) << " len=0x" << fmt::hex(len);

    void *ptr = VirtualAlloc(nullptr, len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!ptr) return MAP_FAILED;
    return ptr;

    HANDLE fm, h;
    
    const DWORD dwFileOffsetLow  = (DWORD)((off      ) & 0xFFFFFFFFUL);
    const DWORD dwFileOffsetHigh = (DWORD)((off >> 32) & 0xFFFFFFFFUL);

    const DWORD protect = __map_mmap_prot_page(prot);
    const DWORD desiredAccess = __map_mmap_prot_file(prot);

    const u64 maxSize = off + len;

    const DWORD dwMaxSizeLow  = (DWORD)((maxSize      ) & 0xFFFFFFFFUL);
    const DWORD dwMaxSizeHigh = (DWORD)((maxSize >> 32) & 0xFFFFFFFFUL);

    errno = 0;
    
    if (len == 0 
        /* Unsupported flag combinations */
        // || (flags & MAP_FIXED) != 0
        /* Usupported protection combinations */
        || prot == PROT_EXEC) {
        errno = EINVAL;
        LOG_ERROR(LOG_LOADER) << "Unsupported combinations";
        return MAP_FAILED;
    }
    
    h = ((flags & MAP_ANONYMOUS) == 0) ? 
                    (HANDLE)_get_osfhandle(fildes) : INVALID_HANDLE_VALUE;

    if ((flags & MAP_ANONYMOUS) == 0 && h == INVALID_HANDLE_VALUE) {
        errno = EBADF;
        LOG_ERROR(LOG_LOADER) << "MAP_ANONYMOUS";
        return MAP_FAILED;
    }

    fm = CreateFileMapping(h, NULL, protect, dwMaxSizeHigh, dwMaxSizeLow, NULL);

    if (fm == NULL) {
        LOG_ERROR(LOG_LOADER) << "CreateFileMapping failed, h=" << fmt::hex(h) << " GLE=" << GetLastError();
        errno = __map_mman_error(GetLastError(), EPERM);
        return MAP_FAILED;
    }

    void *map = MapViewOfFileEx(fm, desiredAccess, dwFileOffsetHigh, dwFileOffsetLow, len, addr);

    CloseHandle(fm);
  
    if (map == NULL) {
        errno = __map_mman_error(GetLastError(), EPERM);
        LOG_ERROR(LOG_LOADER) << "MapViewOfFileEx failed";
        return MAP_FAILED;
    }

    LOG_ERROR(LOG_LOADER) << "success " << fmt::hex(map);
    return map;
}

int munmap(void *addr, size_t len)
{
    VirtualFree(addr, len, MEM_DECOMMIT);
    return 0;

    if (UnmapViewOfFile(addr))
        return 0;
        
    errno =  __map_mman_error(GetLastError(), EPERM);
    
    return -1;
}

int mprotect(void *addr, size_t len, int prot)
{
    DWORD newProtect = __map_mmap_prot_page(prot);
    DWORD oldProtect = 0;
    
    if (VirtualProtect(addr, len, newProtect, &oldProtect))
        return 0;
    
    errno =  __map_mman_error(GetLastError(), EPERM);
    
    return -1;
}

int msync(void *addr, size_t len, int UNUSED(flags))
{
    if (FlushViewOfFile(addr, len))
        return 0;
    
    errno =  __map_mman_error(GetLastError(), EPERM);
    
    return -1;
}

int mlock(const void *addr, size_t len)
{
    if (VirtualLock(const_cast<void*>(addr), len))
        return 0;

    errno =  __map_mman_error(GetLastError(), EPERM);
    return -1;
}

int munlock(const void *addr, size_t len)
{
    if (VirtualUnlock(const_cast<void*>(addr), len))
        return 0;

    errno =  __map_mman_error(GetLastError(), EPERM);
    return -1;
}

static ssize_t readv_writev(int fd, const struct iovec *iov, unsigned iov_cnt, bool do_write) {
    unsigned i = 0;
    LOG_DEBUG(LOG_SYSCALL) << "fd=" << fd << " iov_cnt=" << iov_cnt;
    ssize_t ret = 0;
    while (i < iov_cnt) {
        ssize_t r = do_write
            ? write(fd, iov[i].iov_base, iov[i].iov_len)
            : read(fd, iov[i].iov_base, iov[i].iov_len);
        LOG_DEBUG(LOG_SYSCALL) << "iov: base=" << fmt::hex(std::bit_cast<uintptr_t>(iov[i].iov_base)) << " len=" << iov[i].iov_len << " r=" << r;
        if (r > 0) {
            ret += r;
        } else if (r == 0) {
            LOG_DEBUG(LOG_SYSCALL) << "r==0";
            // break;
        //} else if (errno == EINTR) {
        //    continue;
        } else {
            /* else it is some "other" error,
             * only return if there was no data processed. */
            LOG_DEBUG(LOG_SYSCALL) << "errno=" << errno;
            if (ret == 0) {
                LOG_DEBUG(LOG_SYSCALL) << "ret==0";
                ret = -1;
            }
            break;
        }
        i++;
    }
    LOG_DEBUG(LOG_SYSCALL) << "ret=" << ret;
    //Sleep(10);
    return ret;
}

ssize_t readv(int fd, const struct iovec *iov, unsigned iov_cnt) {
    return readv_writev(fd, iov, iov_cnt, false);
}

ssize_t writev(int fd, const struct iovec *iov, unsigned iov_cnt) {
    return readv_writev(fd, iov, iov_cnt, true);
}

int clock_gettime(int, struct timespec *spec)      //C-file part
{  __int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
   wintime      -=INT64_C(116444736000000000);  //1jan1601 to 1jan1970
   spec->tv_sec  =wintime / INT64_C(10000000);           //seconds
   spec->tv_nsec =wintime % INT64_C(10000000) *100;      //nano-seconds
   return 0;
}

#define POW10_3                 1000
#define POW10_4                 10000
#define POW10_6                 1000000
#define POW10_9                 1000000000
#define MAX_SLEEP_IN_MS         4294967294UL

/**
 * Sleep for the specified time.
 * @param  request The desired amount of time to sleep.
 * @param  remain The remain amount of time to sleep.
 * @return If the function succeeds, the return value is 0.
 *         If the function fails, the return value is -1,
 *         with errno set to indicate the error.
 */
int nanosleep(const struct timespec *request, struct timespec *remain)
{
    u64 ms, rc = 0;
    u64 ui64, want, real;

    union {
        unsigned __int64 ns100;
        FILETIME ft;
    }  _start, _end;

    if (request->tv_sec < 0 || request->tv_nsec < 0 || request->tv_nsec >= POW10_9) {
        errno = EINVAL;
        return -1;
    }

    if (remain != NULL) GetSystemTimeAsFileTime(&_start.ft);

    want = ui64 = request->tv_sec * POW10_3 + request->tv_nsec / POW10_6;
    while (ui64 > 0 && rc == 0) {
        if (ui64 >= MAX_SLEEP_IN_MS) ms = MAX_SLEEP_IN_MS;
        else ms = (u64) ui64;

        ui64 -= ms;
        Sleep(ms);
        rc = 0; //pthread_delay_np_ms(ms);
    }

    if (rc != 0) { /* WAIT_IO_COMPLETION (192) */
        if (remain != NULL) {
            GetSystemTimeAsFileTime(&_end.ft);
            real = (_end.ns100 - _start.ns100) / POW10_4;

            if (real >= want) ui64 = 0;
            else ui64 = want - real;

            remain->tv_sec = ui64 / POW10_3;
            remain->tv_nsec = (long) (ui64 % POW10_3) * POW10_6;
        }

        errno = EINTR;
        return -1;
    }

    return 0;
}

/* Wait for a child matching PID to die.
   If PID is greater than 0, match any process whose process ID is PID.
   If PID is (pid_t) -1, match any process.
   If PID is (pid_t) 0, match any process with the
   same process group as the current process.
   If PID is less than -1, match any process whose
   process group is the absolute value of PID.
   If the WNOHANG bit is set in OPTIONS, and that child
   is not already dead, return (pid_t) 0.  If successful,
   return PID and store the dead child's status in STAT_LOC.
   Return (pid_t) -1 for errors.  If the WUNTRACED bit is set in OPTIONS,
   return status for stopped children; otherwise don't.  */
pid_t waitpid (pid_t pid, int *stat_loc, int options)
{
    HANDLE hProcess;
    DWORD status;
    
    if ((options & ~(WNOHANG|WUNTRACED)) != 0) {
        _set_errno (EINVAL);
        return (pid_t) -1;
    }
    if ((pid == -1) || (pid == -2)) {
        errno = ECHILD;
        return -1;
    }
    hProcess = OpenProcess (SYNCHRONIZE, FALSE, pid);
    if (hProcess == NULL) {
        //set_werrno;
        return -1;
    }
    if (WaitForSingleObject (hProcess, (DWORD) (-1L)) == WAIT_FAILED) {
        //set_werrno;
        return -1;
    }
    if (!GetExitCodeProcess (hProcess, &status)) {
        //set_werrno;
        return -1;
    }
    if (!stat_loc)
        *stat_loc = status;
    CloseHandle (hProcess);
    return pid;  
}

pid_t wait4(pid_t pid, int *stat_loc, int options, struct rusage *usage)
{
  if (usage != NULL)
    {
      _set_errno (ENOSYS);
      return (pid_t) -1;
    }
  return waitpid(pid, stat_loc, options);
}

} // namespace postrisc

#endif // !defined(__linux__)
