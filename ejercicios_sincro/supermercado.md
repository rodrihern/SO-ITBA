# Cadena de supermercados (2.5 puntos)

## Enunciado



Una cadena de supermercados tiene sucursales en todo el país. Debido a que la actualización de precios es una tarea propensa a errores, la delega a cada proveedor. A su vez, cada computadora de cada sucursal actualiza su lista de precios localmente al iniciar.
A continuación se presentan las funciones que ejecutan las sucursales y los proveedores.

```c
sem_t sem_a = 1; // Asumimos que set_t funciona como un int
sem_t sem_b = 1;
int sc = 0;

void sucursal() {
    while (1) {
        sem_wait(&sem_a);
        if(++sc == 1) sem_wait(&sem_b);
        sem_post(&sem_a);

        consultar_precios_en_casa_central();

        sem_wait(&sem_a);
        if(--sc == 0) sem_post(&sem_b);
        sem_post(&sem_a);

        actualizar_precios_localmente();
    }
}

void proveedor() {
    while (1) {
        calcular_aumento_de_precios();

        sem_wait(&sem_b);

        actualizar_precios_en_casa_central();

        sem_post(&sem_b);
    }
}
```
Desde que se usa este sistema, los proveedores reportan demoras de varios minutos para poder actualizar los precios, sobre todo en el horario de apertura de las sucursales.
Explique por qué sucede esto y modifique la solución presentada para evitar estas demoras a los proveedores.


## Mi solucion

Esto sucede por la inanicion del proveedor, si siempre hay sucursales consultando el precio
el proveedor no puede entrar. Es una version del problema clasico de readers y writers con inanicion
del writer. La solucion es agregar un torniquete para que cuando el writer va a entrar,
los proximos readers esperen


```c

sem_t precios_mutex = 1; // anterior sem_b
sem_t sc_mutex = 1; // anterior sem_a
int sc = 0;

// agregamos el torniquete
sem_t torniquete = 0;

void sucursal() {
    while (1) {
        sem_wait(&torniquete);

        sem_wait(&sc_mutex);
        if(++sc == 1) sem_wait(&precios_mutex);
        sem_post(&sc_mutex);

        sem_post(&torniquete);

        consultar_precios_en_casa_central();

        sem_wait(&sc_mutex);
        if(--sc == 0) sem_post(&precios_mutex);
        sem_post(&sc_mutex);

        actualizar_precios_localmente();
    }
}

void proveedor() {
    while (1) {
        sem_wait(&torniquete);
        calcular_aumento_de_precios();

        sem_wait(&precios_mutex);

        actualizar_precios_en_casa_central();

        sem_post(&precios_mutex);
        sem_post(&torniquete);
    }
}
```