/*
 * 62-4. Write a program that displays information indicating whether the terminal 
 *       referred to by standard input is in canonical or noncanonical mode. If in
 *       noncanonical mode, then display the values of TIME and MIN.
 */
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct termios tp;
    
    if (tcgetattr(STDIN_FILENO, &tp) == -1) {
        perror("tcgetattr");
        return 1;
    }

    if (tp.c_lflag & ICANON) {
        printf("Terminal is in canonical mode.\n");
    } else {
        printf("Terminal is in noncanonical mode.\n");
        printf("TIME value is: %d\n", tp.c_cc[VTIME]);
        printf("MIN value is: %d\n", tp.c_cc[VMIN]);
    }

    return 0;
}
