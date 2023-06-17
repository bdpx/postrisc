#pragma once

extern int x;
extern int *y;

__attribute__((noinline)) int test_valist(int fmt, ...);
__attribute__((noinline)) extern int test_switch(int x);

extern float s_float[10];
extern double s_double[10];

int M_vsnprintf(char *buf, size_t buf_len, const char *s, va_list args);
int M_snprintf(char *buf, size_t buf_len, const char *s, ...) __attribute__ ((format (printf, 3, 4)));
