# Memoria

## Memoria virtual

### Que pasa si no la tenemos?

es todo un quilombo, race conditions, si tengo 2 procesos corriendo y ambos quieren usar la direccion 0x4000 cagaste. Todas las soluciones que no usan memoria virtual estan flojas de papeles.

Tambien puede haber procesos traviesos que quieran leer la memoria de otro y ni pinta

Ademas tipo no permite hacer cosas como ejecutar programas que requieran mas memoria que la disponible, con memoria virtual podemos hacer locuras que estan buenas como hacer swapping

aparte habria que usar direcciones relativas a donde esta cargado el programa porque de una ejecucion del programa a otra este podria cargarse en un lugar distinto

muy dificil evitar que la memoria se fragmente

### Espacio de direcciones

El EDV se le presenta al proceso como un bloque contiguo y exclusivo de memoria y por atras el SO lo mapea como quiere, el EDF no es visible para el proceso.

Cada acceso a memoria es supervisado por la MMU (cacho de hardware), cuando hay un problema se lo notifica al SO mediante una excepcion (como page fault)

Este manejo tambien permite cargar en memoria cosas como librerias una sola vez, y que si hay 2 procesos que la acceden, redirigirlos ambos al mismo lugar. En caso de tener 2 procesos apuntando al mismo lugar se puede hacer un copy on write si un proceso quiere modificar algo

Partes fragmentadas del EDF pueden mapearse en el EDV de manera contigua, entonces asi el proceso puede hacer arrays brutalmente largos sin problema

### Y si me quedo sin memoria?

swapping

## Paginacion

seria la unidad minima de memoria, serian tipo como bloques. 

en el EDV se llaman paginas virtuales (VP) y en EDF marcos de pagina (PF)

El mapeo es el mismo que en arqui, lo de las tablas de paginas que te redirigen

### Entrada de la tabla

Tiene cosas como:
* PF number
* present
* RWX y demas permisos como el privilige level
* Modified $\rightarrow$ *no* se resetea porque se puede usar para saber si tiene que reescribirla o no en disco cuando hace el swapping
* Referenced $\rightarrow$ se resetea cada cierto tiempo



### Page Fault

Puede saltar por permisos o si no esta presente

cuando se quiere referenciar una pagina que no esta present, seguramente se la bajo a disco, o hay que mapearla

Si es por violacion de permisos se violan permisos se mata al proceso porque seguramente estaba queriendo hacer una chanchada

## TLB: Translation lookaside buffer

Las tablas de paginas estan en memoria, lo cual hace que cada vez que tenga que traducir una direccion tengo que ir a la memoria (not good)

Gracias a dios existe la TLB que es como una cache tipo para cachear las traducciones y evitar ir a la memoria para traducir la direccion cada vez

como las tablas de pagina son unicas por cada proceso, la TLB se invalida con cada context switch

## Administrar memoria libre

### Bitmap 

Un mapa de bits de si el bloque esta en uso o no

### Freelist

Cada proceso y espacio libre tienen un nodo con inicio, longitud y puntero al siguiente

Es necesario fusionar bloques libres adyacentes

## Criterios para asignar bloques

Se tiene en cuenta la *velocidad* y la *fragmentacion*

* First fit: el primer bloque suficientemente grande
* Next fit: Desde donde nos quedamos, el proximo suficientemente grande
* Best fit: El bloque mas pequeño suficientemente grande
* Worst fit: El bloque mas grande que haya (minimizar fragmentacion)
* Quick fit: Tenemos varias listas de bloques de tamaños frecuentes y agarramos uno de la lista correspondiente

## Tablas multinivel

A mayor EDV, mayor es la tabla de paginas (una entrada por cada direccion virtual)

Se hace muy grande una tabla si el EDV es muy grande, si a eso le sumamos que cada proceso tiene su propia tabla, es mucha memoria. Por eso existe lo de las tablas multinivel

La idea de la tabla multinivel es evitar tener toda la tabla en memoria, solo lo necesario.

el ejemplo que vimos en arqui es del procesador 80386 con el esquema de $2^{10} \times 2^{10} \times 2^{12} $ page directory -> page table

luego las entradas de las tablas pasaron a ser de 64 bits y pasaron a un esquema 
$ 2^9 \times 2^9 \times 2^2 \times 2^{12} $

y asi fue evolucionando hasta ahora que hay hasta nivel 5 (PML5) que puede direccionar hasta 57 bits
$  2^9 \times 2^9 \times  2^9 \times 2^9 \times 2^9 \times 2^{12} $

## Tablas de paginas invertidas

Tengo una entrada por cada pagina fisica (ocupa menos lugar)

hay que ciclar toda la tabla para ver a cual corresponde una entrada p, despues una vez esta en la TLB ya esta, y se hace con hardware esto no con software, lo cual lo hace mas rapido. Igual es una cagada y se usan las multinivel

Se puede hace una hash table con una lista para las colisiones para no tener que ciclar toda la tabla en memoria pero sigue siendo peor que usar multinivel

## Algoritmos de remplazo de paginas

### Optimo

No existe

Pero se puede pensar en un optimo como cota superior

### NRU

Usa los bits R y M

se van en el siguiente orden

R | M
--|--
0 | 0
0 | 1
1 | 0
1 | 1

Es preferible sacar una no modificada para no tener que reescribirla en disco

### FIFO

pretty much self explainatory tbh

Se desaloja la pagina mas antigua en ser cargada

### SC: Second chance

Primero mapeado con el bit R en 0

voy recorriendo las mas viejas y aquellas que tienen el bit R en 1 les doy una second chance, le apago el bit R y la mando al final de la cola, como si fuese la mas nueva (pensando en que tengo una cola con las mas viejas en la cabeza)

### Clock

En escencia es el mismo algoritmo que SC pero con una implementacion con lista circular

Te ahorra tener que sacar y volver a insertar al final, con solo resetear el bit R estas


### LRU

Desalojar la pagina que no ha sido usada el mayor tiempo (requiere de tener una variable de el ultimo tick en la que se la referencio)

### NFU

Un contador para cada pagina cada vez que se referencia, se va la que tiene el contador mas chico

