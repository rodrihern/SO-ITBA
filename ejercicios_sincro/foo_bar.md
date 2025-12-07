# Ejercicio foo bar

## Enunciado

n procesos ejecutan el siguiente loop:

```c
while (1) {
  foo();
  bar();
}
```

Modifique el código de manera que ningún proceso ejecute `bar_i()` hasta que todos hayan ejecutado `foo_i()`, donde el `bar_i()` se refiere a la ejecución de `bar()` en la i-ésima iteración. Esto significa, por ejemplo, que no puedo tener a un proceso ejecutando `bar()` de la segunda iteración (`bar_2()`) mientras otro proceso ejecuta `bar()` de la primera iteración (`bar_1()`).

- La variable n es visible desde todos los threads.
- `foo()` y `bar()` no se pueden modificar.
- `bar()` debe poder ejecutarse concurrente o paralelamente, es decir que forzar la ejecución secuencial de `bar()` se considera incorrecto.
- La solución deberá estar libre de deadlock e inanición.


## Mi solucion

Con agente

```c

#define N 5 // por ejemplo

sem_t foo_done = 0;
sem_t can_bar = 0;

agent() {
    while (1) {
        int foo_count = 0;
        while (foo_count < n) {
            wait(foo_done);
            foo_count++;
        }

        // despierto a los n procesos para que puedan hacer bar
        for (int i = 0; i < N; i++) {
            post(can_bar);
        }
    }
}


process() {
    while (1) {
        foo();
        post(foo_done);
        wait(can_bar);
        bar();
    }
}


```

Sin agente

```c
int n = 5;

sem_t can_bar = 0;
sem_t foo_count_mutex = 1;
int foo_count = 0;

process() {
    while (1) {
        foo();

        wait(foo_count_mutex);
        // si soy el ultimo tengo que habilitar a todos a poder hacer bar
        if (++foo_count == n) {
            while (foo_count-- > 0) {
                post(can_bar);
            }
        }
        post(foo_count_mutex);

        wait(can_bar);
        bar();
    }
}

```