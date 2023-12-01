#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 5

int main(int argc, char *argv[]) {
    char buf[BUF_SIZE];

    printf("Enter text: ");
    fflush(stdout);
    if (fgets(buf, BUF_SIZE, stdin) == NULL)
        printf("Got end-of-file/error on fgets()\n");
    else 
        printf("\nRead: %s", buf);

    exit(EXIT_SUCCESS);
}

