# 📌 Inter Process Communication (IPC) – Resumen

## Conceptos iniciales
- **Procesos independientes**: no se comunican, no comparten estado.  
- **Procesos cooperativos**: necesitan comunicarse.  
- **Motivaciones del IPC**:
  - Compartir información  
  - Acelerar cómputo (paralelismo)  
  - Modularidad (descomposición en procesos)  
  - Conveniencia para el diseño de sistemas

---

## Modelos de IPC
### 1. Memoria compartida
- Procesos acuerdan compartir una región de memoria física.  
- Una vez creada, **no requiere syscalls** para leer/escribir.  
- Organización de datos y sincronización queda a cargo de los procesos.  
- Problema: coherencia de caché en sistemas multicore.  
- **Ejemplos en UNIX**: `shm_open(3)`, `ftruncate(2)`, `mmap(2)`.  
- ⚠️ Importante: los punteros no son válidos entre procesos, deben usarse **offsets**.

### 2. Pasaje de mensajes
- Más simple en sistemas distribuidos.  
- Útil para volúmenes pequeños de información.  
- Requiere intervención del kernel.  
- **Operaciones básicas**:  
  - `send(message)`  
  - `receive(message)`  
- Puede ser con **sincronización bloqueante/no bloqueante**:
  - `send bloqueante`: espera a que el receptor procese.  
  - `send no bloqueante`: retorna inmediatamente.  
  - `receive bloqueante`: espera un mensaje disponible.  
  - `receive no bloqueante`: retorna null si no hay mensaje.  
- **Buffering**:
  - Capacidad 0 → `send` siempre bloquea.  
  - Capacidad acotada → bloquea si el buffer está lleno.  
  - Capacidad no acotada → `send` nunca bloquea.

---

## Mecanismos de IPC

### Pipes
- Comunicación bajo esquema productor–consumidor.  
- Uno de los primeros mecanismos UNIX.  
- **Tipos**:
  - **Anónimos**: solo entre procesos relacionados (padre–hijo). Se crean con `pipe(2)` y se heredan con `fork/execve`.  
  - **Nombrados (FIFOs)**: permiten comunicación entre procesos arbitrarios. Se crean con `mkfifo(3)` y se abren con `open(2)`.  
- Sincronización implícita: `send` bloquea si está lleno, `receive` bloquea si está vacío.  
- Identidad:
  - Pipes anónimos → file descriptors  
  - Pipes nombrados → path en el filesystem  
- Comparación con files: los pipes permiten **ejecución paralela y sincronización automática**.

### Sockets
- Comunicación **bidireccional full-duplex**.  
- Pueden funcionar localmente (Unix Domain) o en red (TCP/UDP).  
- Identificación: IP + puerto.  
- Modelo típico: cliente–servidor.  
- En UNIX: `socket(2)`, `bind(2)`, `listen(2)`, `accept(2)`, `send(2)`, `recv(2)`.  
- Comparación con FIFOs:
  - Un socket permite múltiples clientes en paralelo.  
  - Para FIFOs, se necesitan 2 por cliente (uno para cada dirección).

---

## Señales
- Son **asincrónicas**: pueden ocurrir en cualquier momento.  
- Se instalan handlers con syscalls (`signal`, `sigaction`).  
- No todas las señales son interceptables (ej. `SIGKILL`).  
- El kernel altera el stack del proceso para que ejecute el handler y luego lo restaura con `sigreturn`.  
- Restricciones:
  - Usar solo funciones **async-signal-safe**.  
  - Usar variables globales simples (ej. `volatile sig_atomic_t`) para comunicar que ocurrió la señal.  

---

## Condiciones de carrera
- Ocurren cuando el resultado depende del **orden de ejecución** entre procesos/hilos.  
- Ejemplo: home banking debitando dos veces un saldo.  
- Solución: **exclusión mutua**.  

### Región crítica
- Sección de código que accede a recursos compartidos.  
- Debe protegerse con mecanismos de sincronización (ej. semáforos, mutex).

---

## Semáforos
- Propuestos por Dijkstra (1965).  
- Representan un contador de recursos (`wakeups disponibles`).  
- Operaciones:
  - `down (P)` → decrementa o bloquea si vale 0.  
  - `up (V)` → incrementa y desbloquea a un proceso si corresponde.  
- Usos:
  - **Contadores** (semáforos con valor >1).  
  - **Mutex** (inicialización en 1 → acceso exclusivo).  
- Deben tener **nombres significativos** para la comprensión del código.

---

## Deadlock
- Un conjunto de procesos está en deadlock si **todos esperan un evento que solo otro del mismo conjunto puede causar**.  
- Ejemplos:
  - Un proceso hace `down(mutex)` dos veces sin liberar.  
  - Dos procesos bloqueados esperando recursos en orden invertido.  

---

## Problemas clásicos de IPC
1. **Filósofos comensales**: sincronización de acceso a tenedores → puede haber *deadlock* o *starvation*.  
2. **Lectores y escritores**: balancear acceso concurrente a BD → la versión básica favorece a lectores.  
3. **Baño unisex**: exclusión por género, solo un grupo puede estar en la sección crítica.  

---

## ✅ En resumen
- **IPC** tiene dos enfoques principales: memoria compartida y pasaje de mensajes.  
- Cada mecanismo (pipes, sockets, shm) tiene su **API y restricciones**.  
- Para concurrencia segura: **regiones críticas + sincronización (semáforos, mutex)**.  
- Problemas clásicos ilustran *race conditions*, *deadlocks* y *starvation*.  

## Patrones clasicos

### Patron de light-switch (con inanicion)

```c
typedef int semaphore;
semaphore mutex = 1;   // protege acceso a rc
semaphore db = 1;      // exclusión para acceso a la base
int rc = 0;            // número de lectores activos

void reader(void) {
    while (TRUE) {
        down(&mutex);
        rc = rc + 1;
        if (rc == 1) down(&db);   // primer lector bloquea escritores
        up(&mutex);

        read_data_base();         // SECCIÓN CRÍTICA de lectura

        down(&mutex);
        rc = rc - 1;
        if (rc == 0) up(&db);     // último lector libera escritores
        up(&mutex);
        
        use_data_read();          // fuera de la base
    }
}

void writer(void) {
    while (TRUE) {
        think_up_data();          
        down(&db);                // exclusión total
        write_data_base();        // SECCIÓN CRÍTICA de escritura
        up(&db);
    }
}

```

### Filosofos comensales (con inanicion)

```c
#define N 5
#define LEFT (i+N-1)%N
#define RIGHT (i+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

typedef int semaphore;
int state[N];
semaphore mutex = 1;    // exclusión mutua para región crítica
semaphore s[N];         // un semáforo por filósofo

void philosopher(int i) {
    while (TRUE) {
        think(); 
        take_forks(i);
        eat(); 
        put_forks(i);
    }
}

void take_forks(int i) {
    down(&mutex);            
    state[i] = HUNGRY;        
    test(i);                  
    up(&mutex);               
    down(&s[i]);              // bloquea si no pudo adquirir los tenedores
}

void put_forks(int i) {
    down(&mutex);             
    state[i] = THINKING;      
    test(LEFT);               
    test(RIGHT);              
    up(&mutex);               
}

void test(int i) {
    if (state[i] == HUNGRY 
        && state[LEFT] != EATING 
        && state[RIGHT] != EATING) {
        state[i] = EATING;    
        up(&s[i]);            
    }
}

```


## Preguntas

### Ejercicio 1

```c
// nash bajo y dijo:

sem_t my_s;
int sem_value;

void my_init(int * s, int value) {
    *s = value;
}

void my_down(int * s) {
    sem_wait(&my_s);

    while(*s <= 0){
        sem_post(&my_s);
        //busy waiting
        sem_wait(&my_s);
    }
    (*s)--;
    sem_post(&my_s);
}

void my_up(int * s) {
    sem_wait(&my_s);
    (*s)++;
    sem_post(&my_s);
}

```

### Ejercicio 2

Una traza infinita en la que puede haber inanicion seria que
el 1 esta comiento, luego deja de comer y justo cuando va a 
hacer el test(2) el 3 hace test(3) primero y come
luego cuando se va a hacer el test(2) esta comiendo el 3
entonces da que no, y analogo para cuando suelta el 3

una forma seria hacer que haya un semaforo por cada tenedor y que cuando el tipo termina de pensar manotee un tenedor (o se quede bloqueado esperando ese tenedor) y que una vez lo tenga, agarra el otro (o se queda bloqueado esperandolo), de esa manera evitamos la inanicion. Para evitar deadlock, hacemos que los de indice par agarren primero el tenedor a su derecha y los de indice impar, el de su izquierda (o al reves, es lo mismo)

```c
#define N 5  // Número de filósofos

sem_t tenedores[N];  // Un semáforo por cada tenedor

void filosofo(int id) {

    while(TRUE) {
        if(id % 2 == 0) {
            //Toma el derecho primero
            down(&tenedores[(id + 1) % N]);
            take_fork();

            //Toma el izquierdo
            down(&tenedores[id]);
        } else {
            //Toma el izquierdo primero
            down(&tenedores[id]);

            //Toma el derecho
            down(&tenedores[(id + 1) % N]);
        }

        //Si tiene ambos come
        eat();

        //Una vez que termina de comer los libera
        up(&tenedores[id]);  // Soltar el tenedor izquierdo
        up(&tenedores[(id + 1) % N]);  // Soltar el tenedor derecho
    }
}

```

tambien podria pensarse de implementar una forma en la que 

### Ejercicio 3

hecho por la catedra con inanicion:

```c

sem_t toilet_mutex = 1;

sem_t men_count_mutex = 1;
int men_count = 0;

sem_t women_count_mutex = 1;
int women_count = 0;


// el codigo de la mujer es analogo

man() {
    while(1) {
        wait(men_count_mutex);
        if (men_count++ == 0)
            wait(toilet_mutex);
        post(men_count_mutex);
        
        use_toilet();
        
        wait(men_count_mutex);
        if (men_count_mutex-- == 1) 
            post(toilet_mutex);
        post(men_count_mutex);
        
        work();
    }
} 


```

### Ejercicio 4

en el segundo caso, como estan en background se paralelizan
si uno ponele que leyo el 0, se quedo mucho tiempo sin que lo atiendan

### Ejercicio 5

los 1 antes que los 2

```c

int a1_ready = 0
int b1_ready = 0

void A() {
	a1();
	post(a1_ready);
	wait(b1_ready);
	a2();
}

void B() {
	b1();
	post(b1_ready);
	wait(a1_ready);
	b2();
}

```


### Ejercicio 6

```c

sem_t writer_mutex = 1; // mutex para darle prioridad al writer
sem_t state_mutex = 1; // mutex para acceder al estado
sem_t reader_count_mutex = 1;
int reader_count = 0;

static inline void writer_enter(game_sync_t *s) {
    sem_wait(&s->writer_mutex);
    sem_wait(&s->state_mutex); 
}

static inline void writer_exit(game_sync_t *s) {
    sem_post(&s->state_mutex); 
    sem_post(&s->writer_mutex); 
}

static inline void reader_enter(game_sync_t *s) {
    sem_wait(&s->writer_mutex); 
    sem_wait(&s->reader_count_mutex); 
    if (++s->reader_count == 1)   // patron de light switch
        sem_wait(&s->state_mutex);
    sem_post(&s->reader_count_mutex); 
    sem_post(&s->writer_mutex); 
}

static inline void reader_exit(game_sync_t *s) {
    sem_wait(&s->reader_count_mutex);
    if (--s->reader_count == 0)  // patron de light switch
        sem_post(&s->state_mutex); 
    sem_post(&s->reader_count_mutex);
}
```