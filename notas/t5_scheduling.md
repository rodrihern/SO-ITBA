# Scheduling

El mecanismo por el cual se elige que proceso va a correr

El scheduler **no** es un proceso

## Conceptos

*turnaround time*: tiempo desde que empieza un trabajo hasta que termina

*throughput*: cantidad de trabajos que se terminan por unidad de tiempo

Un buen algoritmo de scheduling busca:
1. minimizar el turnaround time promedio
2. maximizar el throughput

la teca esta en encontrar un balance entre ambas cosas

*CPU burst*: tiempo que el proceso usa el cpu antes de bloquarse

bursts largos $\rightarrow$ cpu bound

bursts cortos $\rightarrow$ I/O bound

*preemptive*: el tipo interrumpe a los procesos, el non-preemptive adivina que hace

*quantum*: tiempo que se le da de cpu a un proceso antes de interrumpirlo

*fairness*: darle a todos una fare share del cpu


## Categorias

### Batch

No hay interaccion con el usuario, puede ser non-preemptive o preemptive con quantum largo

### Interactivo

Es el de las compus para usuarios, como interactuan con usuarios tienen que ser preemptive

si hay un proceso que tarda mucho en bloquearse se pierde la sensacion de responsiveness

### Real time

No hace falta que sea preemptive pues tiene un uso especifico para programas especificos que cooperan entre si. Son sistemas donde no solo importa la correctitud de la respuesta sino el tiempo en el que se da (como el calculo de trayectoria de un misil que me va a hacer teta para tirar las bengalas y distraerlo)

## Algoritmos

### First Come First Served

non-preemptive

como su nombre lo indica, tenemos una cola fifo y los vamos ejecutando

puede pasar que primero llegue uno con un burst muy largo y despues muchos con bursts cortos y ahi el turnaround time promedio y se me hace muy grande, comparado a si atiendo primero a los que tienen burst cortos

### Shortest Job First

primero el mas corto maximizando el throughput y minimizando el turnaround time, la desventaja de este es el tema del responsiveness

### Shortest Remaining Time Next

igual al anterior pero preemptive

### Round-Robin

le asigna un quantum a cada proceso y lo deja correr ese tiempo y pasa al siguiente

### Priority Scheduling

se eligen aquellos con mas prioridad (mejora el responsiveness en comparacion al round robin) osea si yo estoy usando un programa ahora mismo, me interesa mucho mas que corra el programa a que corran otras cosas que no estoy usando

La prioridad se puede asignar estatica o dinamicamente

se puede hacer tambien con una multilevel queue teniendo una queue por prioridad

La prioridad tambien puede estar dada por *shortest process next* que se puede estimar con una tecnica llamada *aging*

$$
S_{n+1} = a \cdot S_n + (1 - a) \cdot T_n  
$$

S: estimacion de la duracion del burst

T: lo que realmente duro el burst


### Guaranteed scheduling

prometemos que si hay n usuarios conectados, cada uno recibe 1/n del tiempo de CPU, o lo mismo para procesos

Se fija cuanto tiempo uso respecto de la proporcion y elige el proceso con la menor proporcion

### Lotery Scheduling

Cada proceso tiene tickets de loteria para cierto recurso, cada cierto tiempo sorteamos a quien le toca y confiamos en los dioses de la probabilidad y la estadistica haciendo que el proceso con mas tickets termina corriendo mas

### Fair-Share Scheduling

si tenemos un usuario con 9 procesos pero otro con solo 1, un round robin normal es nefasto porque el que tiene un proceso solo le estamos dando mucho menos tiempo de cpu

Entonces vemos de darle a cada usuario un fare share,

Usuario 1: procesos A B C D

Usuario 2: proceso E

si le damos 50 / 50 a cada uno corre:

A E B E C E D E ...

sino tambien se pueden dar distintas proporciones como 67 / 33

A B E C D E ...




