# Ejercicio ascensor

## Enunciado

Un ascensor tiene una capacidad máxima de 6 personas y 450 kilogramos.

Se quiere simular el comportamiento del ascensor mediante un conjunto de hilos que actúan como personas que entran y salen del aparato. Para ello, tiene usted que implementar dos operaciones, sube_persona(peso) y baja_persona(peso).

Estas dos operaciones serán invocadas por los hilos que simulan las personas.

La operación de subir debe dejar bloqueado al proceso que hace la llamada mientras el ascensor esté lleno, o bien la carga en kilos no permita que el proceso se monte en el ascensor.

La operación de bajar debe actualizar el estado del ascensor y, si es el caso, inducir el desbloqueo de otras personas que puedan estar esperando para entrar.

TAREA. Debe usted implementar mediante semáforos las acciones de sincronización de estas operaciones de subir y bajar. Introduzca las variables auxiliares que considere necesarias. No hay que implementar el sistema completo, ni los hilos: sólo el código de sincronización de estas dos operaciones.

## Posibles solucion

En esta solucion, para evitar inanicion de gordos, hacemos que hasta que no se pueda subir uno, los demas tampoco se puedan subir con el lock de esperando_para_subir


```c
#define MAX_PESO 450
#define MAX_PERSONAS 6

sem_t esperando_para_subir = 1;
sem_t espacios_libres = MAX_PERSONAS
sem_t mutex_peso = 1;
int peso_ascensor = 0;

sube_persona(int peso) {
    wait(esperando_para_subir); // solo puede haber uno
    wait(espacios_libres); // tome mi lugar
    int espacios_esperados = 0;
    wait(mutex_peso); // me quiero fijar el peso
    while (peso_ascensor + peso > MAX_PESO) {
        post(mutex_peso);
        wait(espacios_libres);
        espacios_esperados++;
        wait(mutex_peso);
    }
    post(mutex_peso);

    // hago los post de los lugares libres
    while (espascios_esperados-- > 0) {
        post(espacios_libres);
    }

    subo(); // no importa que hace
    post(esperando_para_subir);
}

baja_persona(int peso) {
    wait(mutex_peso);
    peso_ascensor -= peso;
    post(mutex_peso);
    salgo(); // no importa que hace
    post(espacios_libres);
}



```

