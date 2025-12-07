# Ejercicio de gatos y ratones

## Enunciado

Es como el de readers y writer pero con un maximo de readers.
En este caso writer seria el gato (los ratones no quieren estar cuando esta el gato) y los ratones pueden estar mientras estan otros ratones pero hasta cierta cantidad

## Posible solucion

propuesta por branda en el foro y a ariel le gusto

```c
#define MAX_RATONES 3

int sem_cant_ratones = MAX_RATONES;
int sem_mutex = 1;
int sem_gato_ready = 1;
int sem_count_ratones = 1;
int count_ratones = 0;

void raton(){
    while(1){
        wait(sem_gato_ready); // si hay un gato esperando, el raton espera
        post(sem_gato_ready); // liberarlo lo antes posible

        wait(sem_count_ratones);
        if(count_ratones++ == 0) wait(sem_mutex); // entra el primer raton y que no entre ningun gato
        post(sem_count_ratones);

        wait(sem_cant_ratones); // permite que entren hasta 3 ratones

        entrar_cocina();

        post(sem_cant_ratones); // libera el lugar para que entre otro raton

        wait(sem_count_ratones);
        if(--count_ratones == 0) post(sem_mutex);
        post(sem_count_ratones);
    }
}

void gato(){
    while(1){
        wait(sem_gato_ready);
        wait(sem_mutex);
        post(sem_gato_ready);  // liberarlo lo antes posible

        entrar_cocina();

        post(sem_mutex);
    }
}

```