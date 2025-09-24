# Threads

## Conceptos básicos
- Un **proceso** tiene su espacio de direcciones y al menos un **thread**.
- Varios threads dentro de un proceso comparten espacio de direcciones → se ejecutan en paralelo/pseudo-paralelo.
- Los threads son **más baratos** que procesos:
  - Creación / destrucción: 10–100 veces más rápido.
  - Switching más liviano.

## Justificación de uso
- Permiten manejar múltiples actividades simultáneas (input, autosave, GUI, etc.).
- Mejoran el uso del CPU, simplifican la programación.
- Aprovechan arquitecturas multicore.

## Ejemplos
- **Procesador de texto**: corrector ortográfico, auto-guardado, renderizado.
- **Servidor web**: múltiples conexiones simultáneas, búsquedas en disco (bloqueantes), envío de páginas en caché.

## Modelo de threads
- **Proceso**: agrupa recursos (memoria, archivos, señales).
- **Thread**: entidad schedulada en CPU → unidad de ejecución dentro del proceso.
- Threads = *lightweight processes*.
- **Problema**: comparten memoria → condiciones de carrera si no hay sincronización.

## Implementaciones
1. **Espacio de usuario**:
   - Kernel no los ve → solo ve un proceso.
   - Switching rápido, controlado por librería.
   - Desventajas:
     - Syscalls bloqueantes → bloquean todo el proceso.
     - Page faults bloquean todo.
     - Riesgo de inanición.

2. **Espacio de kernel**:
   - Kernel conoce cada thread.
   - Un bloqueo afecta solo a ese thread.
   - Mayor costo de creación/cambio.

3. **Modelo híbrido**:
   - Kernel maneja *kernel-threads*.
   - Librerías mapean múltiples *user-threads* a cada *kernel-thread*.
   - Ejemplo: *scheduler activations*.

---

# Ejercicio 1
**Enunciado:** Escribir un programa con threads que tenga condición de carrera (ej. varios threads incrementando una variable sin sincronización). Luego resolver con semáforos.

## Ejemplo en C (condición de carrera)
```c
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

int counter = 0;

void* increment(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        counter++;
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);


    printf("Counter = %d\n", counter);
    return 0;
}

```



## Ejemplo corregido con semaforos

```c
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/semaphore.h>

int counter = 0;
sem_t mutex;

void* increment(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        sem_wait(&mutex);
        counter++;
        sem_post(&mutex)
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    sem_init(&mutex, 0, 1);

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&mutex);
    printf("Counter = %d\n", counter);
    return 0;
}

```

Ejemplo de salida de ambos programas

```bash
# Sin sincronizacion
root@a035b5c72377:~/clase/practica# gcc thread_race_condition.c
root@a035b5c72377:~/clase/practica# ./a.out
Counter = 989397

# Con sincronizacion
root@a035b5c72377:~/clase/practica# gcc thread_race_condition.c
root@a035b5c72377:~/clase/practica# ./a.out
Counter = 2000000
```

