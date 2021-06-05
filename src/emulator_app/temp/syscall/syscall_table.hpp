// syscall database

SYSCALL( read,                   SSIZE_T, PROTO( FD fd, PTR buf, SIZE_T count ) )
SYSCALL( pread,                  SSIZE_T, PROTO( FD fd, PTR buf, SIZE_T count, OFF_T pos ) )
SYSCALL( write,                  SSIZE_T, PROTO( FD fd, CPTR buf, SIZE_T count ) )
SYSCALL( pwrite,                 SSIZE_T, PROTO( FD fd, CPTR buf, SIZE_T count, OFF_T pos ) )
SYSCALL( readv,                  SSIZE_T, PROTO( FD fd, PCIOVEC vec, SIZE_T vlen ) )
SYSCALL( writev,                 SSIZE_T, PROTO( FD fd, PCIOVEC vec, SIZE_T vlen ) )
SYSCALL( preadv,                 SSIZE_T, PROTO( FD fd, PCIOVEC vec, SIZE_T vlen, OFF_T pos ) )
SYSCALL( pwritev,                SSIZE_T, PROTO( FD fd, PCIOVEC vec, SIZE_T vlen, OFF_T pos ) )
SYSCALL( preadv2,                SSIZE_T, PROTO( FD fd, PCIOVEC vec, SIZE_T vlen, OFF_T pos, rwf_t flags ) )
SYSCALL( pwritev2,               SSIZE_T, PROTO( FD fd, PCIOVEC vec, SIZE_T vlen, OFF_T pos, rwf_t flags ) )

SYSCALL( open,                   FD,      PROTO( STR filename, int flags, umode_t mode ) )
SYSCALL( openat,                 FD,      PROTO( FD dfd, STR filename, int flags, umode_t mode ) )
SYSCALL( close,                  STATUS,  PROTO( FD fd ) )
SYSCALL( lseek,                  OFF_T,   PROTO( FD fd, OFF_T offset, unsigned int whence ) )
SYSCALL( ioctl,                  STATUS,  PROTO( FD fd, unsigned int cmd, unsigned long arg ) )

SYSCALL( stat,                   STATUS,  PROTO( STR filename, struct stat *statbuf ) )
SYSCALL( lstat,                  STATUS,  PROTO( STR filename, struct stat *statbuf ) )
SYSCALL( fstat,                  STATUS,  PROTO( FD fd, struct stat *statbuf ) )
SYSCALL( fstatat,                STATUS,  PROTO( FD dfd, STR filename, struct stat *statbuf, int flag ) )

SYSCALL( poll,                   STATUS,  PROTO( struct pollfd *ufds, unsigned int nfds, int timeout ) )
SYSCALL( ppoll,                  STATUS,  PROTO( struct pollfd *ufds, unsigned int nfds, const struct timespec *tmo_p, const sigset_t *sigmask, SIZE_T sigsetsize) )

// virtual memory
SYSCALL( mmap,                   PTR,     PROTO( PTR addr, SIZE_T len, int prot, int flags, FD fd, OFF_T offset ) )
SYSCALL( mprotect,               STATUS,  PROTO( PTR addr, SIZE_T len, unsigned long prot ) )
SYSCALL( munmap,                 STATUS,  PROTO( PTR addr, SIZE_T len ) )
SYSCALL( mremap,                 STATUS,  PROTO( PTR addr, SIZE_T old_len, SIZE_T new_len, unsigned long flags, PTR new_addr ) )
SYSCALL( remap_file_pages,       STATUS,  PROTO( PTR addr, SIZE_T size, unsigned long prot, SIZE_T pgoff, unsigned long flags ) )
SYSCALL( msync,                  STATUS,  PROTO( PTR addr, SIZE_T len, int flags ) )
SYSCALL( mincore,                STATUS,  PROTO( PTR addr, SIZE_T len, unsigned char * vec ) )
SYSCALL( madvise,                STATUS,  PROTO( PTR addr, SIZE_T len, int behavior ) )
SYSCALL( mbind,                  STATUS,  PROTO( PTR addr, SIZE_T len, unsigned long mode, const unsigned long *nmask, SIZE_T maxnode, unsigned flags ) )
SYSCALL( brk,                    STATUS,  PROTO( PTR addr ) )
SYSCALL( mlock,                  STATUS,  PROTO( PTR addr, SIZE_T len ) )
SYSCALL( mlock2,                 STATUS,  PROTO( PTR addr, SIZE_T len, unsigned int flags ) )
SYSCALL( munlock,                STATUS,  PROTO( PTR addr, SIZE_T len ) )
SYSCALL( mlockall,               STATUS,  PROTO( int flags ) )
SYSCALL( munlockall,             STATUS,  PROTO( ) )

// real-time
SYSCALL( rt_sigaction,           STATUS,  PROTO( int, const struct sigaction *, struct sigaction *, SIZE_T ) )
SYSCALL( rt_sigprocmask,         STATUS,  PROTO( int how, sigset_t *set, sigset_t *oset, SIZE_T sigsetsize ) )
SYSCALL( rt_sigreturn,           STATUS,  PROTO( ?, ?, ?, ?, ?, ? ) )


SYSCALL( access,                 STATUS,  PROTO( STR filename, int mode ) )
SYSCALL( pipe,                   SSIZE_T, PROTO( FD *fildes ) )

SYSCALL( dup,                    FD,      PROTO( FD oldfd ) )
SYSCALL( dup2,                   FD,      PROTO( FD oldfd, FD newfd ) )
SYSCALL( dup3,                   FD,      PROTO( FD oldfd, FD newfd, int flags ) )

SYSCALL( select,                 SSIZE_T, PROTO( int n, fd_set *inp, fd_set *outp, fd_set *exp, struct timeval *tvp ) )
SYSCALL( shmget,                 STATUS,  PROTO( key_t key, SIZE_T size, int flag ) )
SYSCALL( shmat,                  STATUS,  PROTO( int shmid, char *shmaddr, int shmflg ) )
SYSCALL( shmctl,                 STATUS,  PROTO( int shmid, int cmd, struct shmid_ds *buf ) )

// timers/sheduling
SYSCALL( pause,                  STATUS,  PROTO( ) )
SYSCALL( getitimer,              STATUS,  PROTO( int which, struct itimerval *value ) )
SYSCALL( alarm,                  STATUS,  PROTO( unsigned int seconds ) )
SYSCALL( setitimer,              STATUS,  PROTO( int which, struct itimerval *value, struct itimerval *ovalue ) )

SYSCALL( getpid,                 PID,     PROTO( ) )
SYSCALL( sendfile,               STATUS,  PROTO( FD out_fd, FD in_fd, OFF_T *offset, SIZE_T count ) )

// sockets
SYSCALL( socket,                 FD,      PROTO( int, int, int ) )
SYSCALL( connect,                STATUS,  PROTO( FD sockfd, PSOCKADDR, socklen_t ) )
SYSCALL( accept,                 FD,      PROTO( FD sockfd, PSOCKADDR, socklen_t * ) )
SYSCALL( accept4,                FD,      PROTO( FD sockfd, PSOCKADDR, socklen_t *, int ) )
SYSCALL( sendto,                 SSIZE_T, PROTO( FD sockfd, CPTR, SIZE_T, unsigned, PSOCKADDR, int ) )
SYSCALL( recvfrom,               SSIZE_T, PROTO( FD sockfd,  PTR, SIZE_T, unsigned, PSOCKADDR, int * ) )
SYSCALL( sendmsg,                SSIZE_T, PROTO( FD sockfd, struct msghdr *msg, unsigned flags ) )
SYSCALL( recvmsg,                SSIZE_T, PROTO( FD sockfd, struct msghdr *msg, unsigned flags ) )
SYSCALL( sendmmsg,               SSIZE_T, PROTO( FD sockfd, struct mmsghdr *msg, SIZE_T vlen, unsigned flags ) )
SYSCALL( recvmmsg,               SSIZE_T, PROTO( FD sockfd, struct mmsghdr *msg, SIZE_T vlen, unsigned flags, struct timespec *timeout ) )
SYSCALL( shutdown,               STATUS,  PROTO( FD sockfd, int how) )
SYSCALL( bind,                   STATUS,  PROTO( FD sockfd, PSOCKADDR, int ) )
SYSCALL( listen,                 STATUS,  PROTO( FD sockfd, int ) )
SYSCALL( getsockname,            STATUS,  PROTO( FD sockfd, PSOCKADDR, int * ) )
SYSCALL( getpeername,            STATUS,  PROTO( FD sockfd, PSOCKADDR, int * ) )
SYSCALL( socketpair,             STATUS,  PROTO( FD sockfd, int, int, int * ) )
SYSCALL( setsockopt,             STATUS,  PROTO( FD sockfd, int level, int optname, char *optval, int optlen ) )
SYSCALL( getsockopt,             STATUS,  PROTO( FD sockfd, int level, int optname, char *optval, int *optlen ) )

// process
SYSCALL( exit,                   VOID,    PROTO( int error_code ) )
SYSCALL( exit_group,             VOID,    PROTO( int error_code ) )

SYSCALL( clone,                  PID,     PROTO( PTR func, PTR stack, int *, int *, unsigned long ) )
SYSCALL( fork,                   PID,     PROTO( ) )
SYSCALL( vfork,                  PID,     PROTO( ) )
SYSCALL( wait,                   PID,     PROTO( PID_T pid, int *stat_addr, int options, struct rusage *ru ) )
SYSCALL( kill,                   STATUS,  PROTO( PID_T pid, int sig ) )
SYSCALL( tkill,                  STATUS,  PROTO( PID_T pid, int sig ) )
SYSCALL( tgkill,                 STATUS,  PROTO( PID_T tgid, PID_T pid, int sig ) )

SYSCALL( uname,                  STATUS,  PROTO( struct old_utsname * ) )

SYSCALL( execve,                 STATUS,  PROTO( STR filename, STR const *argv, STR const *envp ) )
SYSCALL( execveat,               STATUS,  PROTO( FD dfd, STR filename, STR const *argv, STR const *envp, int flags ) )

SYSCALL( semget,                 STATUS,  PROTO( key_t key, int nsems, int semflg ) )
SYSCALL( semop,                  STATUS,  PROTO( int semid, struct sembuf *sops, unsigned nsops ) )
SYSCALL( semctl,                 STATUS,  PROTO( int semid, int semnum, int cmd, unsigned long arg ) )
SYSCALL( shmdt,                  STATUS,  PROTO( char *shmaddr ) )
SYSCALL( msgget,                 STATUS,  PROTO( key_t key, int msgflg ) )
SYSCALL( msgsnd,                 STATUS,  PROTO( int msqid, struct msgbuf *msgp, SIZE_T msgsz, int msgflg ) )
SYSCALL( msgrcv,                 STATUS,  PROTO( int msqid, struct msgbuf *msgp, SIZE_T msgsz, long msgtyp, int msgflg ) )
SYSCALL( msgctl,                 STATUS,  PROTO( int msqid, int cmd, struct msqid_ds *buf ) )

SYSCALL( fcntl,                  STATUS,  PROTO( FD fd, unsigned int cmd, unsigned long arg ) )
SYSCALL( flock,                  STATUS,  PROTO( FD fd, unsigned int cmd ) )
SYSCALL( fsync,                  STATUS,  PROTO( FD fd ) )
SYSCALL( fdatasync,              STATUS,  PROTO( FD fd ) )
SYSCALL( truncate,               STATUS,  PROTO( STR path, OFF_T length ) )
SYSCALL( ftruncate,              STATUS,  PROTO( FD fd, OFF_T length ) )
SYSCALL( getdents,               STATUS,  PROTO( FD fd, struct linux_dirent *dirent, unsigned int count ) )
SYSCALL( getcwd,                 STATUS,  PROTO( char *buf, SIZE_T size ) )
SYSCALL( chdir,                  STATUS,  PROTO( STR filename ) )
SYSCALL( fchdir,                 STATUS,  PROTO( FD fd ) )
SYSCALL( rename,                 STATUS,  PROTO( STR oldname, STR newname ) )
SYSCALL( renameat,               STATUS,  PROTO( FD olddfd, STR oldname, FD newdfd, STR newname, unsigned int flags ) )

SYSCALL( mkdir,                  STATUS,  PROTO( STR pathname, umode_t mode ) )
SYSCALL( rmdir,                  STATUS,  PROTO( STR pathname ) )
SYSCALL( creat,                  STATUS,  PROTO( STR pathname, umode_t mode ) )
SYSCALL( link,                   STATUS,  PROTO( STR oldname, STR newname ) )
SYSCALL( unlink,                 STATUS,  PROTO( STR pathname ) )
SYSCALL( symlink,                STATUS,  PROTO( STR oldname, STR newname ) )
SYSCALL( symlinkat,              STATUS,  PROTO( STR oldname, FD newdfd, STR newname ) )
SYSCALL( readlink,               SSIZE_T, PROTO( STR path, char *buf, SIZE_T bufsiz ) )
SYSCALL( readlinkat,             SSIZE_T, PROTO( FD dfd, STR path, char *buf, SIZE_T bufsiz ) )
SYSCALL( chmod,                  STATUS,  PROTO( STR filename, umode_t mode ) )
SYSCALL( fchmod,                 STATUS,  PROTO( FD fd, umode_t mode ) )
SYSCALL( chown,                  STATUS,  PROTO( STR filename, uid_t user, gid_t group ) )
SYSCALL( fchown,                 STATUS,  PROTO( FD fd, uid_t user, gid_t group ) )
SYSCALL( lchown,                 STATUS,  PROTO( STR filename, uid_t user, gid_t group ) )
SYSCALL( umask,                  STATUS,  PROTO( int mask ) )
SYSCALL( gettimeofday,           STATUS,  PROTO( struct timeval *tv, struct timezone *tz ) )
SYSCALL( getrlimit,              STATUS,  PROTO( unsigned int resource, struct rlimit *rlim ) )
SYSCALL( getrusage,              STATUS,  PROTO( int who, struct rusage *ru ) )
SYSCALL( sysinfo,                STATUS,  PROTO( struct sysinfo *info ) )
SYSCALL( times,                  STATUS,  PROTO( struct tms *tbuf ) )
SYSCALL( ptrace,                 STATUS,  PROTO( enum __ptrace_request request, PID_T pid, PTR addr, PTR data ) )
SYSCALL( getuid,                 STATUS,  PROTO( ) )
SYSCALL( syslog,                 STATUS,  PROTO( int type, char *buf, int len ) )
SYSCALL( getgid,                 STATUS,  PROTO( ) )
SYSCALL( setuid,                 STATUS,  PROTO( uid_t uid ) )
SYSCALL( setgid,                 STATUS,  PROTO( gid_t gid ) )
SYSCALL( geteuid,                STATUS,  PROTO( ) )
SYSCALL( getegid,                STATUS,  PROTO( ) )
SYSCALL( setpgid,                STATUS,  PROTO( PID_T pid, PID_T pgid ) )
SYSCALL( getppid,                STATUS,  PROTO( ) )
SYSCALL( getpgrp,                STATUS,  PROTO( ) )
SYSCALL( setsid,                 STATUS,  PROTO( ) )
SYSCALL( setreuid,               STATUS,  PROTO( uid_t ruid, uid_t euid ) )
SYSCALL( setregid,               STATUS,  PROTO( gid_t rgid, gid_t egid ) )
SYSCALL( getgroups,              STATUS,  PROTO( int gidsetsize, gid_t *grouplist ) )
SYSCALL( setgroups,              STATUS,  PROTO( int gidsetsize, gid_t *grouplist ) )
SYSCALL( setresuid,              STATUS,  PROTO( uid_t ruid, uid_t euid, uid_t suid ) )
SYSCALL( getresuid,              STATUS,  PROTO( uid_t *ruid, uid_t *euid, uid_t *suid ) )
SYSCALL( setresgid,              STATUS,  PROTO( gid_t rgid, gid_t egid, gid_t sgid ) )
SYSCALL( getresgid,              STATUS,  PROTO( gid_t *rgid, gid_t *egid, gid_t *sgid ) )
SYSCALL( getpgid,                STATUS,  PROTO( PID_T pid ) )
SYSCALL( setfsuid,               STATUS,  PROTO( uid_t uid ) )
SYSCALL( setfsgid,               STATUS,  PROTO( gid_t gid ) )
SYSCALL( getsid,                 STATUS,  PROTO( PID_T pid ) )
SYSCALL( capget,                 STATUS,  PROTO( cap_user_header_t header, cap_user_data_t dataptr ) )
SYSCALL( capset,                 STATUS,  PROTO( cap_user_header_t header, const cap_user_data_t data ) )
SYSCALL( rt_sigpending,          STATUS,  PROTO( sigset_t *set, SIZE_T sigsetsize ) )
SYSCALL( rt_sigtimedwait,        STATUS,  PROTO( const sigset_t *uthese, siginfo_t *uinfo, const struct timespec *uts, SIZE_T sigsetsize ) )
SYSCALL( rt_sigqueueinfo,        STATUS,  PROTO( PID_T pid, int sig, siginfo_t *uinfo ) )
SYSCALL( rt_sigsuspend,          STATUS,  PROTO( sigset_t *unewset, SIZE_T sigsetsize ) )
SYSCALL( sigaltstack,            STATUS,  PROTO( const struct sigaltstack *uss, struct sigaltstack *uoss ) )

SYSCALL( utime,                  STATUS,  PROTO( STR filename, struct utimbuf *times ) )
SYSCALL( utimes,                 STATUS,  PROTO( STR filename, const struct timeval utimes[2] ) )

SYSCALL( mknod,                  STATUS,  PROTO( STR filename, umode_t mode, unsigned dev ) )
SYSCALL( uselib,                 STATUS,  PROTO( STR library ) )
SYSCALL( personality,            STATUS,  PROTO( unsigned int personality ) )
SYSCALL( ustat,                  STATUS,  PROTO( unsigned dev, struct ustat *ubuf ) )
SYSCALL( statfs,                 STATUS,  PROTO( STR path, struct statfs *buf ) )
SYSCALL( fstatfs,                STATUS,  PROTO( FD fd, struct statfs *buf ) )
SYSCALL( sysfs,                  STATUS,  PROTO( int option, unsigned long arg, unsigned long arg2)
SYSCALL( getpriority,            STATUS,  PROTO( int which, int who ) )
SYSCALL( setpriority,            STATUS,  PROTO( int which, int who, int niceval ) )

// sheduling
SYSCALL( nanosleep,              STATUS,  PROTO( struct timespec *rqtp, struct timespec *rmtp ) )
SYSCALL( clock_nanosleep,        STATUS,  PROTO( clockid_t which_clock, int flags, const struct timespec *rqtp, struct timespec *rmtp ) )
SYSCALL( sched_yield,            STATUS,  PROTO( ) )
SYSCALL( sched_setparam,         STATUS,  PROTO( PID_T pid, struct sched_param *param ) )
SYSCALL( sched_getparam,         STATUS,  PROTO( PID_T pid, struct sched_param *param ) )
SYSCALL( sched_setscheduler,     STATUS,  PROTO( PID_T pid, int policy, struct sched_param *param ) )
SYSCALL( sched_getscheduler,     STATUS,  PROTO( PID_T pid ) )
SYSCALL( sched_get_priority_max, STATUS,  PROTO( int policy ) )
SYSCALL( sched_get_priority_min, STATUS,  PROTO( int policy ) )
SYSCALL( sched_rr_get_interval,  STATUS,  PROTO( PID_T pid, struct timespec *interval ) )
SYSCALL( sched_setaffinity,      STATUS,  PROTO( PID_T pid, unsigned int len, unsigned long *user_mask_ptr ) )
SYSCALL( sched_getaffinity,      STATUS,  PROTO( PID_T pid, unsigned int len, unsigned long *user_mask_ptr ) )
SYSCALL( sched_setattr,          STATUS,  PROTO( PID_T pid, struct sched_attr *attr, unsigned int flags ) )
SYSCALL( sched_getattr,          STATUS,  PROTO( PID_T pid, struct sched_attr *attr, unsigned int size, unsigned int flags ) )


SYSCALL( vhangup,                STATUS,  PROTO( ) )
//SYSCALL( modify_ldt,             STATUS,  PROTO( ?, ?, ?, ?, ?, ? ) )
SYSCALL( pivot_root,             STATUS,  PROTO( STR new_root, STR put_old ) )
//SYSCALL( _sysctl,                STATUS,  PROTO( ?, ?, ?, ?, ?, ? ) )
SYSCALL( prctl,                  STATUS,  PROTO( int option, unsigned long arg, unsigned long arg, unsigned long arg, unsigned long arg5)
//SYSCALL( arch_prctl,             STATUS,  PROTO( ?, ?, ?, ?, ?, ? ) )
SYSCALL( adjtimex,               STATUS,  PROTO( struct __kernel_timex *txc_p ) )
SYSCALL( setrlimit,              STATUS,  PROTO( unsigned int resource, struct rlimit *rlim ) )
SYSCALL( chroot,                 STATUS,  PROTO( STR filename ) )
SYSCALL( sync,                   STATUS,  PROTO( ) )
SYSCALL( acct,                   STATUS,  PROTO( STR name ) )
SYSCALL( settimeofday,           STATUS,  PROTO( struct timeval *tv, struct timezone *tz ) )
SYSCALL( mount,                  STATUS,  PROTO( char *dev_name, char *dir_name, char *type, unsigned long flags, void *data ) )
SYSCALL( umount,                 STATUS,  PROTO( STR target, int flags ) )
SYSCALL( swapon,                 STATUS,  PROTO( STR specialfile, int swap_flags ) )
SYSCALL( swapoff,                STATUS,  PROTO( STR specialfile ) )
SYSCALL( reboot,                 STATUS,  PROTO( int magic, int magic, unsigned int cmd, void *arg ) )
SYSCALL( sethostname,            STATUS,  PROTO( STR name, SIZE_T len ) )
SYSCALL( setdomainname,          STATUS,  PROTO( STR name, SIZE_T len ) )

// SYSCALL( iopl,                   STATUS,  PROTO( ?, ?, ?, ?, ?, ? ) )
//SYSCALL( create_module,          STATUS,  PROTO( STR name, SIZE_T size ) )
//SYSCALL( init_module,            STATUS,  PROTO( void *umod, unsigned long len, STR uargs ) )
//SYSCALL( delete_module,          STATUS,  PROTO( STR name_user, unsigned int flags ) )

SYSCALL( quotactl,               STATUS,  PROTO( unsigned int cmd, STR special, qid_t id, void *addr ) )
SYSCALL( nfsservctl,             STATUS,  PROTO( int cmd, struct nfsctl_arg *argp, union nfsctl_res *resp ) )

// STREAMS
//SYSCALL( getpmsg,                STATUS,  PROTO( int fildes, struct strbuf *ctlptr, struct strbuf *dataptr, int *bandp, int *flagsp ) )
//SYSCALL( putpmsg,                STATUS,  PROTO( int fildes, const struct strbuf *ctlptr, const struct strbuf *dataptr, int band, int flags ) )

SYSCALL( security,               STATUS,  PROTO( ?, ?, ?, ?, ?, ? ) )
SYSCALL( gettid,                 STATUS,  PROTO( ) )
SYSCALL( readahead,              STATUS,  PROTO( FD fd, OFF_T offset, SIZE_T count ) )
SYSCALL( setxattr,               STATUS,  PROTO( STR path, STR name, const void *value, SIZE_T size, int flags ) )
SYSCALL( lsetxattr,              STATUS,  PROTO( STR path, STR name, const void *value, SIZE_T size, int flags ) )
SYSCALL( fsetxattr,              STATUS,  PROTO( FD fd, STR name, const void *value, SIZE_T size, int flags ) )
SYSCALL( getxattr,               STATUS,  PROTO( STR path, STR name, void *value, SIZE_T size ) )
SYSCALL( lgetxattr,              STATUS,  PROTO( STR path, STR name, void *value, SIZE_T size ) )
SYSCALL( fgetxattr,              STATUS,  PROTO( FD fd, STR name, void *value, SIZE_T size ) )
SYSCALL( listxattr,              STATUS,  PROTO( STR path, char *list, SIZE_T size ) )
SYSCALL( llistxattr,             STATUS,  PROTO( STR path, char *list, SIZE_T size ) )
SYSCALL( flistxattr,             STATUS,  PROTO( FD fd, char *list, SIZE_T size ) )
SYSCALL( removexattr,            STATUS,  PROTO( STR path, STR name ) )
SYSCALL( lremovexattr,           STATUS,  PROTO( STR path, STR name ) )
SYSCALL( fremovexattr,           STATUS,  PROTO( FD fd, STR name ) )
SYSCALL( time,                   STATUS,  PROTO( time_t *tloc ) )
SYSCALL( futex,                  STATUS,  PROTO( u32 *uaddr, int op, u32 val, struct timespec *utime, u32 *uaddr, u32 val3 ) )
//SYSCALL( set_thread_area,        STATUS,  PROTO( ?, ?, ?, ?, ?, ? ) )
SYSCALL( io_setup,               STATUS,  PROTO( unsigned nr_reqs, aio_context_t *ctx ) )
SYSCALL( io_destroy,             STATUS,  PROTO( aio_context_t ctx ) )
SYSCALL( io_getevents,           STATUS,  PROTO( aio_context_t ctx_id, long min_nr, long nr, struct io_event *events, struct timespec *timeout ) )
SYSCALL( io_submit,              STATUS,  PROTO( aio_context_t, long, struct iocb * * ) )
SYSCALL( io_cancel,              STATUS,  PROTO( aio_context_t ctx_id, struct iocb *iocb, struct io_event *result ) )
//SYSCALL( get_thread_area,        STATUS,  PROTO( ?, ?, ?, ?, ?, ? ) )
SYSCALL( lookup_dcookie,         STATUS,  PROTO( u64 cookie, char *buf, SIZE_T len ) )

// epoll
SYSCALL( epoll_create,           FD,      PROTO( int size ) )
SYSCALL( epoll_create1,          FD,      PROTO( int flags ) )
SYSCALL( epoll_wait,             STATUS,  PROTO( FD epfd, struct epoll_event *events, int maxevents, int timeout ) )
SYSCALL( epoll_pwait,            STATUS,  PROTO( FD epfd, struct epoll_event *events, int maxevents, int timeout, const sigset_t *sigmask, SIZE_T sigsetsize ) )
SYSCALL( epoll_pwait2,           STATUS,  PROTO( FD epfd, struct epoll_event *events, int maxevents, const struct timespec *timeout, const sigset_t *sigmask ) )
SYSCALL( epoll_ctl,              STATUS,  PROTO( FD epfd, int op, FD fd, struct epoll_event *event ) )


SYSCALL( getdents,               STATUS,  PROTO( FD fd, struct linux_dirent64 *dirent, unsigned int count ) )
SYSCALL( set_tid_address,        STATUS,  PROTO( int *tidptr ) )
SYSCALL( restart_syscall,        STATUS,  PROTO( ) )
SYSCALL( semtimedop,             STATUS,  PROTO( int semid, struct sembuf *sops, unsigned nsops, const struct timespec *timeout ) )
SYSCALL( fadvise,                STATUS,  PROTO( FD fd, OFF_T offset, SIZE_T len, int advice ) )
SYSCALL( timer_create,           STATUS,  PROTO( clockid_t which_clock, struct sigevent *timer_event_spec, timer_t * created_timer_id ) )
SYSCALL( timer_settime,          STATUS,  PROTO( timer_t timer_id, int flags, PCKERNEL_ITIMERSPEC new_setting, PKERNEL_ITIMERSPEC old_setting ) )
SYSCALL( timer_gettime,          STATUS,  PROTO( timer_t timer_id, PKERNEL_ITIMERSPEC setting ) )
SYSCALL( timer_getoverrun,       STATUS,  PROTO( timer_t timer_id ) )
SYSCALL( timer_delete,           STATUS,  PROTO( timer_t timer_id ) )
SYSCALL( clock_settime,          STATUS,  PROTO( clockid_t which_clock, const struct timespec *tp ) )
SYSCALL( clock_gettime,          STATUS,  PROTO( clockid_t which_clock, struct timespec *tp ) )
SYSCALL( clock_getres,           STATUS,  PROTO( clockid_t which_clock, struct timespec *tp ) )
SYSCALL( clock_adjtime,          STATUS,  PROTO( clockid_t which_clock, struct __kernel_timex *tx ) )

// SYSCALL( vserver,                STATUS,  PROTO( ?, ?, ?, ?, ?, ? ) )
SYSCALL( set_mempolicy,          STATUS,  PROTO( int mode, const unsigned long *nmask, unsigned long maxnode ) )
SYSCALL( get_mempolicy,          STATUS,  PROTO( int *policy, unsigned long *nmask, unsigned long maxnode, PTR addr, unsigned long flags ) )
SYSCALL( mq_open,                STATUS,  PROTO( STR name, int oflag, umode_t mode, struct mq_attr *attr ) )
SYSCALL( mq_unlink,              STATUS,  PROTO( STR name ) )
SYSCALL( mq_timedsend,           STATUS,  PROTO( mqd_t mqdes, STR msg_ptr, SIZE_T msg_len, unsigned int msg_prio, const struct timespec *abs_timeout ) )
SYSCALL( mq_timedreceive,        STATUS,  PROTO( mqd_t mqdes, char *msg_ptr, SIZE_T msg_len, unsigned int *msg_prio, const struct timespec *abs_timeout ) )
SYSCALL( mq_notify,              STATUS,  PROTO( mqd_t mqdes, const struct sigevent *notification ) )
SYSCALL( mq_getsetattr,          STATUS,  PROTO( mqd_t mqdes, const struct mq_attr *mqstat, struct mq_attr *omqstat ) )


SYSCALL( waitid,                 STATUS,  PROTO( int which, PID_T pid, struct siginfo *infop, int options, struct rusage *ru ) )
SYSCALL( add_key,                STATUS,  PROTO( STR _type, STR _description, const void *_payload, SIZE_T plen, key_serial_t destringid ) )
SYSCALL( request_key,            STATUS,  PROTO( STR _type, STR _description, STR _callout_info, key_serial_t destringid ) )
SYSCALL( keyctl,                 STATUS,  PROTO( int cmd, unsigned long arg, unsigned long arg, unsigned long arg, unsigned long arg5)
SYSCALL( ioprio_set,             STATUS,  PROTO( int which, int who, int ioprio ) )
SYSCALL( ioprio_get,             STATUS,  PROTO( int which, int who ) )
SYSCALL( inotify_init,           STATUS,  PROTO( ) )
SYSCALL( inotify_init1,          STATUS,  PROTO( int flags ) )
SYSCALL( inotify_add_watch,      STATUS,  PROTO( FD fd, STR path, u32 mask ) )
SYSCALL( inotify_rm_watch,       STATUS,  PROTO( FD fd, int wd ) )
SYSCALL( migrate_pages,          STATUS,  PROTO( PID_T pid, unsigned long maxnode, const unsigned long *from, const unsigned long *to ) )
SYSCALL( mkdirat,                STATUS,  PROTO( FD dfd, STR  pathname, umode_t mode ) )
SYSCALL( mknodat,                STATUS,  PROTO( FD dfd, STR  filename, umode_t mode, unsigned dev ) )
SYSCALL( fchownat,               STATUS,  PROTO( FD dfd, STR filename, uid_t user, gid_t group, int flag ) )
SYSCALL( futimesat,              STATUS,  PROTO( FD dfd, STR filename, struct timeval *utimes ) )
SYSCALL( unlinkat,               STATUS,  PROTO( FD dfd, STR  pathname, int flag ) )
SYSCALL( renameat,               STATUS,  PROTO( FD olddfd, STR  oldname, FD newdfd, STR  newname ) )
SYSCALL( linkat,                 STATUS,  PROTO( FD olddfd, STR oldname, FD newdfd, STR newname, int flags ) )
SYSCALL( fchmodat,               STATUS,  PROTO( FD dfd, STR  filename, umode_t mode ) )
SYSCALL( faccessat,              STATUS,  PROTO( FD dfd, STR filename, int mode ) )

SYSCALL( pselect,                SSIZE_T, PROTO( int, fd_set *, fd_set *, fd_set *, struct timespec *, void * ) )
SYSCALL( unshare,                STATUS,  PROTO( unsigned long unshare_flags ) )
SYSCALL( set_robust_list,        STATUS,  PROTO( struct robust_list_head *head, SIZE_T len ) )
SYSCALL( get_robust_list,        STATUS,  PROTO( int pid, struct robust_list_head * *head_ptr, SIZE_T *len_ptr ) )
SYSCALL( splice,                 STATUS,  PROTO( FD fd_in, OFF_T *off_in, FD fd_out, OFF_T *off_out, SIZE_T len, unsigned int flags ) )
SYSCALL( tee,                    STATUS,  PROTO( FD fdin, FD fdout, SIZE_T len, unsigned int flags ) )
SYSCALL( sync_file_range,        STATUS,  PROTO( FD fd, OFF_T offset, OFF_T nbytes, unsigned int flags ) )
SYSCALL( vmsplice,               STATUS,  PROTO( FD fd, PCIOVEC iov, unsigned long nr_segs, unsigned int flags ) )
SYSCALL( move_pages,             STATUS,  PROTO( PID_T pid, unsigned long nr_pages, const void * *pages, const int *nodes, int *status, int flags ) )
SYSCALL( utimensat,              STATUS,  PROTO( FD dfd, STR filename, struct timespec *utimes, int flags ) )
SYSCALL( signalfd,               STATUS,  PROTO( FD ufd, sigset_t *user_mask, SIZE_T sizemask ) )
SYSCALL( timerfd_create,         STATUS,  PROTO( int clockid, int flags ) )
SYSCALL( eventfd,                STATUS,  PROTO( unsigned int count ) )
SYSCALL( fallocate,              STATUS,  PROTO( FD fd, int mode, OFF_T offset, OFF_T len ) )
SYSCALL( timerfd_settime,        STATUS,  PROTO( FD ufd, int flags, PCKERNEL_ITIMERSPEC utmr, PKERNEL_ITIMERSPEC otmr ) )
SYSCALL( timerfd_gettime,        STATUS,  PROTO( FD ufd, PKERNEL_ITIMERSPEC otmr ) )
SYSCALL( signalfd,               STATUS,  PROTO( FD ufd, sigset_t *user_mask, SIZE_T sizemask, int flags ) )
SYSCALL( eventfd,                STATUS,  PROTO( unsigned int count, int flags ) )
SYSCALL( pipe,                   SSIZE_T, PROTO( int *fildes, int flags ) )


SYSCALL( rt_tgsigqueueinfo,      STATUS,  PROTO( PID_T tgid, PID_T pid, int sig, siginfo_t *uinfo ) )
SYSCALL( perf_event_open,        STATUS,  PROTO( struct perf_event_attr *attr_uptr, PID_T pid, int cpu, int group_fd, unsigned long flags ) )
SYSCALL( fanotify_init,          FD,      PROTO( unsigned int flags, unsigned int event_f_flags ) )
SYSCALL( fanotify_mark,          STATUS,  PROTO( FD fanotify_fd, unsigned int flags, u64 mask, FD fd, STR pathname ) )
SYSCALL( prlimit,                STATUS,  PROTO( PID_T pid, unsigned int resource, const struct rlimit64 *new_rlim, struct rlimit64 *old_rlim ) )
SYSCALL( name_to_handle_at,      STATUS,  PROTO( FD dfd, STR name, struct file_handle *handle, int *mnt_id, int flag ) )
SYSCALL( open_by_handle_at,      STATUS,  PROTO( FD mountdirfd, struct file_handle *handle, int flags ) )
SYSCALL( syncfs,                 STATUS,  PROTO( FD fd ) )
SYSCALL( setns,                  STATUS,  PROTO( FD fd, int nstype ) )
SYSCALL( getcpu,                 STATUS,  PROTO( unsigned *cpu, unsigned *node, struct getcpu_cache *cache ) )
SYSCALL( kcmp,                   STATUS,  PROTO( PID_T pid,  PID_T pid,  int type, unsigned long idx, unsigned long idx2)
SYSCALL( finit_module,           STATUS,  PROTO( FD fd, STR uargs, int flags ) )

SYSCALL( process_vm_readv,       SSIZE_T, PROTO( PID_T pid, PCIOVEC lvec, SIZE_T liovcnt, PCIOVEC rvec, SIZE_T riovcnt, unsigned long flags ) )
SYSCALL( process_vm_writev,      SSIZE_T, PROTO( PID_T pid, PCIOVEC lvec, SIZE_T liovcnt, PCIOVEC rvec, SIZE_T riovcnt, unsigned long flags ) )

SYSCALL( seccomp,                STATUS,  PROTO( unsigned int op, unsigned int flags, void *uargs ) )
SYSCALL( memfd_create,           STATUS,  PROTO( STR uname_ptr, unsigned int flags ) )
SYSCALL( kexec_load,             STATUS,  PROTO( unsigned long entry, unsigned long nr_segments, struct kexec_segment *segments, unsigned long flags ) )
SYSCALL( kexec_file_load,        STATUS,  PROTO( int kernel_fd, int initrd_fd, unsigned long cmdline_len, STR cmdline_ptr, unsigned long flags ) )
SYSCALL( bpf,                    STATUS,  PROTO( int cmd, union bpf_attr *attr, unsigned int size ) )
SYSCALL( userfaultfd,            STATUS,  PROTO( int flags ) )
SYSCALL( membarrier,             STATUS,  PROTO( int cmd, int flags ) )
SYSCALL( statx,                  STATUS,  PROTO( FD dfd, STR path, unsigned flags, unsigned mask, struct statx *buffer ) )
SYSCALL( copy_file_range,        STATUS,  PROTO( FD fd_in, OFF_T *off_in, FD fd_out, OFF_T *off_out, SIZE_T len, unsigned int flags ) )

// protection keys (x86)
SYSCALL( pkey_mprotect,          STATUS,  PROTO( PTR start, SIZE_T len, unsigned long prot, int pkey ) )
SYSCALL( pkey_alloc,             STATUS,  PROTO( unsigned long flags, unsigned long init_val ) )
SYSCALL( pkey_free,              STATUS,  PROTO( int pkey ) )

// misc
SYSCALL( getrandom,              SSIZE_T, PROTO( PTR buf, SIZE_T count, unsigned int flags ) )


/*
333    common    io_pgetevents        sys_io_pgetevents
334    common    rseq            sys_rseq
424    common    pidfd_send_signal    sys_pidfd_send_signal
425    common    io_uring_setup        sys_io_uring_setup
426    common    io_uring_enter        sys_io_uring_enter
427    common    io_uring_register    sys_io_uring_register
428    common    open_tree        sys_open_tree
429    common    move_mount        sys_move_mount
430    common    fsopen            sys_fsopen
431    common    fsconfig        sys_fsconfig
432    common    fsmount            sys_fsmount
433    common    fspick            sys_fspick
434    common    pidfd_open        sys_pidfd_open
435    common    clone3            sys_clone3
436    common    close_range        sys_close_range
437    common    openat2            sys_openat2
438    common    pidfd_getfd        sys_pidfd_getfd
439    common    faccessat2        sys_faccessat2
440    common    process_madvise        sys_process_madvise
441    common    epoll_pwait2        sys_epoll_pwait2
442    common    mount_setattr        sys_mount_setattr
443    common    quotactl_fd        sys_quotactl_fd
444    common    landlock_create_ruleset    sys_landlock_create_ruleset
445    common    landlock_add_rule    sys_landlock_add_rule
446    common    landlock_restrict_self    sys_landlock_restrict_self
447    common    memfd_secret        sys_memfd_secret
448    common    process_mrelease    sys_process_mrelease
449    common    futex_waitv        sys_futex_waitv
450    common    set_mempolicy_home_node    sys_set_mempolicy_home_node
*/
