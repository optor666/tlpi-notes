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
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
/*
 * perror -> stdio.h
 * errno -> errno.h
 * STDIN_FILENO -> unistd.h
 * open -> fcntl.h
 */

#define _D_ALLOC_NAMLEN(d) (((char *) (d) + (d)->d_reclen) - &(d)->d_name[0])

static char* getttyname_name;

/*
 * static 修饰的函数为静态函数。
 * 静态函数的定义仅存在于文件作用域，多个文件可以存在同名的静态函数。
 */
static char* getttyname(int fd, dev_t mydev, ino_t myino, int save, int *dostat) {
    static const char dev[] = "/dev";
    static size_t namelen;
    struct stat st;
    DIR *dirstream;
    struct dirent *d;
    
    dirstream = opendir(dev);
    if (dirstream == NULL) {
        *dostat = -1;
        return NULL;
    }

    while ((d = readdir(dirstream)) != NULL) {
        if (
            ((ino_t) d->d_fileno == myino || *dostat)
                && strcmp(d->d_name, "stdin")
                && strcmp(d->d_name, "stdout")
                && strcmp(d->d_name, "stderr")
                ) {
            size_t dlen = _D_ALLOC_NAMLEN(d);
            if (sizeof(dev) + dlen > namelen) {
                free(getttyname_name);
                namelen = 2 * (sizeof(dev) + dlen); // Big enough.
                getttyname_name = malloc(namelen);
                if (!getttyname_name) {
                    *dostat = -1;
                    // Perhaps it helps to free the directory stream buffer.
                    closedir(dirstream);
                    return NULL;
                }
                memcpy(getttyname_name, dev, sizeof(dev) - 1);
                getttyname_name[sizeof(dev) - 1] = '/';
            }
            memcpy(&getttyname_name[sizeof(dev)], d->d_name, dlen);
            if (stat(getttyname_name, &st) == 0
                    && S_ISCHR(st.st_mode) && st.st_rdev == mydev
               ) {
                closedir(dirstream);
                errno = save;
                return getttyname_name;
            }
        }
    }

    closedir(dirstream);
    errno = save;
    return NULL;
}


/*
 * To find the name of the terminal, ttyname() uses the opendir() and readdir() functions
 * described in Section 18.8 to walk through the directories holding terminal 
 * device names, looking at each directory entry until it finds one whose device ID (the
 * st_rdev field of the stat structure) matches that of the device referred to by the file
 * descriptor fd. Terminal device entries normally reside in two directories: /dev and
 * /dev/pts. The /dev directory contains entries for virtual consoles (e.g., /dev/tty1)
 * and BSD pseudoterminals. The /dev/pts directory contains entries for (System V-style)
 * pseudoterminal slave devices. (We describe pseudoterminals in Chapter 64.)
 */
char* custom_ttyname(int fd) {
    struct stat st;
    int dostat = 0;
    char *name;
    int save = errno;

    if (!isatty(fd))
        return NULL;

    if (fstat(fd, &st) < 0) 
        return NULL;

    name = getttyname(fd, st.st_rdev, st.st_ino, save, &dostat);
    if (!name && dostat != -1) {
        dostat = 1;
        name = getttyname(fd, st.st_rdev, st.st_ino, save, &dostat);
    }

    return name;
}


int main(int argc, char *argv[]) {
    printf("Standard input ttyname is %s.\n", custom_ttyname(STDIN_FILENO));
    printf("Standard output ttyname is %s.\n", custom_ttyname(STDOUT_FILENO));
    printf("Standard error ttyname is %s.\n", custom_ttyname(STDERR_FILENO));

    return 0;
}
