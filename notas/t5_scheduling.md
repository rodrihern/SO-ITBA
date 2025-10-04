## ¿Qué compone el contexto de un proceso?
### Contexto de hardware
Es la "foto" instantánea de la CPU en el momento en que el proceso deja de ejecutarse.
- **Contador de Programa (PC)**: la dirección de la próxima instrucción a ejecutar
- **Registros de la CPU**: Todos loa valores almacenados en los registros del procesador
- **Puntero de Pila (Stack Pointer)**: dirección actual en la pila del proceso

## Contexto de Espacio de memoria: 
Define el espacio de direcciones del proceso
- El **código** del programa (segmento de texto)
- **Datos globales** (segmentos de datos)
- La **pila** (stack) para las variables locales y llamadas a funciones
- El **heap** para memoria dinámica
- **Tablas de páginas o segmentos** que usa el sistema para traducer direcciones virtuales a físicas

### Contexto del Sistema Operativo
Información administrativa que el kernel utiliza para gestionar el proceso
- **Estado del proceso** (ready, running, blocked)
- **Identificador del proceso** (pid)
- **Información de scheduling**: prioridad, tiempo de CPU consumido
- **Información de E/S**: lista de archivos abiertos, dispositivos en uso
- **Credenciales**: propietario del proceso (UID, GID)

## ¿Qué sucede cuando el proceso no se está ejecutando?
Cuando no está en ejecución, su contexto se guarda en una estructura de datos del kernel llamada **Bloque de Control de Proceso** **(PCB)**.

El PCB actúa como un marcador. Cuando el so decide que el proceso debe volver a ejecutarse:
1. **Guarda** el contexto del proceso que está saliendo de la CPU en su respectivo PCB.
2. **Carga** el contexto del proceso que va a entrar desde su PCB hacia los registros de la CPU.

Este mecanismo también es conocido como **cambio de contexto** (contexto swtich), permite que múltiples procesos compartan una única CPU de forma concurrente, dando ilusión de que se ejecutan simultáneamente.

## Instrucciones
El control de la pila se gestiona con un puntero llamado **Stack Pointer (SP)**, que siempre apunta a la cima de la pila (último elemento).

### PUSH
1. **Incrementar el Puntero (SP)**: Primero, se hace espacio para le nuevo elemento moviendo el puntero a la siguiente posición disponible. En la mayoría de las arquitecturas, las pilas crecen hacia direcciones de memoria más bajas, por lo que este paso en realidad decrementa el valor del puntero.
2. **Guarda el Dato**: el dato que se quiere añadir una copia en la dirección de memoria a la que ahora apunta el SP.
> Mueve el puntero y luego se guarda el dato

### POP
1. **Leer el Dato**: se lee o copia el dato que se encuentra en la cima del stack.
2. **Decrementar el Puntero (SP)**: el puntero se mueve a al dirección anterior, "liberando" efectivamente el espacio y dejando el elemento anterior como la nueva cima. Si la pila crece hacia abajo, este paso incrementa el valor del puntero.
> Se lee el dato y se mueve el puntero

### CALL
1. **Guarda la Dirección de Retorno (RSP)**: hace un `push` de la **siguiente instrucción** en la pila. Esta es la dirección a la que se debe volver cuando termine la función llamada.
2. **Salta a la Función**: cambia el **Contador del Programa (PC)** a la dirección de la primera instrucción de la nueva función que se está llamando. Esto transfiere el control del programa a la nueva función.
> Guarda la posición actual y va a la función a ejecutarse

### RET
1. **Recupera la Dirección de Retorno (RSP)**: hace un `pop` del valor que está en la cima del stack y lo carga en el **Contador de Programa (PC)**.
2. **Continúa la Ejecución**: Como el PC ahora apunta a la instrucción que seguía al `call` original, el programa reanuda su ejecución justo donde se lo dejó.
> Inversa al call

## Interrupción e iret
Cuando una interrupción ocurre, el procesador detiene lo que está haciendo y realiza automáticamente los siguientes pasos:
1. **Termina la instrucción actual** con el objetivo de no dejar al sistema en un estado incosistente.
2. **Guarda el Estado del Proceso**: realiza un `push` al stack del **kernel**. A deferencia de `call`, no solo guarda la dirección de retorno (`CS`, `IP`/`EIP`), sino que también guarda el registro de **flags** (`EFLAGS`). Esto es crucial porque los flags contienen el estado del procesador (resultados de comparaciones, si las interrupciones estaban habilitadas, etc).
3. **Busca el Manejador** usando el número de la interrupción en la **IDT** y realiza lo indicado por la tabla para dicha interrupción. 

### iret
La instrucción `iret` se coloca al final de cada rutina de servicio de interrupción (ISR) y realiza el proceso inverso.
1. **Restaura el Estado**: hace `pop` de la pila del kernel para restaurar, en orden, la **dirección de retorno** y las **flags**.
2. **Reanudar la Ejecución**: al restaurar los registros, el procesador vuelve exactamente al estado en que se encontraba y reanuda la ejecución del programa que fue interrumpido.

#### Diferencia Clave con CALL y 
- `call` es una instrucción **síncrona** y **planificada** dentro del flujo de un programa. Por el contrario, una **interrupción** es un evento **impredecible**, iniciado por hardware externo.
- `iret` restaura el registro de `flags`, mientras que `ret` no lo hace. Esta es la diferencia técnica fundamental, ya que permite aislar completamente el contexto del manejador de la interrupción del contexto del programa interrumpido.

### Handlers Dummy
Explican de forma incremental cómo se debe construir una rutina de atención de interrupciones (ISR o handler).
### Handler Dummy
``` 
dummy_handler

; send EOI
mov al, 20h
out 20h, al

iretq
```
Tarea mínima indispensable.
- **Notificar que la interrupción** fue atendida. Solo envía la señal `EOI` (End of Interrupt) al controlador de interrupciones.
- Le dice al hardware "ya recibí tu señal". Si no se la envía, el sistema se colgaría porque no aceptaría más interrupciones de ese dispositivo.
- **La falla**: Este handler **no preserva el estado del proceso interrumpido**. Al ejecutar `iretq` sin haber guardado los registros, el proceso que se reanude lo hará con valores corruptos.
## Still Handler Dummy
```
still_dummy_handler:
pushState

; send EOI
mov al, 20h
out 20h, al

popState

iretq
```
**Preservación de Contexto**
- `pushState` justo al entrar guarda todos los **registros de propósito general** **(GPR)** en la pila, esto crea una "foto" completa del estado de la CPU antes de que el handler haga cualquier cosa.
- **Envía EOI**
- `popState` justo antes de salir, restaura todos los registros desde la pila.

**La mejora**: Este manejador sí es seguro. Aunque no hace nada "útil" (como cambiar de proceso), garantiza que el programa interrumpido pueda continuar su ejecución sin errores, como si nada hubiera pasado.

## Cambio de Contexto Real y Scheduler
El `interesting_handler` muestra cómo se realiza un **cambio de contexto** real entre procesos.

```
interesting_handler:
pushState

mov rid, rsp
call schedule
mov rsp, rax

; send EOI
mov al, 20h
out 20h, al

popState

iretq
```
1. **Guardar el Estado Actual**: Al igual que antes, el handler empieza con un `pushState` para guardar todos los registros del proceso actual (P1) en su pila.
2. **Guardar el Puntero a la Pila (RSP)**: El puntero de pila actual (`rsp`), que ahora apunta a la cima de la pila con todo el contexto guardado, se pasa como argumento a la función `schedule`. La función del scheduler es guardar este puntero en el **PCB (Process Control Block)** del proceso P1 y elegir el siguiente proceso a ejecutar (P0).
3. **Llamar al Scheduler**: La función `schedule` retorna el puntero de pila del proceso elegido (P0).
4. **Restaurar el Nuevo Estado**: El handler carga este nuevo puntero en el registro `rsp`. Ahora, la pila activa es la de P0. Al ejecutar `popState` y `iretq`, se restauran los registros y el flujo de ejecución de P0, continuando donde este último había sido interrumpido.

En esencia, el cambio de un proceso a otro se logra simplemente **intercambiando los punteros de pila** (`RSP`) a través del scheduler.

## Creación y Arranque de un Proceso
Para que un proceso nuevo pueda ser ejecutado por el scheduler, se debe crear un estado inicial para él de manera muy específica.
- **Concepto clave**: se "falsifica" el stack de un proceso nuevo para que sea vea **exactamente como si hubiera sido interrumpido por el timer justo antes de ejecutar su primera instrucción**.
	- **Implementación**: Se reserva memoria para su pila y se rellena manualmente con los valores iniciales de los registros (`RIP` apuntando a la primera instrucción del programa, `RFLAGS` con las interrupciones habilitadas, etc.).
	- **Arranque**: El proceso se añade a la lista de los listos (`READY`). Cuando el scheduler lo elija, cargará su puntero de pila y ejecutará `popState` y `iretq`, el proceso comenzará a ejecutarse "por primera vez" como si estuviera volviendo de una interrupción que nunca ocurrió.
## Syscalls
Las `syscalls` son la forma en que un programa de modo usuario solicita servicios de kernel. Funcionan como interrupciones por software.
- **Mecanismo**: una `syscall` (ej. `read`, `exit`, `yield` ) invoca un handler de interrupciones específico.
- **Diferencia clave**: durante una `syscall`, a menudo no se guarda `RAX`, ya que convencionalmente se usa para devolver el resultado de la llamada (por eso el handler usa `pushStateNoRax`)
- **Bloqueo**: si una `syscall` es **bloqueante** (como `read` esperando datos), el handler cambia el estado del proceso a `BLOCKED`, guarda su contexto y llama al scheduler (`int 0x20`) para que ejecute otro proceso. El proceso original no se da cuenta de que fue pausado y reanudado.

## Finalización de un Proceso
Un proceso no puede terminar simplemente ejecutando `ret` desde su función `main`.
- **Problema**: el `ret` intentaría hacer `pop` de una dirección de retorno que no existe en la pila, ya que el proceso no fue iniciado con una instrucción `call`. Esto provocaría un fallo.
- **Solución**: Un proceso debe terminar siempre llamando a la `syscall` `exit()`. El handler de `exit` se encarga de cambiar el estado del proceso a `EXITED`, liberar sus recursos y llamar al scheduler para ceder la CPU permanentemente.

## ¿Qué pasa si no  hay procesos listos? 
Si no hay ningún proceso en estado `READY` para ejecutar (por ejemplo, al inicio, cuando solo la `shell` está esperando un comando), el sistema no puede detenerse.
- **Proceso Idle**: El scheduler elige un **proceso especial de baja prioridad** (proceso `idle`).
- **Función**: La única tarea de este proceso es ejecutar la instrucción **`hlt`** (halt), que detiene la CPU hasta que llegue la próxima interrupción (como la del teclado o el timer). Esto evita que la CPU consuma energía innecesariamente corriendo en un bucle vacío.
	