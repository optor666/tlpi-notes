/*
 * Listing 62-4: Demonstrating cbreak and raw modes
 */
#include <termios.h>
#include <signal.h>
#include <ctype.h>
#include "tty_functions.h" 

#include "tlpi_hdr.h"

// Terminal settings as defined by user
static struct termios userTermios;

int main(int argc, char *argv[]) {
    char ch;
    struct sigaction sa, prev;
    ssize_t n;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (argc > 1) { // Use cbreak mode
        if (ttySetCbreak(STDIN_FILENO, &userTermios) == -1)
            errExit("ttySetCbreak");
    } else { // Use raw mode
        if (ttySetRaw(STDIN_FILENO, &userTermios) == -1)
            errExit("ttySetRaw");
    }

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &userTermios) == -1)
        errExit("tcsetattr");
    exit(EXIT_SUCCESS);
}
