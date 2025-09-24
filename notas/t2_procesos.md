# 📌 Procesos

## Modelo de procesos
- Todo software ejecutable se organiza en **procesos secuenciales**.  
- **Proceso**: abstracción de un programa en ejecución.  
- **Programa**: estático en disco, no hace nada hasta que se ejecuta.  
- Un mismo programa puede correr varias veces → distintos procesos.  
- Conceptualmente cada proceso tiene su propio CPU (sus registros y variables).  
- Esto permite **pseudo-concurrencia** en un solo CPU (*pseudo paralelismo*).  
- **Paralelismo real**: múltiples CPUs compartiendo memoria física.  
- Multiprogramación: facilita pensar en procesos corriendo en paralelo en lugar de en los *context switches*.  
- El switch de procesos **no es determinista ni reproducible** → difícil suponer tiempos de ejecución.

---

## Creación de procesos
### Casos comunes
- Al iniciar el SO (daemons).  
- Cuando un proceso crea otro proceso (ej: scripts bash).  
- Cuando un usuario ejecuta un programa (terminal, GUI).  
- Inicio de un trabajo por lotes (batch).  

### En UNIX
- `fork`: clona el proceso (padre → hijo), incluyendo memoria, variables y fds abiertos.  
- `execve`: reemplaza la imagen del proceso (código, heap, stack, registros) pero preserva fds abiertos.  
- Modelo en **2 etapas**: el hijo puede ajustar file descriptors antes de `exec`.

### En Win32
- `CreateProcess`: requiere ~10 parámetros (programa, args, atributos de seguridad, prioridad, ventana, etc).  
- +100 syscalls extra para administración/sincronización.  
- En ambos sistemas, cada proceso tiene **su propio espacio de direcciones**.

---

## Terminación de procesos
- **Normal (voluntaria)**: `exit(0)`, `return 0`.  
- **Por error (voluntaria)**: `exit(!=0)`, `return !=0`.  
- **Error fatal (involuntario)**: instrucción inválida, división por 0, acceso a memoria inválida → señal.  
- **Muerto por otro proceso (involuntario)**: `kill`, sujeto a permisos y modo kernel/user.  
- Pregunta importante: ¿qué ocurre con los procesos hijos cuando muere el padre? (depende del sistema).

---

## Jerarquía de procesos
### UNIX
- Relación **padre–hijo**.  
- Un padre y descendientes forman un **grupo de procesos**.  
- Señales pueden recibirse por grupo (ej: `CTRL+C` desde teclado).  
- El proceso **init** es la raíz de este árbol.  

### Win32
- No existe jerarquía.  
- El padre recibe un **handle** para manejar al hijo.  
- Ese handle puede transferirse a otros procesos.  

---

## Estados de procesos
- **Running**: ejecutándose en CPU.  
- **Ready**: listo para ejecutar, esperando CPU.  
- **Blocked**: esperando un evento externo (ej: I/O).  

Los procesos van cambiando entre estos estados mediante *scheduler* y eventos de I/O.

---

## Implementación de procesos
- Se mantiene una **tabla de procesos**.  
- Cada entrada es un **Process Control Block (PCB)** con:
  - Estado del proceso  
  - Registros de CPU  
  - Información de memoria  
  - Archivos abiertos  
- El PCB permite restaurar un proceso en un *context switch*.  
- La multiprogramación da la ilusión de múltiples procesos secuenciales.

---

## Multiprogramación
- Mejora la utilización del CPU.  
- Ejemplo simple:
  - Si un proceso usa CPU el 20% del tiempo → con 5 procesos se aprovecha 100%.  
- Análisis probabilístico:
  - Supongamos: cada proceso pasa una fracción `p` esperando I/O.  
  - Con `n` procesos en memoria, la probabilidad de que todos estén esperando = `p^n`.  
  - **Utilización del CPU = 1 - p^n**.  
- Conclusión: la multiprogramación *garpa*, porque reduce los tiempos muertos del CPU.

---

## Ejercicios típicos
1. **Comando `type`**  
   - Clasifica un comando en alias, keyword, function, builtin, file.  
   - Preguntas: ejemplos, por qué `cd` es builtin y qué pasaría si no lo fuera.  

2. **Diagrama de estados**  
   - Hay 3 estados principales → hasta 8 transiciones posibles.  
   - Analizar por qué algunas no existen.  

3. **Comando `time`**  
   - Reporta tiempo real, user y sys.  
   - Ejercicios prácticos: `time bash`, `time cat`, `time md5sum big`, etc.  
   - Analizar proporciones de user/sys respecto a real.  

4. **Directorio `/proc`**  
   - Investigar qué información expone (archivos virtuales con info del kernel y procesos).  

5. **Multiprogramación con 5 procesos**  
   - Cada uno bloqueado 50% del tiempo → calcular fracción de CPU ocioso.  

6. **Memoria y utilización de CPU**  
   - Máquina con 4GB RAM, 512MB SO, procesos de 256MB.  
   - Calcular cuántos entran y qué proporción de espera por I/O permite llegar a 99% utilización.  

7. **Multiprogramación con 6 procesos**  
   - Cada proceso espera 40% del tiempo.  
   - Calcular utilización de CPU.  

---

## ✅ En resumen
- El proceso es la **unidad básica de ejecución** en un SO.  
- Se crean con `fork/exec` (UNIX) o `CreateProcess` (Win32).  
- Terminan de forma voluntaria o involuntaria.  
- Estados: Running, Ready, Blocked.  
- Los SO los gestionan mediante PCBs.  
- La multiprogramación permite aprovechar mejor el CPU gracias al solapamiento de I/O y cómputo.  

## Preguntas

### Ejercicio 1

type nos indica el tipo de comando, los posibles resultados con ejemplos son:

* alias -> ll puede ser un alias de ls -lh
* keyword -> if, for
* function -> definido por el usuario, como mydate() { date +%F; }
* builtin -> type, cd, echo
* file -> ls, ps


¿Por qué cd es builtin?  
El cwd es estado del proceso. Un hijo no puede persistir cambios en el padre tras terminar.  
¿Qué haría falta si no fuera builtin?  
1. Nueva syscall (setcwd(pid,...)) para cambiar el directorio de otro proceso (rompe el modelo de aislamiento).  
2. Protocolo: el binario “cd” calcula el path y la shell luego hace chdir() (la shell igual necesita lógica especial).  
3. Enviar un descriptor de directorio (SCM_RIGHTS) y la shell hace fchdir().  
4. Reemplazar la shell con otra (pierde estado; impráctico).  
5. Usar “source” en vez de ejecutar un proceso separado (equivale a mantenerlo builtin).

Todas las opciones terminan reintroduciendo código en la shell; por eso cd permanece builtin.



### Ejercicio 2

No se puede pasar de bloqueado a running porque justamente si estas bloqueado, el scheduler no te va a poner a correr bro, primero pasas por ready

Si estas ready no podes pasar a blockeado porque como ejecutas la funcion que te bloquea bro? tenes que estar corriendo si o si para bloquearte


### Ejercicio 3

corriendo

``` bash
time bash
```

y ejecutando comandos random en la terminal se obtuvo la siguiente salida:

```
real 1m7.375s
user 0m0.455s
sys 0m0.109s
```


`real`: tiempo total transcurrido desde que empieza hasta que termina (incluye espera de usuario/I/O).  
`user`: CPU en espacio de usuario consumida por el proceso (y a veces sus hijos).  
`sys`: CPU en modo kernel (syscalls).  
En sesiones interactivas: `real >> user + sys` porque la shell pasa la mayor parte esperando entrada (bloqueado).

### Ejercicio 4

/proc es un directorio (no esta en disco) en el cual hay informacion sobre los procesos que estan corriendo

### Ejercicio 5

**Una computadora particular puede alojar 5 procesos en su memoria principal. Estos procesos están bloqueados por I/O la mitad del tiempo. ¿Qué fracción del tiempo de CPU es desperdiciada?**

la probabilidad de que uno este bloqueado es 0.5, luego la probabilidad de que los 5 esten bloqueados al mismo tiempo es $0.5^5 = \frac{1}{32}$ osea esta bloqueado esa fraccion del tiempo

### Ejercicio 6
**Una computadora particular tiene 4GB de RAM, de los cuales 512 MB son ocupados por el sistema operativo. Los procesos (por simplicidad) ocupan 256 MB c/u y tienen las mismascaracterísticas. Si el objetivo es lograr un 99% de utilización de CPU, ¿Cuál esla máxima proporción de tiempo que puede pasar un proceso esperando por I/O?**


Memoria disponible para procesos -> 3 GB y 512 MB

Procesos identicos de 256 MB -> tengo espacio para n = 14 procesos

si un proceso esta bloqueado una proporcion de tiempo p, se desperdicia $\frac{1}{p^n}$ del tiempo

luego

$$
1 - p^{14} = 0.99 \Rightarrow p^{14} = 0.01
$$
$$
\therefore p = 0.01^{\frac{1}{14}} \approx 0.72
$$

### Ejercicio 7

**Considere un sistema con un grado de multiprogramación de 6 (i.e 6 procesos en memoria al mismo tiempo). Asuma que cada proceso pasa el 40% del tiempo esperando por I/O. ¿Cuál es la utilización de CPU?**

proporcion de tiempo de espera por proceso: p = 0.4

cantidad de procesos en memoria en un instante de tiempo: n = 6

proporcion del tiempo que el cpu esta de chill: $p^n$

luego la utilizacion del cpu es de 
$$
1 - p^n = 1 - 0.4^6 \approx 0.996
$$