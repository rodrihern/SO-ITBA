#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int pipefd[2];  // File descriptors para el pipe
    pid_t pid1, pid2;

    // Crear el pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) { // child 1
        close(pipefd[0]); // cerramos el extremo de lectura
        
        // Redirigir STDOUT al extremo de escritura del pipe
        dup2(pipefd[1], STDOUT_FILENO); // le asigna al valor stdout=1 lo mismo que en pipefd[1]
        close(pipefd[1]); 
        
        // Ejecutar el programa 'p'
        char *argv[] = {"p", NULL};
        char *envp[] = {NULL};
        execve("./p", argv, envp);
        perror("execve p");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) { // child 2
        close(pipefd[1]);
        
        // Redirigir STDIN al extremo de lectura del pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        
        // Ejecutar el programa 'c'
        char *argv[] = {"c", NULL};
        char *envp[] = {NULL};
        execve("./c", argv, envp);
        perror("execve c");
        exit(EXIT_FAILURE);
    }

    // Proceso padre: cerrar ambos extremos del pipe
    close(pipefd[0]);
    close(pipefd[1]);

    // Esperar a que terminen ambos procesos hijos
    wait(NULL);
    wait(NULL);

    

    printf("Ambos procesos terminaron\n");
    return 0;
}