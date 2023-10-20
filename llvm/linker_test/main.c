
int extern_value[8] = { 1, 2, 3, 4 };

extern int extern_func(int, int);

int main(void)
{
    return extern_func(1, 1);
}

void _start() {
  main();
  __builtin_postrisc_halt();
}