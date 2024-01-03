/*
 * 62-3. Implement the getpass() function described in Section 8.5. 
 *  The getpass() function can obtain a file descriptor for the controlling terminal by opening /dev/tty.
 *  The getpass() function opens /dev/tty (the controlling terminal of the process), outputs the string prompt, turns off echoing,
 *   reads one line (the "password"), restores the terminal state and closes /dev/tty again.
 *
 * #include <unistd.h>
 *
 * char *getpass(const char *prompt);
 *              The function returns a pointer to a static buffer containing (the first PASS_MAX bytes of) the password without the 
 *              trailing newline, terminated by a null byte ('\0'). This buffer may be overwritten by a following call. On error, 
 *              the terminal state is restored, errno is set to indicate the error, and NULL is returned.
 * 
 */
#include <stdio.h>
#include <termios.h>

char* getpass(const char* prompt) {
    FILE *in, *out;
    struct termios s, t;
    int tty_changed;
    static char *buf;
    static size_t bufsize;
    ssize_t nread;

    // Try to write to and read from the terminal if we can.
    // If we can't open the terminal, use stderr and stdin.
    in = fopen("/dev/tty", "w+ce");
    if (in == NULL) {
        in = stdin;
        out = stderr;
    } else {
        // We do the locking ourselves.
        // in->_flags |= _IO_USER_LOCK;
        out = in;
    }

    // Make sure the stream we opened is closed even if the thread 
    // is canceled.
    flockfile(out);

    // Turn echoing off if it is on now.
    if (tcgetattr(fileno(in), &t) == 0) {
        // Save the old one.
        s = t;
        // Tricky, tricky.
        t.c_lflag &= ~(ECHO|ISIG);
        tty_changed = (tcsetattr(fileno(in), TCSAFLUSH | TCSASOFT, &t) == 0);
    } else {
        tty_changed = 0;
    } 

    // Write the prompt.
    fprintf(out, "%s", prompt);
    fflush(out);

    // Read the password.
    nread = getline(&buf, &bufsize, in);
    if (buf != NULL) {
        if (nread < 0) {
            buf[0] = '\0';
        } else if (buf[nread - 1] == '\n') {
            // Remove the newline.
            buf[nread - 1] = '\0';
            if (tty_changed) {
                // Write the newline that was not echoed.
                fprintf(out, "\n");
            }
        }
    }

    // Restore the original setting.
    if (tty_changed) {
        tcsetattr(fileno(in), TCSAFLUSH | TCSASOFT, &s);
    }

    funlockfile(out);

    if (in != stdin) 
        // We opened the terminal; now close it.
        fclose(in);

    return buf;
}

int main(int argc, char *argv[]) {
    char* password = getpass("Please input password: ");
    printf("Your password is %s.\n", password);

    return 0;
}
