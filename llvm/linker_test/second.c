
int bss_value;

extern int extern_value[8];

int extern_func(int a, int b)
{
    return a + b + extern_value[0] + extern_value[1] + extern_value[2] + extern_value[3];
}
