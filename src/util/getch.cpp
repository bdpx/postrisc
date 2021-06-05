
#if defined(__unix__)

#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>

static int get_char(tcflag_t flags)
{
    termios oldattr;
    tcgetattr(STDIN_FILENO, &oldattr);

    termios newattr = oldattr;
    newattr.c_lflag &= ~flags;
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

    const int ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return ch;
}

/* reads from keypress, doesn't echo */
int _getch(void)
{
    return get_char(ICANON | ECHO);
}

/* reads from keypress, echoes */
int _getche(void)
{
    return get_char(ICANON);
}


int _kbhit(void)
{
    static bool initialized = false;

    if (!initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting = 0;
    ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

#if 0
int main(int argc, char** argv)
{
    printf("Press any key");
    while (!kbhit()) {
        printf(".");
        fflush(stdout);
        usleep(10000);
    }
    printf("\nDone.\n");

    return 0;
}
#endif

#endif // __unix__
