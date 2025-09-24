#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int status;
    
    // hay que checkear el caso de que fork devuelve -1
    // no lo hago ahora porque no quiero
    if (fork() == 0) {
        execl("./false", NULL);
        perror("execl failed");
        exit(1);
    }
    wait(&status);
    
    // Solo ejecutar el segundo proceso si el primero terminó con éxito (código 0)
    if (status != 0) {
        return 0;        
    }

    if (fork() == 0) {
        execl("./true", NULL);
        perror("execl failed");
        exit(1);
    }
        wait(NULL);
    
    return 0;
}