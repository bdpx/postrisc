X(0, read, 0x00, unsigned int fd, char *buf, size_t count, -, -, -
X(1, write, 0x01, unsigned int fd, const char *buf, size_t count, -, -, -
X(2, open, 0x02, const char *filename, int flags, umode_t mode, -, -, -
X(3, close, 0x03, unsigned int fd, -, -, -, -, -
X(4, stat, 0x04, const char *filename, struct __old_kernel_stat *statbuf, -, -, -, -
X(5, fstat, 0x05, unsigned int fd, struct __old_kernel_stat *statbuf, -, -, -, -
X(6, lstat, 0x06, const char *filename, struct __old_kernel_stat *statbuf, -, -, -, -
X(7, poll, 0x07, struct pollfd *ufds, unsigned int nfds, int timeout, -, -, -
X(8, lseek, 0x08, unsigned int fd, off_t offset, unsigned int whence, -, -, -
X(9, mmap, 0x09, void *addr, size_t length, int prot, int flags, int fd, off_t offset
X(10, mprotect, 0x0a, unsigned long start, size_t len, unsigned long prot, -, -, -
X(11, munmap, 0x0b, unsigned long addr, size_t len, -, -, -, -
X(12, brk, 0x0c, unsigned long brk, -, -, -, -, -
X(13, rt_sigaction, 0x0d, int, const struct sigaction *, struct sigaction *, size_t, -, -
X(14, rt_sigprocmask, 0x0e, int how, sigset_t *set, sigset_t *oset, size_t sigsetsize, -, -
X(15, rt_sigreturn, 0x0f, ?, ?, ?, ?, ?, ?
X(16, ioctl, 0x10, unsigned int fd, unsigned int cmd, unsigned long arg, -, -, -
X(17, pread64, 0x11, unsigned int fd, char *buf, size_t count, loff_t pos, -, -
X(18, pwrite64, 0x12, unsigned int fd, const char *buf, size_t count, loff_t pos, -, -
X(19, readv, 0x13, unsigned long fd, const struct iovec *vec, unsigned long vlen, -, -, -
X(20, writev, 0x14, unsigned long fd, const struct iovec *vec, unsigned long vlen, -, -, -
X(21, access, 0x15, const char *filename, int mode, -, -, -, -
X(22, pipe, 0x16, int *fildes, -, -, -, -, -
X(23, select, 0x17, int n, fd_set *inp, fd_set *outp, fd_set *exp, struct timeval *tvp, -
X(24, sched_yield, 0x18, -, -, -, -, -, -
X(25, mremap, 0x19, unsigned long addr, unsigned long old_len, unsigned long new_len, unsigned long flags, unsigned long new_addr, -
X(26, msync, 0x1a, unsigned long start, size_t len, int flags, -, -, -
X(27, mincore, 0x1b, unsigned long start, size_t len, unsigned char * vec, -, -, -
X(28, madvise, 0x1c, unsigned long start, size_t len, int behavior, -, -, -
X(29, shmget, 0x1d, key_t key, size_t size, int flag, -, -, -
X(30, shmat, 0x1e, int shmid, char *shmaddr, int shmflg, -, -, -
X(31, shmctl, 0x1f, int shmid, int cmd, struct shmid_ds *buf, -, -, -
X(32, dup, 0x20, unsigned int fildes, -, -, -, -, -
X(33, dup2, 0x21, unsigned int oldfd, unsigned int newfd, -, -, -, -
X(34, pause, 0x22, -, -, -, -, -, -
X(35, nanosleep, 0x23, struct __kernel_timespec *rqtp, struct __kernel_timespec *rmtp, -, -, -, -
X(36, getitimer, 0x24, int which, struct itimerval *value, -, -, -, -
X(37, alarm, 0x25, unsigned int seconds, -, -, -, -, -
X(38, setitimer, 0x26, int which, struct itimerval *value, struct itimerval *ovalue, -, -, -
X(39, getpid, 0x27, -, -, -, -, -, -
X(40, sendfile, 0x28, int out_fd, int in_fd, off_t *offset, size_t count, -, -
X(41, socket, 0x29, int, int, int, -, -, -
X(42, connect, 0x2a, int, struct sockaddr *, int, -, -, -
X(43, accept, 0x2b, int, struct sockaddr *, int *, -, -, -
X(44, sendto, 0x2c, int, void *, size_t, unsigned, struct sockaddr *, int
X(45, recvfrom, 0x2d, int, void *, size_t, unsigned, struct sockaddr *, int *
X(46, sendmsg, 0x2e, int fd, struct user_msghdr *msg, unsigned flags, -, -, -
X(47, recvmsg, 0x2f, int fd, struct user_msghdr *msg, unsigned flags, -, -, -
X(48, shutdown, 0x30, int, int, -, -, -, -
X(49, bind, 0x31, int, struct sockaddr *, int, -, -, -
X(50, listen, 0x32, int, int, -, -, -, -
X(51, getsockname, 0x33, int, struct sockaddr *, int *, -, -, -
X(52, getpeername, 0x34, int, struct sockaddr *, int *, -, -, -
X(53, socketpair, 0x35, int, int, int, int *, -, -
X(54, setsockopt, 0x36, int fd, int level, int optname, char *optval, int optlen, -
X(55, getsockopt, 0x37, int fd, int level, int optname, char *optval, int *optlen, -
X(56, clone, 0x38, unsigned long, unsigned long, int *, int *, unsigned long, -
X(57, fork, 0x39, -, -, -, -, -, -
X(58, vfork, 0x3a, -, -, -, -, -, -
X(59, execve, 0x3b, const char *filename, const char *const *argv, const char *const *envp, -, -, -
X(60, exit, 0x3c, int error_code, -, -, -, -, -
X(61, wait4, 0x3d, pid_t pid, int *stat_addr, int options, struct rusage *ru, -, -
X(62, kill, 0x3e, pid_t pid, int sig, -, -, -, -
X(63, uname, 0x3f, struct old_utsname *, -, -, -, -, -
X(64, semget, 0x40, key_t key, int nsems, int semflg, -, -, -
X(65, semop, 0x41, int semid, struct sembuf *sops, unsigned nsops, -, -, -
X(66, semctl, 0x42, int semid, int semnum, int cmd, unsigned long arg, -, -
X(67, shmdt, 0x43, char *shmaddr, -, -, -, -, -
X(68, msgget, 0x44, key_t key, int msgflg, -, -, -, -
X(69, msgsnd, 0x45, int msqid, struct msgbuf *msgp, size_t msgsz, int msgflg, -, -
X(70, msgrcv, 0x46, int msqid, struct msgbuf *msgp, size_t msgsz, long msgtyp, int msgflg, -
X(71, msgctl, 0x47, int msqid, int cmd, struct msqid_ds *buf, -, -, -
X(72, fcntl, 0x48, unsigned int fd, unsigned int cmd, unsigned long arg, -, -, -
X(73, flock, 0x49, unsigned int fd, unsigned int cmd, -, -, -, -
X(74, fsync, 0x4a, unsigned int fd, -, -, -, -, -
X(75, fdatasync, 0x4b, unsigned int fd, -, -, -, -, -
X(76, truncate, 0x4c, const char *path, long length, -, -, -, -
X(77, ftruncate, 0x4d, unsigned int fd, unsigned long length, -, -, -, -
X(78, getdents, 0x4e, unsigned int fd, struct linux_dirent *dirent, unsigned int count, -, -, -
X(79, getcwd, 0x4f, char *buf, unsigned long size, -, -, -, -
X(80, chdir, 0x50, const char *filename, -, -, -, -, -
X(81, fchdir, 0x51, unsigned int fd, -, -, -, -, -
X(82, rename, 0x52, const char *oldname, const char *newname, -, -, -, -
X(83, mkdir, 0x53, const char *pathname, umode_t mode, -, -, -, -
X(84, rmdir, 0x54, const char *pathname, -, -, -, -, -
X(85, creat, 0x55, const char *pathname, umode_t mode, -, -, -, -
X(86, link, 0x56, const char *oldname, const char *newname, -, -, -, -
X(87, unlink, 0x57, const char *pathname, -, -, -, -, -
X(88, symlink, 0x58, const char *old, const char *new, -, -, -, -
X(89, readlink, 0x59, const char *path, char *buf, int bufsiz, -, -, -
X(90, chmod, 0x5a, const char *filename, umode_t mode, -, -, -, -
X(91, fchmod, 0x5b, unsigned int fd, umode_t mode, -, -, -, -
X(92, chown, 0x5c, const char *filename, uid_t user, gid_t group, -, -, -
X(93, fchown, 0x5d, unsigned int fd, uid_t user, gid_t group, -, -, -
X(94, lchown, 0x5e, const char *filename, uid_t user, gid_t group, -, -, -
X(95, umask, 0x5f, int mask, -, -, -, -, -
X(96, gettimeofday, 0x60, struct timeval *tv, struct timezone *tz, -, -, -, -
X(97, getrlimit, 0x61, unsigned int resource, struct rlimit *rlim, -, -, -, -
X(98, getrusage, 0x62, int who, struct rusage *ru, -, -, -, -
X(99, sysinfo, 0x63, struct sysinfo *info, -, -, -, -, -
X(100, times, 0x64, struct tms *tbuf, -, -, -, -, -
X(101, ptrace, 0x65, long request, long pid, unsigned long addr, unsigned long data, -, -
X(102, getuid, 0x66, -, -, -, -, -, -
X(103, syslog, 0x67, int type, char *buf, int len, -, -, -
X(104, getgid, 0x68, -, -, -, -, -, -
X(105, setuid, 0x69, uid_t uid, -, -, -, -, -
X(106, setgid, 0x6a, gid_t gid, -, -, -, -, -
X(107, geteuid, 0x6b, -, -, -, -, -, -
X(108, getegid, 0x6c, -, -, -, -, -, -
X(109, setpgid, 0x6d, pid_t pid, pid_t pgid, -, -, -, -
X(110, getppid, 0x6e, -, -, -, -, -, -
X(111, getpgrp, 0x6f, -, -, -, -, -, -
X(112, setsid, 0x70, -, -, -, -, -, -
X(113, setreuid, 0x71, uid_t ruid, uid_t euid, -, -, -, -
X(114, setregid, 0x72, gid_t rgid, gid_t egid, -, -, -, -
X(115, getgroups, 0x73, int gidsetsize, gid_t *grouplist, -, -, -, -
X(116, setgroups, 0x74, int gidsetsize, gid_t *grouplist, -, -, -, -
X(117, setresuid, 0x75, uid_t ruid, uid_t euid, uid_t suid, -, -, -
X(118, getresuid, 0x76, uid_t *ruid, uid_t *euid, uid_t *suid, -, -, -
X(119, setresgid, 0x77, gid_t rgid, gid_t egid, gid_t sgid, -, -, -
X(120, getresgid, 0x78, gid_t *rgid, gid_t *egid, gid_t *sgid, -, -, -
X(121, getpgid, 0x79, pid_t pid, -, -, -, -, -
X(122, setfsuid, 0x7a, uid_t uid, -, -, -, -, -
X(123, setfsgid, 0x7b, gid_t gid, -, -, -, -, -
X(124, getsid, 0x7c, pid_t pid, -, -, -, -, -
X(125, capget, 0x7d, cap_user_header_t header, cap_user_data_t dataptr, -, -, -, -
X(126, capset, 0x7e, cap_user_header_t header, const cap_user_data_t data, -, -, -, -
X(127, rt_sigpending, 0x7f, sigset_t *set, size_t sigsetsize, -, -, -, -
X(128, rt_sigtimedwait, 0x80, const sigset_t *uthese, siginfo_t *uinfo, const struct __kernel_timespec *uts, size_t sigsetsize, -, -
X(129, rt_sigqueueinfo, 0x81, pid_t pid, int sig, siginfo_t *uinfo, -, -, -
X(130, rt_sigsuspend, 0x82, sigset_t *unewset, size_t sigsetsize, -, -, -, -
X(131, sigaltstack, 0x83, const struct sigaltstack *uss, struct sigaltstack *uoss, -, -, -, -
X(132, utime, 0x84, char *filename, struct utimbuf *times, -, -, -, -
X(133, mknod, 0x85, const char *filename, umode_t mode, unsigned dev, -, -, -
X(134, uselib, 0x86, const char *library, -, -, -, -, -
X(135, personality, 0x87, unsigned int personality, -, -, -, -, -
X(136, ustat, 0x88, unsigned dev, struct ustat *ubuf, -, -, -, -
X(137, statfs, 0x89, const char * path, struct statfs *buf, -, -, -, -
X(138, fstatfs, 0x8a, unsigned int fd, struct statfs *buf, -, -, -, -
X(139, sysfs, 0x8b, int option, unsigned long arg1, unsigned long arg2, -, -, -
X(140, getpriority, 0x8c, int which, int who, -, -, -, -
X(141, setpriority, 0x8d, int which, int who, int niceval, -, -, -
X(142, sched_setparam, 0x8e, pid_t pid, struct sched_param *param, -, -, -, -
X(143, sched_getparam, 0x8f, pid_t pid, struct sched_param *param, -, -, -, -
X(144, sched_setscheduler, 0x90, pid_t pid, int policy, struct sched_param *param, -, -, -
X(145, sched_getscheduler, 0x91, pid_t pid, -, -, -, -, -
X(146, sched_get_priority_max, 0x92, int policy, -, -, -, -, -
X(147, sched_get_priority_min, 0x93, int policy, -, -, -, -, -
X(148, sched_rr_get_interval, 0x94, pid_t pid, struct __kernel_timespec *interval, -, -, -, -
X(149, mlock, 0x95, unsigned long start, size_t len, -, -, -, -
X(150, munlock, 0x96, unsigned long start, size_t len, -, -, -, -
X(151, mlockall, 0x97, int flags, -, -, -, -, -
X(152, munlockall, 0x98, -, -, -, -, -, -
X(153, vhangup, 0x99, -, -, -, -, -, -
X(154, modify_ldt, 0x9a, ?, ?, ?, ?, ?, ?
X(155, pivot_root, 0x9b, const char *new_root, const char *put_old, -, -, -, -
X(156, _sysctl, 0x9c, ?, ?, ?, ?, ?, ?
X(157, prctl, 0x9d, int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5, -
X(158, arch_prctl, 0x9e, ?, ?, ?, ?, ?, ?
X(159, adjtimex, 0x9f, struct __kernel_timex *txc_p, -, -, -, -, -
X(160, setrlimit, 0xa0, unsigned int resource, struct rlimit *rlim, -, -, -, -
X(161, chroot, 0xa1, const char *filename, -, -, -, -, -
X(162, sync, 0xa2, -, -, -, -, -, -
X(163, acct, 0xa3, const char *name, -, -, -, -, -
X(164, settimeofday, 0xa4, struct timeval *tv, struct timezone *tz, -, -, -, -
X(165, mount, 0xa5, char *dev_name, char *dir_name, char *type, unsigned long flags, void *data, -
X(166, umount2, 0xa6, ?, ?, ?, ?, ?, ?
X(167, swapon, 0xa7, const char *specialfile, int swap_flags, -, -, -, -
X(168, swapoff, 0xa8, const char *specialfile, -, -, -, -, -
X(169, reboot, 0xa9, int magic1, int magic2, unsigned int cmd, void *arg, -, -
X(170, sethostname, 0xaa, char *name, int len, -, -, -, -
X(171, setdomainname, 0xab, char *name, int len, -, -, -, -
X(172, iopl, 0xac, ?, ?, ?, ?, ?, ?
X(173, ioperm, 0xad, unsigned long from, unsigned long num, int on, -, -, -
X(174, create_module, 0xae, ?, ?, ?, ?, ?, ?
X(175, init_module, 0xaf, void *umod, unsigned long len, const char *uargs, -, -, -
X(176, delete_module, 0xb0, const char *name_user, unsigned int flags, -, -, -, -
X(177, get_kernel_syms, 0xb1, ?, ?, ?, ?, ?, ?
X(178, query_module, 0xb2, ?, ?, ?, ?, ?, ?
X(179, quotactl, 0xb3, unsigned int cmd, const char *special, qid_t id, void *addr, -, -
X(180, nfsservctl, 0xb4, ?, ?, ?, ?, ?, ?
X(181, getpmsg, 0xb5, ?, ?, ?, ?, ?, ?
X(182, putpmsg, 0xb6, ?, ?, ?, ?, ?, ?
X(183, afs_syscall, 0xb7, ?, ?, ?, ?, ?, ?
X(184, tuxcall, 0xb8, ?, ?, ?, ?, ?, ?
X(185, security, 0xb9, ?, ?, ?, ?, ?, ?
X(186, gettid, 0xba, -, -, -, -, -, -
X(187, readahead, 0xbb, int fd, loff_t offset, size_t count, -, -, -
X(188, setxattr, 0xbc, const char *path, const char *name, const void *value, size_t size, int flags, -
X(189, lsetxattr, 0xbd, const char *path, const char *name, const void *value, size_t size, int flags, -
X(190, fsetxattr, 0xbe, int fd, const char *name, const void *value, size_t size, int flags, -
X(191, getxattr, 0xbf, const char *path, const char *name, void *value, size_t size, -, -
X(192, lgetxattr, 0xc0, const char *path, const char *name, void *value, size_t size, -, -
X(193, fgetxattr, 0xc1, int fd, const char *name, void *value, size_t size, -, -
X(194, listxattr, 0xc2, const char *path, char *list, size_t size, -, -, -
X(195, llistxattr, 0xc3, const char *path, char *list, size_t size, -, -, -
X(196, flistxattr, 0xc4, int fd, char *list, size_t size, -, -, -
X(197, removexattr, 0xc5, const char *path, const char *name, -, -, -, -
X(198, lremovexattr, 0xc6, const char *path, const char *name, -, -, -, -
X(199, fremovexattr, 0xc7, int fd, const char *name, -, -, -, -
X(200, tkill, 0xc8, pid_t pid, int sig, -, -, -, -
X(201, time, 0xc9, time_t *tloc, -, -, -, -, -
X(202, futex, 0xca, u32 *uaddr, int op, u32 val, struct __kernel_timespec *utime, u32 *uaddr2, u32 val3
X(203, sched_setaffinity, 0xcb, pid_t pid, unsigned int len, unsigned long *user_mask_ptr, -, -, -
X(204, sched_getaffinity, 0xcc, pid_t pid, unsigned int len, unsigned long *user_mask_ptr, -, -, -
X(205, set_thread_area, 0xcd, ?, ?, ?, ?, ?, ?
X(206, io_setup, 0xce, unsigned nr_reqs, aio_context_t *ctx, -, -, -, -
X(207, io_destroy, 0xcf, aio_context_t ctx, -, -, -, -, -
X(208, io_getevents, 0xd0, aio_context_t ctx_id, long min_nr, long nr, struct io_event *events, struct __kernel_timespec *timeout, -
X(209, io_submit, 0xd1, aio_context_t, long, struct iocb * *, -, -, -
X(210, io_cancel, 0xd2, aio_context_t ctx_id, struct iocb *iocb, struct io_event *result, -, -, -
X(211, get_thread_area, 0xd3, ?, ?, ?, ?, ?, ?
X(212, lookup_dcookie, 0xd4, u64 cookie64, char *buf, size_t len, -, -, -
X(213, epoll_create, 0xd5, int size, -, -, -, -, -
X(214, epoll_ctl_old, 0xd6, ?, ?, ?, ?, ?, ?
X(215, epoll_wait_old, 0xd7, ?, ?, ?, ?, ?, ?
X(216, remap_file_pages, 0xd8, unsigned long start, unsigned long size, unsigned long prot, unsigned long pgoff, unsigned long flags, -
X(217, getdents64, 0xd9, unsigned int fd, struct linux_dirent64 *dirent, unsigned int count, -, -, -
X(218, set_tid_address, 0xda, int *tidptr, -, -, -, -, -
X(219, restart_syscall, 0xdb, -, -, -, -, -, -
X(220, semtimedop, 0xdc, int semid, struct sembuf *sops, unsigned nsops, const struct __kernel_timespec *timeout, -, -
X(221, fadvise64, 0xdd, int fd, loff_t offset, size_t len, int advice, -, -
X(222, timer_create, 0xde, clockid_t which_clock, struct sigevent *timer_event_spec, timer_t * created_timer_id, -, -, -
X(223, timer_settime, 0xdf, timer_t timer_id, int flags, const struct __kernel_itimerspec *new_setting, struct __kernel_itimerspec *old_setting, -, -
X(224, timer_gettime, 0xe0, timer_t timer_id, struct __kernel_itimerspec *setting, -, -, -, -
X(225, timer_getoverrun, 0xe1, timer_t timer_id, -, -, -, -, -
X(226, timer_delete, 0xe2, timer_t timer_id, -, -, -, -, -
X(227, clock_settime, 0xe3, clockid_t which_clock, const struct __kernel_timespec *tp, -, -, -, -
X(228, clock_gettime, 0xe4, clockid_t which_clock, struct __kernel_timespec *tp, -, -, -, -
X(229, clock_getres, 0xe5, clockid_t which_clock, struct __kernel_timespec *tp, -, -, -, -
X(230, clock_nanosleep, 0xe6, clockid_t which_clock, int flags, const struct __kernel_timespec *rqtp, struct __kernel_timespec *rmtp, -, -
X(231, exit_group, 0xe7, int error_code, -, -, -, -, -
X(232, epoll_wait, 0xe8, int epfd, struct epoll_event *events, int maxevents, int timeout, -, -
X(233, epoll_ctl, 0xe9, int epfd, int op, int fd, struct epoll_event *event, -, -
X(234, tgkill, 0xea, pid_t tgid, pid_t pid, int sig, -, -, -
X(235, utimes, 0xeb, char *filename, struct timeval *utimes, -, -, -, -
X(236, vserver, 0xec, ?, ?, ?, ?, ?, ?
X(237, mbind, 0xed, unsigned long start, unsigned long len, unsigned long mode, const unsigned long *nmask, unsigned long maxnode, unsigned flags
X(238, set_mempolicy, 0xee, int mode, const unsigned long *nmask, unsigned long maxnode, -, -, -
X(239, get_mempolicy, 0xef, int *policy, unsigned long *nmask, unsigned long maxnode, unsigned long addr, unsigned long flags, -
X(240, mq_open, 0xf0, const char *name, int oflag, umode_t mode, struct mq_attr *attr, -, -
X(241, mq_unlink, 0xf1, const char *name, -, -, -, -, -
X(242, mq_timedsend, 0xf2, mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio, const struct __kernel_timespec *abs_timeout, -
X(243, mq_timedreceive, 0xf3, mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio, const struct __kernel_timespec *abs_timeout, -
X(244, mq_notify, 0xf4, mqd_t mqdes, const struct sigevent *notification, -, -, -, -
X(245, mq_getsetattr, 0xf5, mqd_t mqdes, const struct mq_attr *mqstat, struct mq_attr *omqstat, -, -, -
X(246, kexec_load, 0xf6, unsigned long entry, unsigned long nr_segments, struct kexec_segment *segments, unsigned long flags, -, -
X(247, waitid, 0xf7, int which, pid_t pid, struct siginfo *infop, int options, struct rusage *ru, -
X(248, add_key, 0xf8, const char *_type, const char *_description, const void *_payload, size_t plen, key_serial_t destringid, -
X(249, request_key, 0xf9, const char *_type, const char *_description, const char *_callout_info, key_serial_t destringid, -, -
X(250, keyctl, 0xfa, int cmd, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5, -
X(251, ioprio_set, 0xfb, int which, int who, int ioprio, -, -, -
X(252, ioprio_get, 0xfc, int which, int who, -, -, -, -
X(253, inotify_init, 0xfd, -, -, -, -, -, -
X(254, inotify_add_watch, 0xfe, int fd, const char *path, u32 mask, -, -, -
X(255, inotify_rm_watch, 0xff, int fd, __s32 wd, -, -, -, -
X(256, migrate_pages, 0x100, pid_t pid, unsigned long maxnode, const unsigned long *from, const unsigned long *to, -, -
X(257, openat, 0x101, int dfd, const char *filename, int flags, umode_t mode, -, -
X(258, mkdirat, 0x102, int dfd, const char * pathname, umode_t mode, -, -, -
X(259, mknodat, 0x103, int dfd, const char * filename, umode_t mode, unsigned dev, -, -
X(260, fchownat, 0x104, int dfd, const char *filename, uid_t user, gid_t group, int flag, -
X(261, futimesat, 0x105, int dfd, const char *filename, struct timeval *utimes, -, -, -
X(262, newfstatat, 0x106, int dfd, const char *filename, struct stat *statbuf, int flag, -, -
X(263, unlinkat, 0x107, int dfd, const char * pathname, int flag, -, -, -
X(264, renameat, 0x108, int olddfd, const char * oldname, int newdfd, const char * newname, -, -
X(265, linkat, 0x109, int olddfd, const char *oldname, int newdfd, const char *newname, int flags, -
X(266, symlinkat, 0x10a, const char * oldname, int newdfd, const char * newname, -, -, -
X(267, readlinkat, 0x10b, int dfd, const char *path, char *buf, int bufsiz, -, -
X(268, fchmodat, 0x10c, int dfd, const char * filename, umode_t mode, -, -, -
X(269, faccessat, 0x10d, int dfd, const char *filename, int mode, -, -, -
X(270, pselect6, 0x10e, int, fd_set *, fd_set *, fd_set *, struct __kernel_timespec *, void *
X(271, ppoll, 0x10f, struct pollfd *, unsigned int, struct __kernel_timespec *, const sigset_t *, size_t, -
X(272, unshare, 0x110, unsigned long unshare_flags, -, -, -, -, -
X(273, set_robust_list, 0x111, struct robust_list_head *head, size_t len, -, -, -, -
X(274, get_robust_list, 0x112, int pid, struct robust_list_head * *head_ptr, size_t *len_ptr, -, -, -
X(275, splice, 0x113, int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags
X(276, tee, 0x114, int fdin, int fdout, size_t len, unsigned int flags, -, -
X(277, sync_file_range, 0x115, int fd, loff_t offset, loff_t nbytes, unsigned int flags, -, -
X(278, vmsplice, 0x116, int fd, const struct iovec *iov, unsigned long nr_segs, unsigned int flags, -, -
X(279, move_pages, 0x117, pid_t pid, unsigned long nr_pages, const void * *pages, const int *nodes, int *status, int flags
X(280, utimensat, 0x118, int dfd, const char *filename, struct __kernel_timespec *utimes, int flags, -, -
X(281, epoll_pwait, 0x119, int epfd, struct epoll_event *events, int maxevents, int timeout, const sigset_t *sigmask, size_t sigsetsize
X(282, signalfd, 0x11a, int ufd, sigset_t *user_mask, size_t sizemask, -, -, -
X(283, timerfd_create, 0x11b, int clockid, int flags, -, -, -, -
X(284, eventfd, 0x11c, unsigned int count, -, -, -, -, -
X(285, fallocate, 0x11d, int fd, int mode, loff_t offset, loff_t len, -, -
X(286, timerfd_settime, 0x11e, int ufd, int flags, const struct __kernel_itimerspec *utmr, struct __kernel_itimerspec *otmr, -, -
X(287, timerfd_gettime, 0x11f, int ufd, struct __kernel_itimerspec *otmr, -, -, -, -
X(288, accept4, 0x120, int, struct sockaddr *, int *, int, -, -
X(289, signalfd4, 0x121, int ufd, sigset_t *user_mask, size_t sizemask, int flags, -, -
X(290, eventfd2, 0x122, unsigned int count, int flags, -, -, -, -
X(291, epoll_create1, 0x123, int flags, -, -, -, -, -
X(292, dup3, 0x124, unsigned int oldfd, unsigned int newfd, int flags, -, -, -
X(293, pipe2, 0x125, int *fildes, int flags, -, -, -, -
X(294, inotify_init1, 0x126, int flags, -, -, -, -, -
X(295, preadv, 0x127, unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, -
X(296, pwritev, 0x128, unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, -
X(297, rt_tgsigqueueinfo, 0x129, pid_t tgid, pid_t pid, int sig, siginfo_t *uinfo, -, -
X(298, perf_event_open, 0x12a, struct perf_event_attr *attr_uptr, pid_t pid, int cpu, int group_fd, unsigned long flags, -
X(299, recvmmsg, 0x12b, int fd, struct mmsghdr *msg, unsigned int vlen, unsigned flags, struct __kernel_timespec *timeout, -
X(300, fanotify_init, 0x12c, unsigned int flags, unsigned int event_f_flags, -, -, -, -
X(301, fanotify_mark, 0x12d, int fanotify_fd, unsigned int flags, u64 mask, int fd, const char *pathname, -
X(302, prlimit64, 0x12e, pid_t pid, unsigned int resource, const struct rlimit64 *new_rlim, struct rlimit64 *old_rlim, -, -
X(303, name_to_handle_at, 0x12f, int dfd, const char *name, struct file_handle *handle, int *mnt_id, int flag, -
X(304, open_by_handle_at, 0x130, int mountdirfd, struct file_handle *handle, int flags, -, -, -
X(305, clock_adjtime, 0x131, clockid_t which_clock, struct __kernel_timex *tx, -, -, -, -
X(306, syncfs, 0x132, int fd, -, -, -, -, -
X(307, sendmmsg, 0x133, int fd, struct mmsghdr *msg, unsigned int vlen, unsigned flags, -, -
X(308, setns, 0x134, int fd, int nstype, -, -, -, -
X(309, getcpu, 0x135, unsigned *cpu, unsigned *node, struct getcpu_cache *cache, -, -, -
X(310, process_vm_readv, 0x136, pid_t pid, const struct iovec *lvec, unsigned long liovcnt, const struct iovec *rvec, unsigned long riovcnt, unsigned long flags
X(311, process_vm_writev, 0x137, pid_t pid, const struct iovec *lvec, unsigned long liovcnt, const struct iovec *rvec, unsigned long riovcnt, unsigned long flags
X(312, kcmp, 0x138, pid_t pid1, pid_t pid2, int type, unsigned long idx1, unsigned long idx2, -
X(313, finit_module, 0x139, int fd, const char *uargs, int flags, -, -, -
X(314, sched_setattr, 0x13a, pid_t pid, struct sched_attr *attr, unsigned int flags, -, -, -
X(315, sched_getattr, 0x13b, pid_t pid, struct sched_attr *attr, unsigned int size, unsigned int flags, -, -
X(316, renameat2, 0x13c, int olddfd, const char *oldname, int newdfd, const char *newname, unsigned int flags, -
X(317, seccomp, 0x13d, unsigned int op, unsigned int flags, void *uargs, -, -, -
X(318, getrandom, 0x13e, char *buf, size_t count, unsigned int flags, -, -, -
X(319, memfd_create, 0x13f, const char *uname_ptr, unsigned int flags, -, -, -, -
X(320, kexec_file_load, 0x140, int kernel_fd, int initrd_fd, unsigned long cmdline_len, const char *cmdline_ptr, unsigned long flags, -
X(321, bpf, 0x141, int cmd, union bpf_attr *attr, unsigned int size, -, -, -
X(322, execveat, 0x142, int dfd, const char *filename, const char *const *argv, const char *const *envp, int flags, -
X(323, userfaultfd, 0x143, int flags, -, -, -, -, -
X(324, membarrier, 0x144, int cmd, int flags, -, -, -, -
X(325, mlock2, 0x145, unsigned long start, size_t len, int flags, -, -, -
X(326, copy_file_range, 0x146, int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags
X(327, preadv2, 0x147, unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, rwf_t flags
X(328, pwritev2, 0x148, unsigned long fd, const struct iovec *vec, unsigned long vlen, unsigned long pos_l, unsigned long pos_h, rwf_t flags
X(329, pkey_mprotect, 0x149, unsigned long start, size_t len, unsigned long prot, int pkey, -, -
X(330, pkey_alloc, 0x14a, unsigned long flags, unsigned long init_val, -, -, -, -
X(331, pkey_free, 0x14b, int pkey, -, -, -, -, -
X(332, statx, 0x14c, int dfd, const char *path, unsigned flags, unsigned mask, struct statx *buffer, -
