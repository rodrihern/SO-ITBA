#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int fd = open("./hola.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
     if (fd == -1) {
        perror("open failed");
        exit(1);
    }
    
    
    if (fork() == 0) {
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2 failed");
            exit(1);
        }
        execl("./p", "p", NULL);
        perror("execl failed");
        exit(1);
    }

    wait(NULL);
    close(fd);
    
    return 0;
}