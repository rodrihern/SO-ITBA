# Enunciado

Este es el que tomaron en el primer parcial

Hay 3 recursos que son necesarios para vivir, hay 3 civiles y un dios pluto

Cada civil tiene infinito 1 recurso y necesita los otros dos, estos recursos son:
1. Dinero
2. Energia
3. Tiempo

pluto hace lo siguiente:

```c

sem_t pluto = 0;
sem_t dinero = 0;
sem_t energia = 0;
sem_t tiempo = 0;

pluto() {

    while(1) {
    
        switch (randInt() % 3) {
            case 0:
                provide(dinero); provide(energia);
                post(dinero); post(energia); break;
            case 1:
                provide(dinero); provide(tiempo);
                post(dinero); post(tiempo); break;
            case 0:
                provide(tiempo); provide(energia);
                post(tiempo); post(energia); break;
        }
    
        wait(pluto);
    }
}

```

pluto no se puede modificar, no se puede leer el valor de los semaforos, lo unico que se puede hacer con el semaforo es `wait()` y `post()`




## Mi solucion

Esta no fue mi solucion en el examen, no sabia que podia poner agentes. 

A una solucion parecida a esta le dijo que estaba correcta en el foro.

```c

typedef enum {
    DINERO = 0,
    ENERGIA, 
    TIEMPO
} recurso_t;

sem_t dinero_tiempo = 0;
sem_t dinero_energia = 0;
sem_t energia_tiempo = 0;

sem_t recursos_mutex = 1;
int recursos[3] = {0};

agente_dinero() {
    while (1) {
        wait(dinero);
        wait(recursos_mutex);
        if (recursos[ENERGIA]) {
            recursos[ENERGIA]--;
            post(dinero_energia);
        } else if (recursos[TIEMPO]) {
            recursos[TIEMPO]--;
            post(dinero_tiempo);
        } else {
            recursos[DINERO]++;
        }
        post(recursos_mutex);
    }
}

civil_energia_inf() {
    while(1) {
        wait(need_energia_tiempo); // consumi los recursos
        post(pluto); // liberar lo antes posible
        sobrevivir();
    }
}


// analogo para los otros agentes y civiles





```