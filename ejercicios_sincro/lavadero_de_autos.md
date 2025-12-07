# Ejercicio lavadero de autos

## Enunciado

Se esta implementando una estación de lavado automotor con 5 robots de lavado. Cada robot realiza las siguientes tareas:
1. Toma un auto de la fila, si no hay autos espera a que llegue alguno.
2. Toma 5 litros de liquido detergente retornable de un deposito cuya capacidad inicial es de 25 litros y realiza un prelavado inicial de 10 minutos.
3. Toma 5 litros adicionales de liquido detergente retornable del mismo deposito para un lavado de 20 minutos.
4. Devuelve los 10 litros al deposito.
5. Toma una pistola secadora (hay 3 disponibles para un periodo de secado de 3 minutos)
6. Devuelve la pistola secadora
7. Libera el auto y toma el siguiente.

Realice el código de sincronización del proceso del robot.

## Mi solucion

```c

sem_t pistolas = 3;
sem_t detergente_5lts = 5;
sem_t autos = 0;
sem_t puedo_prelavar = 4;

void robot() {
    while (1) {
        wait(autos);
        wait(puedo_prelavar);
        wait(detergente_5lts);
        prelavado();

        // tengo que agarrar el detergente primero para evitar deadlock
        wait(detergente_5lts);
        post(puedo_prelavar); 
        lavado();
        post(detergente_5lts);
        post(detergente_5lts);

        wait(pistolas)
        secado()
        post(pistolas)

        // el post del auto lo hace el auto

    }
}

```






