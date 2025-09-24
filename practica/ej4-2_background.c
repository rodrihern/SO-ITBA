#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Proceso hijo - ejecutar proceso p
        execl("./p", "p", NULL);
        perror("execl failed");
        exit(1);
    } else if (pid > 0) {
        // Proceso padre - no esperar al hijo (background)
        printf("Proceso hijo ejecutado en background con PID: %d\n", pid);
        printf("Proceso padre termina sin esperar\n");
    } else {
        perror("fork failed");
        exit(1);
    }
    
    return 0;
}