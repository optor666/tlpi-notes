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

// General handler: restore tty settings and exit
static void handler(int sig) {
    if (tcsetattr(sTDIN_FILENO, SCSAFLUSH, &userTermios) == -1)
        errExit("tcsetattr");
    _exit(EXIT_SUCCESS);
}

// Handler for SIGTSTP
static void tstpHandler(int sig) {
    // To save our tty settings
    struct termios ourTermios;
    sigset_t tstpMask, prevMask;
    struct sigaction sa;
    int savedErrno;

    // We might change 'errno' here
    savedErrno = errno;

    // Save current terminal settings, restore terminal to
    // state at time of program startup
    if (tcgetattr(STDIN_FILENO, &ourTermios) == -1)
        errExit("tcgetattr");
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &userTermios) == -1)
        errExit("tcsetattr");

    // Set the disposition of SIGTSTP to the default, raise the signal
    // once more, and then unblock it so that we actually stop
    if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
        errExit("signal");
    raise(SIGTSTP);

    sigemptyset(&tstpMask);
    sigaddset(&tstpMask, SIGTSTP);
    if (sigprocmask(SIG_UNBLOCK, &tstpMask, &prevMask) == -1)
        errExit("sigprocmask");

    // Execution resumes here after SIGCONT
    // Reblock SIGTSTP
    if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
        errExit("sigprocmask");

    // Reestablish handler
    sigemptyset(&sa.sa_mask); 
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = tstpHandler;
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
        errExit("sigaction");

    // The user may have changed the terminal settings while we were
    // stopped; save the settings so we can restore them later
    if (tcgetattr(STDIN_FILENO, &userTermios) == -1)
        errExit("tcgetattr");

    // Restore our terminal settings
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ourTermios) == -1)
        errExit("tcsetattr");

    errno = savedErrno;
}

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
