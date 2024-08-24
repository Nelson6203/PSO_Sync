# PSO_Sync

## Estudiantes
- Nelson Alvarado Navarro
- Casey Baeza Castrillo
- Bryan Castro Solís

## Descripción

La biblioteca "sync" proporciona una implementación en C de varias estructuras de sincronización. Estas son utlizadas para facilitar la coordinación de hilos en programas que requieren acceso controlado a recursos compartidos.

Las estructuras implementadas son:
- Semáforo
- Barrera
- Read/Write Lock

## Pre-requisitos
- Esta biblioteca funciona con la biblioteca incorporada de PThreads, en un entorno Linux
- Instalación del compilador de C gcc
  ```bash
  sudo dnf install gcc
  ```

## Implementación
### Semáforo (`sync.h` y `sync.c`)
Se implementó mediante una estructura la cual se llamó `Semaphore`, la cual tiene un valor entero inicial (`value`), un mutex (`mutex`) y una variable de condición (`cond`) de la biblioteca `pthread.h`. Se realizaron 3 funciones; `semaphore_init` en la cual se inicializa el semáforo con un valor inicial, también la función `semaphore_wait` y `semaphore_signal` las cuales decrementa e incrementa el valor del semáforo.

### Barrera (`sync.h` y `sync.c`)
Se implementó mediante una estructura llamada `Barrier`, la cual tiene un contador de hilos que han llegado a la barrera (`count`), un valor de cantidad de hilos que esperan en la barrera (`total`), un mutex (`mutex`) y una variable de condición (`cond`) de la biblioteca `pthread h`. Se realizaron 2 funciones; `barrier_init` en la cual se inicializa la barrera con un valor de la cantidad de hilos y la función `barrier_wait` la cual sincroniza los hilos. Esta función incrementa el contador de hilos que han llegado a la barrera. Si todos los hilos han llegado, se reinicia el contador y se despierta a todos los hilos, y si no el hilo se bloquea.


### Read/Write Lock (`sync.h` y `sync.c`)
Se implementó una estructura llamada `RWLock`, la cual tiene tres contadores, uno para los lectores activos (`num_readers_waiting`), otro para los escritores en espera (`num_writers_waiting`), y el último para ver si un escritor está activo (`writer_active`). Además, la estructura incluye un mutex (`mutex`)y dos variables de condición de la biblioteca “pthread.h”, una para los escritores (`cond_writers`) y otra para los lectores (`cond_readers`). Se realizaron 5 funciones, una de inicialización (`RWLock_init`), una que gestiona el inicio (`RWLock_begin_read`) y otra el fin de una operación de lectura (`RWLock_end_read`), y por último una que controla el inicio (`RWLock_begin_write`) y otra el fin de una operación de escritura (`RWLock_end_write`).


## Uso de la Biblioteca
Para utilizar la biblioteca ha de considerar los siguientes pasos:

1. Incluir el include del archivo `sync.h` en el archivo de pruebas `main.c`.
   ```c
   #include "sync.h"
   ```

2. Inicializar las estructuras de sincronización (semáforos, barreras o Read/Write lock) utilizando las funciones correspondientes (`semaphore_init`, `barrier_init`, `RWLock_init`).

3. Llevar a cabo la sincronización de hilos.

  
## Compilación
Esta biblioteca debe de compilarse en el entorno de desarrollo de Linux con los siguientes los siguientes pasos:

1. Abra una terminal en la dirección de la carpeta en donde se encuentran los archivos descargados.
2. Después, ejecute los siguientes comandos en la terminal:
   ```bash
   gcc -c sync.c -o sync.o -pthread
   ```
   ```bash
   gcc main.c sync.o -o main -pthread
   ```
   ```bash
   ./main
   ```
