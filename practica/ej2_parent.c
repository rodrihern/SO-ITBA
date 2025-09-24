#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    for (int i = 0; i < 10; i++) {
        if (fork() == 0) {
            char *args[] = {"./child", NULL};
            execve("./child", args, NULL);
            // If execve fails
            perror("execve failed");
            _exit(1);
        }
    }
    
    for (int i = 0; i < 10; i++) {
        wait(NULL);
        printf("TERMINO %d\n", i);
    }

    return 0;
}