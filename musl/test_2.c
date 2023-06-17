#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#include "test.h"

int x = 155;
int *y = &x;

__attribute__((noinline)) int test_switch(int x)
{
    switch (x) {

#define TEXT(NUM) "test_switch " #NUM " detected\n"
#define X(NUM) case NUM: \
    fwrite(TEXT(NUM), 1, sizeof(TEXT(NUM))-1, stdout); \
    break;

    X(1)
    X(2)
    X(3)
    X(4)
    X(5)
    X(6)
    }
    return 0;
}

__attribute__((noinline)) int test_valist(int fmt, ...)
{
    unsigned value;
    va_list ap;
    va_start(ap, fmt);
    value = va_arg(ap, unsigned);
    test_switch(value);
    value = va_arg(ap, unsigned);
    test_switch(value);
    value = va_arg(ap, unsigned);
    test_switch(value);
    value = va_arg(ap, unsigned);
    test_switch(value);
    va_end(ap);
    return 0;
}

float s_float[10] = {
    2.0f,
    2.1f,
    2.2f,
    2.3f,
    2.4f,
    2.5f,
    2.6f,
    2.7f,
    2.8f,
    2.9f,
};

double s_double[10] = {
    2.0,
    2.1,
    2.2,
    2.3,
    2.4,
    2.5,
    2.6,
    2.7,
    2.8,
    2.9,
};

// Safe, portable vsnprintf().
int M_vsnprintf(char *buf, size_t buf_len, const char *s, va_list args)
{
    int result;

    if (buf_len < 1)
    {
        return 0;
    }

    // Windows (and other OSes?) has a vsnprintf() that doesn't always
    // append a trailing \0. So we must do it, and write into a buffer
    // that is one byte shorter; otherwise this function is unsafe.
    result = vsnprintf(buf, buf_len, s, args);

    // FIXME: M_vsnprintf doens't work without this printf
    // printf("vsnprintf: len=%d fmt=%s res=%s\n", result, s, buf);
    // If truncated, change the final char in the buffer to a \0.
    // A negative result indicates a truncated buffer on Windows.
    if (result < 0 || result >= buf_len)
    {
        buf[buf_len - 1] = '\0';
        result = buf_len - 1;
    }

    return result;
}

// Safe, portable snprintf().
int M_snprintf(char *buf, size_t buf_len, const char *s, ...)
{
    va_list args;
    int result;
    va_start(args, s);
    result = M_vsnprintf(buf, buf_len, s, args);
    va_end(args);
    return result;
}
