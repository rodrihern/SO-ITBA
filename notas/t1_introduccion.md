# Introducción a Sistemas Operativos

## Kernel vs User Mode
- **Kernel mode**: el procesador puede ejecutar todas las instrucciones, incluso las privilegiadas (acceso directo a hardware, manejo de memoria, interrupciones).  
- **User mode**: ejecución restringida, los programas de usuario no pueden ejecutar instrucciones privilegiadas.  
- Solo el **sistema operativo** corre en modo kernel; el resto de los procesos corre en modo usuario y solicita servicios mediante **system calls**.



## Funciones principales de un SO
1. **Máquina extendida**: abstrae el hardware y ofrece una interfaz simple, elegante y consistente.  
   Ejemplo: drivers de disco, sistema de archivos, memoria virtual.  
2. **Administrador de recursos**: administra CPU, memoria, dispositivos, multiplexando en **tiempo** y **espacio**.  
   Ejemplo: coordinar dos procesos que quieren imprimir al mismo tiempo.


## Revisión de hardware

### Procesador
- **Ciclo de ejecución**: fetch → decode → execute → repeat.  
- **Registros**:
  - Generales y de propósito específico.
  - Especiales: Instruction Pointer (IP), Stack Pointer (SP), Program Status Word (PSW/RFLAGS).
- **Context switch**: guardar y restaurar el estado de un proceso o thread.
- **Pipeline**: solapa ejecución de instrucciones.
- **Superscalar**: múltiples instrucciones en paralelo.
- **Modos de ejecución**: user vs kernel.

### Multithreading y Multicore
- **Multithreading (hardware)**: un núcleo mantiene varios hilos listos e intercambia entre ellos rápidamente. No es paralelismo real.  
- **Multicore**: múltiples núcleos físicos independientes, que sí permiten paralelismo real.  
- **Ley de Moore**: crecimiento exponencial histórico de transistores.

### Memoria
- **Jerarquía**: registros → cache → RAM → disco.  
- Características ideales: rápida, enorme, barata (imposible simultáneamente).  
- **Cache**:
  - Decide qué guardar, dónde y qué reemplazar.
  - Se basa en **localidad temporal** (usar de nuevo lo mismo) y **espacial** (accesos contiguos).

### Disco
- **HDD**: cilindros, tracks, sectores. Acceso mecánico.  
- **SSD**: sin partes móviles, más rápido y confiable.  
- **Memoria virtual**: combina RAM y disco para dar la ilusión de más memoria disponible.

### I/O (Input/Output)
- **Componentes**: dispositivo + controlador.  
- **Modos**:
  - Busy waiting.
  - Interrupciones.
  - DMA (Direct Memory Access).  
- **Flujo típico**:
  1. Driver da orden al controlador.
  2. El dispositivo ejecuta.
  3. Controlador interrumpe al CPU al finalizar.
  4. El kernel atiende la interrupción.

### Booteo
1. La BIOS (firmware en la placa madre) corre al encender.  
2. Testea memoria y hardware.  
3. Escanea buses PCIe/PCI en busca de dispositivos.  
4. Determina dispositivo de booteo.  
5. Carga el primer sector en memoria y lo ejecuta.  
6. Se carga el sistema operativo.  


## Zoológico de Sistemas Operativos
- **Mainframe**: mucha memoria/almacenamiento, jobs por lotes, timesharing. Ej: OS/390.  
- **Server**: servidores de archivos, impresión, web. Ej: Solaris, FreeBSD, Linux, Windows Server.  
- **PC**: soporte a un usuario. Ej: Linux, Windows, macOS.  
- **Handheld**: tablets y smartphones. Ej: Android, iOS.  
- **Embedded**: firmware en electrodomésticos o routers. Ej: QNX, VxWorks.  
- **Sensor-node**: redes de sensores con CPU y RAM muy limitados. Ej: TinyOS.  
- **Tiempo real (RTOS)**: con deadlines estrictos (hard/soft). Ej: FreeRTOS, eCos.  
- **Smart Card**: sistemas extremadamente limitados (pagos electrónicos).  



## System Calls
- Son funciones que realizan un **TRAP** → transición de user mode a kernel mode.  
- Proveen acceso controlado a recursos del sistema.  
- En POSIX se exponen mediante librerías (`unistd.h` en C).  

### Ejemplo: `read(fd, buffer, nbytes)`
1. El caller pushea parámetros.  
2. Llama a `lib_read`.  
3. `lib_read` setea un registro con el número de syscall.  
4. Ejecuta instrucción TRAP (`int 0x80` / `syscall`).  
5. El dispatcher selecciona el handler en función del número.  
6. El handler ejecuta la acción (leer del fd) o bloquea el proceso.  
7. Retorno a user mode → `lib_read` devuelve al programa.  



## Estructuras de un SO
- **Monolítico**:
  - Todo el SO en un binario único.
  - Alta eficiencia, pero difícil de mantener.
  - Ej: Linux, Windows 95/98, FreeBSD.
- **Microkernel**:
  - Kernel mínimo (solo lo esencial).
  - Drivers y servicios en user mode.
  - Más confiable, más mensajes de IPC.
  - Ej: MINIX, QNX.
- **Reincarnation server (MINIX)**: monitoriza y reinicia drivers caídos.
- **Mecanismo vs política**:
  - Mecanismo en kernel: cómo ejecutar procesos.
  - Política afuera: qué prioridades asignar.  
- **Máquinas virtuales**:
  - **Hypervisor tipo 1**: sobre el hardware directamente (VMware ESXi).  
  - **Hypervisor tipo 2**: sobre un SO host (VirtualBox, QEMU).  
  - Virtual machines vs containers: nivel de aislamiento y performance.


## Preguntas y Respuestas

1. **Funciones principales de un SO**  
   - Administrar recursos (CPU, memoria, dispositivos).  
   - Proveer abstracción y protección entre procesos.  

2. **Ventajas de almacenar cache lines completas vs 1 byte**  
   - Se aprovecha la localidad espacial: se suelen usar datos contiguos.  
   - Reduce cache misses y mejora eficiencia al minimizar accesos a memoria.  

3. **¿Por qué las instrucciones de I/O suelen ser privilegiadas?**  
   - Seguridad: impedir que un proceso lea hardware sensible (ej: teclado con contraseñas).  
   - Aísla procesos maliciosos y protege el sistema.  

4. **¿Cómo facilita disponer de 2 modos (kernel/user)?**  
   - Separa operaciones críticas de las normales.  
   - Aumenta la seguridad y estabilidad: fallas de user mode no afectan al kernel.  

5. **Instrucciones permitidas solo en kernel mode**  
   - a. Deshabilitar interrupciones ✔️  
   - b. Leer la hora ✖️ (puede ser user mode)  
   - c. Setear la hora ✔️  
   - d. Cambiar el mapeo de memoria ✔️  

6. **Considere un sistema con 2 CPU y 2 threads por CPU (hyperthreading). Suponga que se inician 3 programas con tiempos de ejecución de 5, 10 y 20 ms respectivamente. ¿Cuánto demora la ejecución de estos programas? Los programas son 100% CPU bound, no se bloquean y no cambian de CPU.**  
   
   Hay 4 hilos de ejecucion -> alcanza uno para cada proceso

   Hay 2 cpu por lo que en un instante de tiempo pueden estar corriendo a lo sumo 2 procesos

   Si se le asigna a un cpu el programa de 20 y al otro el de 10 y 5 con hyperthreading, el tiempo total va a ser
    $$ max\{20, 10+5\} = 20 $$

7. **¿Qué es una instrucción TRAP?**  
   - Instrucción especial para pasar de user mode a kernel mode.  
   - Se usa para invocar syscalls de forma controlada.  

8. **¿Es relevante para el programador saber si una función hace syscall?**  
   - Sí, porque las syscalls son más costosas que una función normal (cambio de contexto y privilegios).  
   - También pueden bloquear el proceso y tienen implicancias de seguridad.  



## Glosario
- **Multiplexar**: compartir recursos en tiempo y espacio.  
- **PSW (Program Status Word)**: estado del CPU (flags).  
- **Pipeline**: ejecución solapada de instrucciones.  
- **Superscalar**: múltiples instrucciones en paralelo.  
- **Localidad**: propiedad de accesos repetidos o contiguos.  
- **POSIX**: estándar portable de interfaces.  
- **Monolithic / Microkernel**: arquitecturas de sistemas operativos.  
- **Ley de Moore**: duplicación de transistores con el tiempo.  
- **HDD / SSD**: tipos de almacenamiento.  
