#pragma once

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501    // Change this to the appropriate value to target other versions of Windows.
#endif

/* All the headers include this file. */
#ifndef _MSC_VER
#include <_mingw.h>
#endif

#include <sys/types.h>

namespace postrisc {

int getuid();
int geteuid();
int getgid();
int getegid();

int getpid(void);
int gettid(void);

struct iovec {
    void *iov_base;
    size_t iov_len;
};

ssize_t readv(int fd, const struct iovec *iov, unsigned iov_cnt);
ssize_t writev(int fd, const struct iovec *iov, unsigned iov_cnt);

#define PROT_NONE       0
#define PROT_READ       1
#define PROT_WRITE      2
#define PROT_EXEC       4

#define MAP_FILE        0
#define MAP_SHARED      1
#define MAP_PRIVATE     2
#define MAP_TYPE        0xf
#define MAP_FIXED       0x10
#define MAP_ANONYMOUS   0x20
#define MAP_ANON        MAP_ANONYMOUS

#define MAP_FAILED      ((void *)-1)

/* Flags for msync. */
#define MS_ASYNC        1
#define MS_SYNC         2
#define MS_INVALIDATE   4

void*   mmap(void *addr, size_t len, int prot, int flags, int fildes, size_t off);
int     munmap(void *addr, size_t len);
int     mprotect(void *addr, size_t len, int prot);
int     msync(void *addr, size_t len, int flags);
int     mlock(const void *addr, size_t len);
int     munlock(const void *addr, size_t len);

/*
 * Use the same value as Linux for now.
 */
#define IOV_MAX                1024

#define    WNOHANG        1    /* Don't block waiting.  */
#define    WUNTRACED    2    /* Report status of stopped children.  */

//struct timespec {
//   long tv_sec;
//   long tv_nsec;
//};

extern pid_t waitpid (pid_t __pid, int *__stat_loc, int __options);

struct rusage;

/* PID is like waitpid.  Other args are like wait3.  */
extern pid_t wait4 (pid_t __pid, int *__stat_loc, int __options, struct rusage *__usage);

} // namespace postrisc
