
/*
X(name, RETTYPE, PARAM(TYPE, PNAME), PARAM(TYPE, PNAME), PARAM(TYPE, PNAME))
*/

// prototype
#define PARAM(type, name) type name
#define X(name, rtype, __VA_ARGS__) rtype syscall_name(__VA_ARGS__);

// call
#define PARAM(type, name) (type) name
#define X(name, rtype, __VA_ARGS__) ret = syscall_name(__VA_ARGS__);


X(read, unsigned int fd, char *buf, size_t count, -, -, -)
X(write, unsigned int fd, const char *buf, size_t count, -, -, -)
X(open, const char *filename, int flags, umode_t mode, -, -, -)
X(close, unsigned int fd, -, -, -, -, -)
X(stat, const char *filename, struct __old_kernel_stat *statbuf, -, -, -, -)
X(fstat, unsigned int fd, struct __old_kernel_stat *statbuf, -, -, -, -)
X(lstat, const char *filename, struct __old_kernel_stat *statbuf, -, -, -, -)
X(poll, struct pollfd *ufds, unsigned int nfds, int timeout, -, -, -)
X(lseek, unsigned int fd, off_t offset, unsigned int whence, -, -, -)
X(mmap, ?, ?, ?, ?, ?, ?)
X(mprotect, unsigned long start, size_t len, unsigned long prot, -, -, -)
X(munmap, unsigned long addr, size_t len, -, -, -, -)

#if 0

X(brk, unsigned long brk, -, -, -, -, -)
X(rt_sigaction, int, const struct sigaction *, struct sigaction *, size_t, -, -)
X(rt_sigprocmask, int how, sigset_t *set, sigset_t *oset, size_t sigsetsize, -, -)
X(rt_sigreturn, ?, ?, ?, ?, ?, ?)
X(ioctl, unsigned int fd, unsigned int cmd, unsigned long arg, -, -, -)
X(pread64, unsigned int fd, char *buf, size_t count, loff_t pos, -, -)
X(pwrite64, unsigned int fd, const char *buf, size_t count, loff_t pos, -, -)
X(readv, unsigned long fd, const struct iovec *vec, unsigned long vlen, -, -, -)
X(writev, unsigned long fd, const struct iovec *vec, unsigned long vlen, -, -, -)
X(access, const char *filename, int mode, -, -, -, -)
X(pipe, int *fildes, -, -, -, -, -)
X(select, int n, fd_set *inp, fd_set *outp, fd_set *exp, struct timeval *tvp, -)
X(sched_yield, -, -, -, -, -, -)
X(mremap, unsigned long addr, unsigned long old_len, unsigned long new_len, unsigned long flags, unsigned long new_addr, -)
X(msync, unsigned long start, size_t len, int flags, -, -, -)
X(mincore, unsigned long start, size_t len, unsigned char * vec, -, -, -)
X(madvise, unsigned long start, size_t len, int behavior, -, -, -)
X(shmget, key_t key, size_t size, int flag, -, -, -)
X(shmat, int shmid, char *shmaddr, int shmflg, -, -, -)
X(shmctl, int shmid, int cmd, struct shmid_ds *buf, -, -, -)
X(dup, unsigned int fildes, -, -, -, -, -)
X(dup2, unsigned int oldfd, unsigned int newfd, -, -, -, -)
X(pause, -, -, -, -, -, -)
X(nanosleep, struct __kernel_timespec *rqtp, struct __kernel_timespec *rmtp, -, -, -, -)
X(getitimer, int which, struct itimerval *value, -, -, -, -)
X(alarm, unsigned int seconds, -, -, -, -, -)
X(setitimer, int which, struct itimerval *value, struct itimerval *ovalue, -, -, -)
X(getpid, -, -, -, -, -, -)
X(sendfile, int out_fd, int in_fd, off_t *offset, size_t count, -, -)
X(socket, int, int, int, -, -, -)
X(connect, int, struct sockaddr *, int, -, -, -)
X(accept, int, struct sockaddr *, int *, -, -, -)
X(sendto, int, void *, size_t, unsigned, struct sockaddr *, int)
X(recvfrom, int, void *, size_t, unsigned, struct sockaddr *, int *)
X(sendmsg, int fd, struct user_msghdr *msg, unsigned flags, -, -, -)
X(recvmsg, int fd, struct user_msghdr *msg, unsigned flags, -, -, -)
X(shutdown, int, int, -, -, -, -)
X(bind, int, struct sockaddr *, int, -, -, -)
X(listen, int, int, -, -, -, -)
X(getsockname, int, struct sockaddr *, int *, -, -, -)
X(getpeername, int, struct sockaddr *, int *, -, -, -)
X(socketpair, int, int, int, int *, -, -)
X(setsockopt, int fd, int level, int optname, char *optval, int optlen, -)
X(getsockopt, int fd, int level, int optname, char *optval, int *optlen, -)
X(clone, unsigned long, unsigned long, int *, int *, unsigned long, -)
X(fork, -, -, -, -, -, -)
X(vfork, -, -, -, -, -, -)
X(execve, const char *filename, const char *const *argv, const char *const *envp, -, -, -)
X(exit, int error_code, -, -, -, -, -)
X(wait4, pid_t pid, int *stat_addr, int options, struct rusage *ru, -, -)
X(kill, pid_t pid, int sig, -, -, -, -)
X(uname, struct old_utsname *, -, -, -, -, -)
X(semget, key_t key, int nsems, int semflg, -, -, -)
X(semop, int semid, struct sembuf *sops, unsigned nsops, -, -, -)
X(semctl, int semid, int semnum, int cmd, unsigned long arg, -, -)
X(shmdt, char *shmaddr, -, -, -, -, -)
X(msgget, key_t key, int msgflg, -, -, -, -)
X(msgsnd, int msqid, struct msgbuf *msgp, size_t msgsz, int msgflg, -, -)
X(msgrcv, int msqid, struct msgbuf *msgp, size_t msgsz, long msgtyp, int msgflg, -)
X(msgctl, int msqid, int cmd, struct msqid_ds *buf, -, -, -)
X(fcntl, unsigned int fd, unsigned int cmd, unsigned long arg, -, -, -)
X(flock, unsigned int fd, unsigned int cmd, -, -, -, -)
X(fsync, unsigned int fd, -, -, -, -, -)
X(fdatasync, unsigned int fd, -, -, -, -, -)
X(truncate, const char *path, long length, -, -, -, -)
X(ftruncate, unsigned int fd, unsigned long length, -, -, -, -)
X(getdents, unsigned int fd, struct linux_dirent *dirent, unsigned int count, -, -, -)
X(getcwd, char *buf, unsigned long size, -, -, -, -)
X(chdir, const char *filename, -, -, -, -, -)
X(fchdir, unsigned int fd, -, -, -, -, -)
X(rename, const char *oldname, const char *newname, -, -, -, -)
X(mkdir, const char *pathname, umode_t mode, -, -, -, -)
X(rmdir, const char *pathname, -, -, -, -, -)
X(creat, const char *pathname, umode_t mode, -, -, -, -)
X(link, const char *oldname, const char *newname, -, -, -, -)
X(unlink, const char *pathname, -, -, -, -, -)
X(symlink, const char *old, const char *new, -, -, -, -)
X(readlink, const char *path, char *buf, int bufsiz, -, -, -)
X(chmod, const char *filename, umode_t mode, -, -, -, -)
X(fchmod, unsigned int fd, umode_t mode, -, -, -, -)
X(chown, const char *filename, uid_t user, gid_t group, -, -, -)
X(fchown, unsigned int fd, uid_t user, gid_t group, -, -, -)
X(lchown, const char *filename, uid_t user, gid_t group, -, -, -)
X(umask, int mask, -, -, -, -, -)
X(gettimeofday, struct timeval *tv, struct timezone *tz, -, -, -, -)
X(getrlimit, unsigned int resource, struct rlimit *rlim, -, -, -, -)
X(getrusage, int who, struct rusage *ru, -, -, -, -)
X(sysinfo, struct sysinfo *info, -, -, -, -, -)
X(times, struct tms *tbuf, -, -, -, -, -)
X(ptrace, long request, long pid, unsigned long addr, unsigned long data, -, -)
X(getuid, -, -, -, -, -, -)
X(syslog, int type, char *buf, int len, -, -, -)
X(getgid, -, -, -, -, -, -)
X(setuid, uid_t uid, -, -, -, -, -)
X(setgid, gid_t gid, -, -, -, -, -)
X(geteuid, -, -, -, -, -, -)
X(getegid, -, -, -, -, -, -)
X(setpgid, pid_t pid, pid_t pgid, -, -, -, -)
X(getppid, -, -, -, -, -, -)
X(getpgrp, -, -, -, -, -, -)
X(setsid, -, -, -, -, -, -)
X(setreuid, uid_t ruid, uid_t euid, -, -, -, -)
X(setregid, gid_t rgid, gid_t egid, -, -, -, -)
X(getgroups, int gidsetsize, gid_t *grouplist, -, -, -, -)
X(setgroups, int gidsetsize, gid_t *grouplist, -, -, -, -)
X(setresuid, uid_t ruid, uid_t euid, uid_t suid, -, -, -)
X(getresuid, uid_t *ruid, uid_t *euid, uid_t *suid, -, -, -)
X(setresgid, gid_t rgid, gid_t egid, gid_t sgid, -, -, -)
X(getresgid, gid_t *rgid, gid_t *egid, gid_t *sgid, -, -, -)
X(getpgid, pid_t pid, -, -, -, -, -)
X(setfsuid, uid_t uid, -, -, -, -, -)
X(setfsgid, gid_t gid, -, -, -, -, -)
X(getsid, pid_t pid, -, -, -, -, -)
X(capget, cap_user_header_t header, cap_user_data_t dataptr, -, -, -, -)
X(capset, cap_user_header_t header, const cap_user_data_t data, -, -, -, -)
X(rt_sigpending, sigset_t *set, size_t sigsetsize, -, -, -, -)
X(rt_sigtimedwait, const sigset_t *uthese, siginfo_t *uinfo, const struct __kernel_timespec *uts, size_t sigsetsize, -, -)
X(rt_sigqueueinfo, pid_t pid, int sig, siginfo_t *uinfo, -, -, -)
X(rt_sigsuspend, sigset_t *unewset, size_t sigsetsize, -, -, -, -)
X(sigaltstack, const struct sigaltstack *uss, struct sigaltstack *uoss, -, -, -, -)
X(utime, char *filename, struct utimbuf *times, -, -, -, -)
X(mknod, const char *filename, umode_t mode, unsigned dev, -, -, -)
X(uselib, const char *library, -, -, -, -, -)
X(personality, unsigned int personality, -, -, -, -, -)
X(ustat, unsigned dev, struct ustat *ubuf, -, -, -, -)
X(statfs, const char * path, struct statfs *buf, -, -, -, -)
X(fstatfs, unsigned int fd, struct statfs *buf, -, -, -, -)
X(sysfs, int option, unsigned long arg1, unsigned long arg2, -, -, -)
X(getpriority, int which, int who, -, -, -, -)
X(setpriority, int which, int who, int niceval, -, -, -)
X(sched_setparam, pid_t pid, struct sched_param *param, -, -, -, -)
X(sched_getparam, pid_t pid, struct sched_param *param, -, -, -, -)
X(sched_setscheduler, pid_t pid, int policy, struct sched_param *param, -, -, -)
X(sched_getscheduler, pid_t pid, -, -, -, -, -)
X(sched_get_priority_max, int policy, -, -, -, -, -)
X(sched_get_priority_min, int policy, -, -, -, -, -)
X(sched_rr_get_interval, pid_t pid, struct __kernel_timespec *interval, -, -, -, -)
X(mlock, unsigned long start, size_t len, -, -, -, -)
X(munlock, unsigned long start, size_t len, -, -, -, -)
X(mlockall, int flags, -, -, -, -, -)
X(munlockall, -, -, -, -, -, -)
X(vhangup, -, -, -, -, -, -)
X(modify_ldt, ?, ?, ?, ?, ?, ?)
X(pivot_root, const char *new_root, const char *put_old, -, -, -, -)
X(_sysctl, ?, ?, ?, ?, ?, ?)
X(prctl, int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5, -)
X(arch_prctl, ?, ?, ?, ?, ?, ?)
X(adjtimex, struct __kernel_timex *txc_p, -, -, -, -, -)
X(setrlimit, unsigned int resource, struct rlimit *rlim, -, -, -, -)
X(chroot, const char *filename, -, -, -, -, -)
X(sync, -, -, -, -, -, -)
X(acct, const char *name, -, -, -, -, -)
X(settimeofday, struct timeval *tv, struct timezone *tz, -, -, -, -)
X(mount, char *dev_name, char *dir_name, char *type, unsigned long flags, void *data, -)
X(umount2, ?, ?, ?, ?, ?, ?)
X(swapon, const char *specialfile, int swap_flags, -, -, -, -)
X(swapoff, const char *specialfile, -, -, -, -, -)
X(reboot, int magic1, int magic2, unsigned int cmd, void *arg, -, -)
X(sethostname, char *name, int len, -, -, -, -)
X(setdomainname, char *name, int len, -, -, -, -)
X(iopl, ?, ?, ?, ?, ?, ?)
X(ioperm, unsigned long from, unsigned long num, int on, -, -, -)
X(create_module, ?, ?, ?, ?, ?, ?)
X(init_module, void *umod, unsigned long len, const char *uargs, -, -, -)
X(delete_module, const char *name_user, unsigned int flags, -, -, -, -)
X(get_kernel_syms, ?, ?, ?, ?, ?, ?)
X(query_module, ?, ?, ?, ?, ?, ?)
X(quotactl, unsigned int cmd, const char *special, qid_t id, void *addr, -, -)
X(nfsservctl, ?, ?, ?, ?, ?, ?)
X(getpmsg, ?, ?, ?, ?, ?, ?)
X(putpmsg, ?, ?, ?, ?, ?, ?)
X(afs_syscall, ?, ?, ?, ?, ?, ?)
X(tuxcall, ?, ?, ?, ?, ?, ?)
X(security, ?, ?, ?, ?, ?, ?)
X(gettid, -, -, -, -, -, -)
X(readahead, int fd, loff_t offset, size_t count, -, -, -)
X(setxattr, const char *path, const char *name, const void *value, size_t size, int flags, -)
X(lsetxattr, const char *path, const char *name, const void *value, size_t size, int flags, -)
X(fsetxattr, int fd, const char *name, const void *value, size_t size, int flags, -)
X(getxattr, const char *path, const char *name, void *value, size_t size, -, -)
X(lgetxattr, const char *path, const char *name, void *value, size_t size, -, -)
X(fgetxattr, int fd, const char *name, void *value, size_t size, -, -)
X(listxattr, const char *path, char *list, size_t size, -, -, -)
X(llistxattr, const char *path, char *list, size_t size, -, -, -)
X(flistxattr, int fd, char *list, size_t size, -, -, -)
X(removexattr, const char *path, const char *name, -, -, -, -)
X(lremovexattr, const char *path, const char *name, -, -, -, -)
X(fremovexattr, int fd, const char *name, -, -, -, -)
X(tkill, pid_t pid, int sig, -, -, -, -)
X(time, time_t *tloc, -, -, -, -, -)
X(futex, u32 *uaddr, int op, u32 val, struct __kernel_timespec *utime, u32 *uaddr2, u32 val3)
X(sched_setaffinity, pid_t pid, unsigned int len, unsigned long *user_mask_ptr, -, -, -)
X(sched_getaffinity, pid_t pid, unsigned int len, unsigned long *user_mask_ptr, -, -, -)
X(set_thread_area, ?, ?, ?, ?, ?, ?)
X(io_setup, unsigned nr_reqs, aio_context_t *ctx, -, -, -, -)
X(io_destroy, aio_context_t ctx, -, -, -, -, -)
X(io_getevents, aio_context_t ctx_id, long min_nr, long nr, struct io_event *events, struct __kernel_timespec *timeout, -)
X(io_submit, aio_context_t, long, struct iocb * *, -, -, -)
X(io_cancel, aio_context_t ctx_id, struct iocb *iocb, struct io_event *result, -, -, -)
X(get_thread_area, ?, ?, ?, ?, ?, ?)
X(lookup_dcookie, u64 cookie64, char *buf, size_t len, -, -, -)
X(epoll_create, int size, -, -, -, -, -)
X(epoll_ctl_old, ?, ?, ?, ?, ?, ?)
X(epoll_wait_old, ?, ?, ?, ?, ?, ?)
X(remap_file_pages, unsigned long start, unsigned long size, unsigned long prot, unsigned long pgoff, unsigned long flags, -)
X(getdents64, unsigned int fd, struct linux_dirent64 *dirent, unsigned int count, -, -, -)
X(set_tid_address, int *tidptr, -, -, -, -, -)
X(restart_syscall, -, -, -, -, -, -)
X(semtimedop, int semid, struct sembuf *sops, unsigned nsops, const struct __kernel_timespec *timeout, -, -)
X(fadvise64, int fd, loff_t offset, size_t len, int advice, -, -)
X(timer_create, clockid_t which_clock, struct sigevent *timer_event_spec, timer_t * created_timer_id, -, -, -)
X(timer_settime, timer_t timer_id, int flags, const struct __kernel_itimerspec *new_setting, struct __kernel_itimerspec *old_setting, -, -)
X(timer_gettime, timer_t timer_id, struct __kernel_itimerspec *setting, -, -, -, -)
X(timer_getoverrun, timer_t timer_id, -, -, -, -, -)
X(timer_delete, timer_t timer_id, -, -, -, -, -)
X(clock_settime, clockid_t which_clock, const struct __kernel_timespec *tp, -, -, -, -)
X(clock_gettime, clockid_t which_clock, struct __kernel_timespec *tp, -, -, -, -)
X(clock_getres, clockid_t which_clock, struct __kernel_timespec *tp, -, -, -, -)
X(clock_nanosleep, clockid_t which_clock, int flags, const struct __kernel_timespec *rqtp, struct __kernel_timespec *rmtp, -, -)
X(exit_group, int error_code, -, -, -, -, -)
X(epoll_wait, int epfd, struct epoll_event *events, int maxevents, int timeout, -, -)
X(epoll_ctl, int epfd, int op, int fd, struct epoll_event *event, -, -)
X(tgkill, pid_t tgid, pid_t pid, int sig, -, -, -)
X(utimes, char *filename, struct timeval *utimes, -, -, -, -)
X(vserver, ?, ?, ?, ?, ?, ?)
X(mbind, unsigned long start, unsigned long len, unsigned long mode, const unsigned long *nmask, unsigned long maxnode, unsigned flags)
X(set_mempolicy, int mode, const unsigned long *nmask, unsigned long maxnode, -, -, -)
X(get_mempolicy, int *policy, unsigned long *nmask, unsigned long maxnode, unsigned long addr, unsigned long flags, -)
X(mq_open, const char *name, int oflag, umode_t mode, struct mq_attr *attr, -, -)
X(mq_unlink, const char *name, -, -, -, -, -)
X(mq_timedsend, mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio, const struct __kernel_timespec *abs_timeout, -)
X(mq_timedreceive, mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio, const struct __kernel_timespec *abs_timeout, -)
X(mq_notify, mqd_t mqdes, const struct sigevent *notification, -, -, -, -)
X(mq_getsetattr, mqd_t mqdes, const struct mq_attr *mqstat, struct mq_attr *omqstat, -, -, -)
X(kexec_load, unsigned long entry, unsigned long nr_segments, struct kexec_segment *segments, unsigned long flags, -, -)
X(waitid, int which, pid_t pid, struct siginfo *infop, int options, struct rusage *ru, -)
X(add_key, const char *_type, const char *_description, const void *_payload, size_t plen, key_serial_t destringid, -)
X(request_key, const char *_type, const char *_description, const char *_callout_info, key_serial_t destringid, -, -)
X(keyctl, int cmd, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5, -)
X(ioprio_set, int which, int who, int ioprio, -, -, -)
X(ioprio_get, int which, int who, -, -, -, -)
X(inotify_init, -, -, -, -, -, -)
X(inotify_add_watch, int fd, const char *path, u32 mask, -, -, -)
X(inotify_rm_watch, int fd, __s32 wd, -, -, -, -)
X(migrate_pages, pid_t pid, unsigned long maxnode, const unsigned long *from, const unsigned long *to, -, -)
X(openat, int dfd, const char *filename, int flags, umode_t mode, -, -)
X(mkdirat, int dfd, const char * pathname, umode_t mode, -, -, -)
X(mknodat, int dfd, const char * filename, umode_t mode, unsigned dev, -, -)
X(fchownat, int dfd, const char *filename, uid_t user, gid_t group, int flag, -)
X(futimesat, int dfd, const char *filename, struct timeval *utimes, -, -, -)
X(newfstatat, int dfd, const char *filename, struct stat *statbuf, int flag, -, -)
X(unlinkat, int dfd, const char * pathname, int flag, -, -, -)
X(renameat, int olddfd, const char * oldname, int newdfd, const char * newname, -, -)
X(linkat, int olddfd, const char *oldname, int newdfd, const char *newname, int flags, -)
X(symlinkat, const char * oldname, int newdfd, const char * newname, -, -, -)
X(readlinkat, int dfd, const char *path, char *buf, int bufsiz, -, -)
X(fchmodat, int dfd, const char * filename, umode_t mode, -, -, -)
X(faccessat, int dfd, const char *filename, int mode, -, -, -)
X(pselect6, int, fd_set *, fd_set *, fd_set *, struct __kernel_timespec *, void *)
X(ppoll, struct pollfd *, unsigned int, struct __kernel_timespec *, const sigset_t *, size_t, -)
X(unshare, unsigned long unshare_flags, -, -, -, -, -)
X(set_robust_list, struct robust_list_head *head, size_t len, -, -, -, -)
X(get_robust_list, int pid, struct robust_list_head * *head_ptr, size_t *len_ptr, -, -, -)
X(splice, int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags)
X(tee, int fdin, int fdout, size_t len, unsigned int flags, -, -)
X(sync_file_range, int fd, loff_t offset, loff_t nbytes, unsigned int flags, -, -)
X(vmsplice, int fd, const struct iovec *iov, unsigned long nr_segs, unsigned int flags, -, -)
X(move_pages, pid_t pid, unsigned long nr_pages, const void * *pages, const int *nodes, int *status, int flags)
X(utimensat, int dfd, const char *filename, struct __kernel_timespec *utimes, int flags, -, -)
X(epoll_pwait, int epfd, struct epoll_event *events, int maxevents, int timeout, const sigset_t *sigmask, size_t sigsetsize)
X(signalfd, int ufd, sigset_t *user_mask, size_t sizemask, -, -, -)
X(timerfd_create, int clockid, int flags, -, -, -, -)
X(eventfd, unsigned int count, -, -, -, -, -)
X(fallocate, int fd, int mode, loff_t offset, loff_t len, -, -)
X(timerfd_settime, int ufd, int flags, const struct __kernel_itimerspec *utmr, struct __kernel_itimerspec *otmr, -, -)
X(timerfd_gettime, int ufd, struct __kernel_itimerspec *otmr, -, -, -, -)
X(accept4, int, struct sockaddr *, int *, int, -, -)
X(signalfd4, int ufd, sigset_t *user_mask, size_t sizemask, int flags, -, -)
X(eventfd2, unsigned int count, int flags, -, -, -, -)
X(epoll_create1, int flags, -, -, -, -, -)
X(dup3, unsigned int oldfd, unsigned int newfd, int flags, -, -, -)
X(pipe2, int *fildes, int flags, -, -, -, -)
X(inotify_init1, int flags, -, -, -, -, -)
X(preadv, unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, -)
X(pwritev, unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, -)
X(rt_tgsigqueueinfo, pid_t tgid, pid_t pid, int sig, siginfo_t *uinfo, -, -)
X(perf_event_open, struct perf_event_attr *attr_uptr, pid_t pid, int cpu, int group_fd, unsigned long flags, -)
X(recvmmsg, int fd, struct mmsghdr *msg, unsigned int vlen, unsigned flags, struct __kernel_timespec *timeout, -)
X(fanotify_init, unsigned int flags, unsigned int event_f_flags, -, -, -, -)
X(fanotify_mark, int fanotify_fd, unsigned int flags, u64 mask, int fd, const char *pathname, -)
X(prlimit64, pid_t pid, unsigned int resource, const struct rlimit64 *new_rlim, struct rlimit64 *old_rlim, -, -)
X(name_to_handle_at, int dfd, const char *name, struct file_handle *handle, int *mnt_id, int flag, -)
X(open_by_handle_at, int mountdirfd, struct file_handle *handle, int flags, -, -, -)
X(clock_adjtime, clockid_t which_clock, struct __kernel_timex *tx, -, -, -, -)
X(syncfs, int fd, -, -, -, -, -)
X(sendmmsg, int fd, struct mmsghdr *msg, unsigned int vlen, unsigned flags, -, -)
X(setns, int fd, int nstype, -, -, -, -)
X(getcpu, unsigned *cpu, unsigned *node, struct getcpu_cache *cache, -, -, -)
X(process_vm_readv, pid_t pid, const struct iovec *lvec, unsigned long liovcnt, const struct iovec *rvec, unsigned long riovcnt, unsigned long flags)
X(process_vm_writev, pid_t pid, const struct iovec *lvec, unsigned long liovcnt, const struct iovec *rvec, unsigned long riovcnt, unsigned long flags)
X(kcmp, pid_t pid1, pid_t pid2, int type, unsigned long idx1, unsigned long idx2, -)
X(finit_module, int fd, const char *uargs, int flags, -, -, -)
X(sched_setattr, pid_t pid, struct sched_attr *attr, unsigned int flags, -, -, -)
X(sched_getattr, pid_t pid, struct sched_attr *attr, unsigned int size, unsigned int flags, -, -)
X(renameat2, int olddfd, const char *oldname, int newdfd, const char *newname, unsigned int flags, -)
X(seccomp, unsigned int op, unsigned int flags, void *uargs, -, -, -)
X(getrandom, char *buf, size_t count, unsigned int flags, -, -, -)
X(memfd_create, const char *uname_ptr, unsigned int flags, -, -, -, -)
X(kexec_file_load, int kernel_fd, int initrd_fd, unsigned long cmdline_len, const char *cmdline_ptr, unsigned long flags, -)
X(bpf, int cmd, union bpf_attr *attr, unsigned int size, -, -, -)
X(execveat, int dfd, const char *filename, const char *const *argv, const char *const *envp, int flags, -)
X(userfaultfd, int flags, -, -, -, -, -)
X(membarrier, int cmd, int flags, -, -, -, -)
X(mlock2, unsigned long start, size_t len, int flags, -, -, -)
X(copy_file_range, int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags)
X(preadv2, unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, rwf_t flags)
X(pwritev2, unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, rwf_t flags)
X(pkey_mprotect, unsigned long start, size_t len, unsigned long prot, int pkey, -, -)
X(pkey_alloc, unsigned long flags, unsigned long init_val, -, -, -, -)
X(pkey_free, int pkey, -, -, -, -, -)
X(statx, int dfd, const char *path, unsigned flags, unsigned mask, struct statx *buffer, -)

#undef X
#endif
