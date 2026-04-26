---
materia: so
tipo: apuntes
---

# File system

Resuelve 3 problemas:
1. Almacenar mucha informacion
2. Persistencia de la informacion 
3. multiples procesos accediendo a la misma informacion (ver [[t3_ipc|IPC]] para sincronización entre procesos)

Se puede ver al disco como una secuencia lineal de bloques de tamaño fijo que se pueden leer/escribir

los files son abstracciones del disco

## Asignacion continua

mas simple de todos: pones archivos pegaditos uno al lado del otro

se usa en cds porque la lectura contigua es mas rapida y ahi ya se a priori el tamaño de las cosas

fragmentacion externa es un problemon cuando se van liberando bloques, porque necesitas bloques contiguos

## Asignacion enlazada

Cada bloque tiene un puntero al siguiente bloque

Estas comiendo parte del bloque para poner el puntero, lo que hace que no puedas guardar bien cosas de tamaño de potencias de 2.

la fragmentacion externa ya no es un problema pero si quiero ir al bloque 10 de un archivo tengo que visitar los 9 anteriores.

## Asignacion con listas enlazadas (en memoria)

file allocation table (FAT)

toda la tabla tiene que estar en memoria

## I-nodes

index node, cada archivo tiene un i-node que tiene atributos y la lista de bloques

solo se cargan en memoria los i-nodos de los files que se estan usando en ese momento

tiene:
* owner
* timestamps
* size
* block count
* direcciones, directas, simple indirecta, doble indirectas, triple indirectas ...

## Ejercicios

### Pregunta 1

**Describa los efectos de un bloque de datos corrupto para las siguientes implementaciones de archivos:**
* Asignacion contigua:
    lo unico que se guarda es principio y longitud, si se corrompen algunos bits de un bloque en particular, esos datos van a quedar mal pero se sigue pudiendo acceder con normalidad al resto de los bloques de ese archivo. queda basura dentro del archivo pero no se rompe la estructura ni se pierde el resto

* Lista enlazada en disco:
    Si se corrompe un bloque se puede perder el puntero al siguiente, se puede perder todo lo que viene despues o incluso acceder a otro bloque que nada que ver

* Lista enlazada en memoria:
    Como esta en memoria la lista de punteros, si se corrompe un bloque solo se pierde lo de ese bloque y no afecta al resto

### Pregunta 2

**Toda la información correspondiente a cada alumno del ITBA está almacenada en archivos (1 por alumno). Estos archivos son todos del mismo tamaño y son accedidos de forma aleatoria (random access). ¿Qué esquema de asignación es más conveniente y por qué?**
* Asignación contigua
* Lista enlazada en disco
* Lista enlazada en memoria

Y como son del mismo tamaño (y este no cambia) y el acceso es aleatorio, asignacion contigua es la que va (puedo acceder directo sin tener que recorrer muchos bloques o una lista enlazada)

### Pregunta 3

**Considere un disco que tiene 10 bloques de datos desde el bloque 14 hasta el bloque 23. Sean f1 y f2 2 archivos almacenados en el disco. La estructura de directorio especifica que el primer bloque de cada archivo es 22 y 16 respectivamente. Dada la siguiente tabla FAT ¿Cuáles son los bloques correspondientes a f1 y cuáles a f2?**

bloque | siguiente
-- | --
14 | 18
15 | 17
16 | 23
17 | 21
18 | 20
19 | 15
20 | -1
21 | -1
22 | 19
23 | 14

f1: 22 19 15 17 21

f2: 16 23 14 18 20

### Pregunta 4

**Un file system de UNIX tiene bloques de 4KB y direcciones de disco de 4 bytes. ¿Cuál es el tamaño máximo de archivo si los i-nodes tienen 10 entradas directas, 1 entrada simple indirecta, 1 entrada doble indirecta y 1 entrada triple indirecta?**

bloques de 4K, direcciones de 4B $\rightarrow 2^{12} / 2^2 = 2^{10}$ direcciones por bloque. Ver [[U1_codigo#Tamaño de Tipos de Datos|tamaños de tipos de datos]] para entender el tamaño de un puntero de 4 bytes en 32 bits.

10 entradas directas $\rightarrow 10$ bloques

1 simple indirecta $\rightarrow 2^{10}$ bloques

1 doble indirecta $\rightarrow 2^{20}$ bloques

1 triple indirecta $\rightarrow 2^{30}$ bloques

tamaño maximo: 

$$
(10 + 2^{10} + 2^{20} + 2^{30}) * 4K \approx  4TiB
$$

