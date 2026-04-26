# SO-ITBA - Notas y Práctica de Clase

Material central de la materia Sistemas Operativos:
- **Resúmenes:** Apuntes teóricos sobre procesos, IPC, hilos, scheduling, memoria y filesystem.
- **Ejercicios de Clase:** Implementaciones y soluciones de problemas de sincronización clásicos.
- **Exámenes Viejos:** Modelos de parciales y finales para práctica en la carpeta `finales/`.

## Notas teóricas

| Nota | Temas |
|------|-------|
| [[t1_introduccion]] | Kernel vs user mode, syscalls, interrupciones, tipos de kernel |
| [[t2_procesos]] | Modelo de procesos, estados, PCB, fork/exec, señales |
| [[t3_ipc]] | Pipes, FIFOs, memoria compartida, semáforos POSIX, message queues |
| [[t4_threads]] | Threads vs procesos, POSIX threads, race conditions |
| [[t5_scheduling]] | Algoritmos de scheduling, Round Robin, prioridades, multilevel queues |
| [[t6_memoria]] | Memoria virtual, paginación, TLB, page faults, swapping |
| [[t7_filesystem]] | Filesystems, inodes, directorios, permisos |

## Ejercicios de sincronización

| Ejercicio | Problema |
|-----------|----------|
| [[gatos_y_ratones]] | Lectores/escritores con límite de 3 lectores (ratones) y un escritor (gato) |
| [[supermercado]] | Starvation de escritores (proveedores) — problema clásico R/W |
| [[embarcacion]] | Embarcación con capacidad 20: embarcar en lote y partir |
| [[foo_bar]] | Barrera cíclica: todos terminan `foo()` antes de que alguno empiece `bar()` |
| [[lavadero_de_autos]] | Lavadero: 5 robots, detergente compartido, 3 secadoras — evitar deadlock |
| [[ascensor]] | Ascensor: máx 6 personas y máx 450 kg — sin starvation para pesados |
| [[fumadores]] | Problema clásico de los fumadores (Patil 1971) |
| [[pluto]] | Examen: dios Pluto reparte pares de recursos a civiles |