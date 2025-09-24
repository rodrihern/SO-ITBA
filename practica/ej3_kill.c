#include <unistd.h>
#include <stdio.h>

int main() {
    for (int i = 0; i < 5; i++) {
        if (fork() == 0) {
            _exit(0);
        }
    }

    fork();
    while(1) 
        ;

    return 0;
}
