#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "test.h"

#if defined(__POSTRISC__)
#include "syscall.h"
#include "syscall_arch.h"
#endif

#if 0
static const char write_hello_msg[] = "write: hello from MUSL\n";
static const char fwrite_hello_msg[] = "fwrite: hello from MUSL\n";

int recursive(int value, int x)
{
    if (value < 1000) {
        printf("recursive1=%d %d\n", value, x);
        int result = x ^ recursive(value + 1, recursive(value + 990, x + 3));
        printf("recursive2=%d %d\n", value, x);
        return result;
    }
    // exit(x | value);
    return x | value;
}
#endif

int main(int argc, char **argv, char **env)
{
    char temp[20];

    snprintf(temp, sizeof(temp), "chatmacro%i", 10);
    printf("temp=%s\n", temp);

    M_snprintf(temp, sizeof(temp), "chatmacro%i", 10);
    printf("temp=%s\n", temp);

    for (int i=0; i<argc; i++) {
        printf("argv[%d]=%s\n", i, argv[i]);
    }
    for (int i=0; env[i]; i++) {
        printf("env[%d]=%s\n", i, env[i]);
    }

    return 0;
#if 0
    char src[] = "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
    char dst[] = "abcdefghijklmnopqrstyuvwxyzabcdefghijklmnopqrstyuvwxyzabcdefghijklmnopqrstyuvwxyzabcdefghi";
    //for(size_t i=0; i <= sizeof(src); i++) {
    //    printf("iiii=%d\n", (int)i);
        POSTRISC_DUMP_MEMORY(dst, sizeof(dst));
        memcpy(dst, src, 5);
        POSTRISC_DUMP_MEMORY(dst, sizeof(dst));
    //}
    return 0;

    int ret;
/*
    write(STDOUT_FILENO, write_hello_msg, sizeof(write_hello_msg)-1);
    fwrite(fwrite_hello_msg, 1, sizeof(fwrite_hello_msg)-1, stdout);
    test_switch(argc);
    test_valist(4, 6, 2, 3, 5);
    // ret = printf("MUSL! argc=%dnext\n", argc);
*/
    ret = printf("printf: hello from MUSL! argc=%d next x=%d &y=%p y=%p y[0]=%d\n", argc, x, &y, y, *y);

    char buf[] = "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
    ret = snprintf(buf, sizeof(buf), "snprintf=%d %d", 100, 110);
    printf("snprintf: ret=%d res=%s\n", ret, buf);
    write(STDOUT_FILENO, buf, sizeof(buf));

#if defined(__POSTRISC__)
    __builtin_postrisc_dump_register(ret);
#endif
    printf("\n");
    for(int i = 0; i < 10; i++) {
        printf("[%d] = %d %d %d %d\n", i, (int)s_float[i], (int)s_double[i], (int)-s_float[i], (int)-s_double[i]);
    }

    printf("rec=%d\n", recursive(0, argc));
    return 0;
#endif
}
