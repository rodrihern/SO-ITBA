# Ejercicio Fumadores

## Enunciado

El problema de los fumadores de cigarrillos (Patil 1971). Considere un sistema con tres procesos fumadores y un proceso agente. Cada fumador está continuamente armando y fumando cigarrillos. Sin embargo, para armar un cigarrillo, el fumador necesita tres ingredientes: tabaco, papel y fósforos. Uno de los procesos fumadores tiene papel, otro tiene el tabaco y el tercero los fósforos. El agente tiene una cantidad infinita de los tres materiales. El agente coloca dos de los ingredientes sobre la mesa. El fumador que tiene el ingrediente restante armaría un cigarrillo y se lo fuma, avisando al agente cuando termina. Entonces, el agente coloca dos de los tres ingredientes y se repite el ciclo. Escriba un programa para sincronizar al agente y los fumadores utilizando semáforos.

## Mi solucion

Si el agente sabe a quien le corresponde cada par de ingredientes

```c
#define SMOKERS 3

sem_t ingredientes_listos[SMOKERS] = { 0 }
sem_t ingredientes_counsumidos = 0


void agente() {
    while(1) {
        for (int i = 0; i < SMOKERS; i++) {
            poner_ingredientes(i);
            post(ingredientes_listos[i])
            wait(ingredientes_consumidos)
        }
    }
}

void fumador(int idx) {
    while (1) {
        wait(ingredientes_listos[idx])
        armar()
        post(ingredientes_consumidos)
        fumar()
    }
}
```


Si el agente **no** sabe a quien le corresponde cada par de ingredientes

```c
#define SMOKERS 3

sem_t ingredientes_listos[SMOKERS] = { 0 }
sem_t ingredientes_counsumidos = 0


void agente() {
    while(1) {
        poner_ingredientes()
        for (int i = 0; i < SMOKERS; i++) {
            post(ingredientes_listos[i])
        }
        wait(ingredientes_consumidos)
    }
}

void fumador(int idx) {
    while (1) {
        wait(ingredientes_listos[idx])
        if (ingredientes_que_necesito()) {
            armar()
            post(ingredientes_consumidos)
            fumar()
        }     
    }
}
```

En este igual no se bien que pasaria si por una race condition le hace muchas veces post al semaforo del mismo fumador sin que este lo consuma. Creo que no habria problema pero no estoy 100% seguro