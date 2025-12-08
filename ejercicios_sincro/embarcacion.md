# Ejercicio de embarcaciones

## Enunciado

El ITBA adquirió una embarcación autopilotada y desea sacar a pasear a sus alumnos por las turquesas aguas de Puerto Madero. Para ello se solicita proveer código tanto para la embarcación como para los alumnos para sincronizar el embarque.

Los alumnos llegan al puerto y esperan a la embarcación. Cuando llega, todos embarcan (ejecutan `embarcar()`), pero cualquier alumno que llegue durante el embarque deberá esperar la próxima vuelta. La capacidad de la embarcación es de 20 personas; si hay más de 20 personas esperando, algunos tendrán que esperar la próxima vuelta. Una vez que todos los alumnos embarcaron, la embarcación parte (ejecuta `partir()`). Si no hay alumnos esperando cuando llega la embarcación, esta parte inmediatamente.

- `embarcar()` y `partir()` no se pueden modificar ni se puede asumir que modifican variables creadas por ustedes.
- No es necesario que los alumnos ejecuten `embarcar()` concurrentemente (de hecho, la embarcación tiene solo 1 escotilla).
- No es necesario modelar el desembarque, asuma que una vez que parte, la embarcación vuelve vacía al puerto.
- No es necesario modelar la creación de procesos / threads, un entry point, API de threads, liberación de recursos, etc. Enfoquémonos en lo importante.
- No puede haber calls entre alumnos y embarcación. Son procesos separados.
- La solución deberá estar libre de deadlock, condiciones de carrera, inanición y busy wait.

El código (sin sincronizar) que ejecuta cada alumno es el siguiente:

```c
while (1)
    embarcar();
```

El código (sin sincronizar) que ejecuta la embarcación es el siguiente:

```c
while (1)
    partir();
```

Complete el código con las primitivas de sincronización y/o variables que considere necesarias para resolver las restricciones establecidas.

Para una devolución más clara se recomienda agregar números de línea a la solución, incluso espacios en blanco.



## Mi solucion

```c
#define MAX_STUDENTS 20


sem_t can_board = 0;
sem_t boarded = 0;
sem_t line = MAX_STUDENTS;

sem_t waiting_in_line_mutex = 0;
int waiting_in_line = 0;

student() {
    wait(line);

    wait(waiting_in_line_mutex);
    waiting_in_line++;
    post(waiting_in_line_mutex);

    wait(can_board);
    embarcar();
    post(boarded);
    
}

boat() {

    wait(waiting_in_line_mutex); // funciona tambien como torniquete
    for (int i = 0; waiting_in_line-- > 0; i++) {
        post(can_board);
        wait(boarded);
        post(line`);
    }
    post(waiting_in_line_mutex);

    partir();
}

```


