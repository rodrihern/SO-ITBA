# Ejercicio ascensor

## Enunciado

Un ascensor tiene una capacidad máxima de 6 personas y 450 kilogramos.

Se quiere simular el comportamiento del ascensor mediante un conjunto de hilos que actúan como personas que entran y salen del aparato. Para ello, tiene usted que implementar dos operaciones, sube_persona(peso) y baja_persona(peso).

Estas dos operaciones serán invocadas por los hilos que simulan las personas.

La operación de subir debe dejar bloqueado al proceso que hace la llamada mientras el ascensor esté lleno, o bien la carga en kilos no permita que el proceso se monte en el ascensor.

La operación de bajar debe actualizar el estado del ascensor y, si es el caso, inducir el desbloqueo de otras personas que puedan estar esperando para entrar.

TAREA. Debe usted implementar mediante semáforos las acciones de sincronización de estas operaciones de subir y bajar. Introduzca las variables auxiliares que considere necesarias. No hay que implementar el sistema completo, ni los hilos: sólo el código de sincronización de estas dos operaciones.

## Posibles soluciones


Solucion de korman. Puede haber inanicion onda si hay un gordo de 400 kilos y muchas personas de 60 kilos que se van subiendo y bajando pero nunca dejando al ascensor vacio, o incluso dejandolo vacio pero con condiciones de carrera, el gordo no sube nunca

```c
#define MAX_PESO 450
#define MAX_PERSONAS 6

sem_t mutex = 1;
sem_t espacio = 0;
int peso_total = 0;
int personas = 0;

void sube_persona(int peso) {
    sem_wait(&mutex);
    while (personas >= MAX_PERSONAS || peso_total + peso > MAX_PESO) {
        sem_post(&mutex);
        sem_wait(&espacio);
        sem_wait(&mutex);
    }
    peso_total += peso;
    personas += 1;
    sem_post(&mutex);
}

void baja_persona(int peso) {
    sem_wait(&mutex);
    peso_total -= peso;
    personas -= 1;
    sem_post(&mutex);
    sem_post(&espacio);
}
```

Solucion de branda en la que si hay un gordo que quiere subir y no puede, nadie mas puede subir hasta que el gordo no sube

```c
int sem_mutex_access=1;
int sem_someone_waiting=1;
int sem_maximo_personas=6;
int peso_actual=0;
#define PESO_MAXIMO 450

sube_persona(peso) {
    down(sem_someone_waiting); // si hay una persona intentando entrar se bloquea
    down(sem_maximo_personas);
    while(peso_actual + peso > PESO_MAXIMO){ // itera siempre que no pueda entrar
        down(sem_mutex_access);
    }
    entro(); // no importa que hace
    peso_actual+=peso;
    up(sem_someone_waiting); // libero a la persona que estaba esperando
    up(sem_mutex_access); // permite que se meta otro que recalcule el peso
}

baja_persona(peso) {
    peso_actual-=peso;
    salgo(); // no importa que hace
    up(sem_maximo_personas); 
    up(sem_mutex_access); // permito que se meta otro o que recalcule el peso
}

```

