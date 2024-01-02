/*
 * 62-2. Implement ttyname(). 
 *
 * #include <unistd.h>
 *
 * int ttyname(int fd);
 *              Returns pointer to (statically allocated) string containing terminal name, or NULL on error
 */
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
/*
 * perror -> stdio.h
 * errno -> errno.h
 * STDIN_FILENO -> unistd.h
 * open -> fcntl.h
 */

int custom_isatty(int fd) {
    struct termios tp; 

    if (tcgetattr(fd, &tp) == -1) {
        if (errno != ENOTTY) {
            perror("tcgetattr failed with unknown error");
        }
        return 0;
    }

    return 1;
}


int main(int argc, char *argv[]) {
    if (custom_isatty(STDIN_FILENO)) {
        printf("Standard input is a terminal.\n");    
    } else {
        printf("Standard input is not a terminal.\n");    
    }

    if (custom_isatty(STDOUT_FILENO)) {
        printf("Standard output is a terminal.\n");    
    } else {
        printf("Standard output is not a terminal.\n");    
    }

    if (custom_isatty(STDERR_FILENO)) {
        printf("Standard error is a terminal.\n");    
    } else {
        printf("Standard error is not a terminal.\n");    
    }
    
    int fd = open("/tmp/test.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error open /tmp/test.txt");
    } else {
        if (custom_isatty(fd)) {
            printf("/tmp/test.txt is a terminal.\n");
        } else {
            printf("/tmp/test.txt is not a terminal.\n");
        }
        close(fd);
    }

    return 0;
}
