// Archivo de definición de las funciones del bote

// Se incluye el archivo cabecera en el que se declara la interfaz pública del bote
#include "bote.h"
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/**
 * Bote con sus atributos apropiados
 */
struct Bote {
    ITEM* datos;
    int capacidad;
    int ptr_frente;
    int ptr_final;
    int pendiente_consumir;
};

static struct Bote bote = { .datos = NULL};

/**
 * IMPLEMENTACIÓN DE LAS OPERACIONES
 */
int boteStart(int capacidad){
    bote.datos = malloc(sizeof(ITEM) * capacidad);
    if (bote.datos != NULL){
        bote.capacidad = capacidad;
        bote.ptr_frente = 0;
        bote.ptr_final = 0;
        bote.pendiente_consumir = 0;
        return 0;
    } else {
        return -1;
    }
}

/**
 * Inserta un item en el bote y actualiza los campos de datos del bote
 */
void subirBote(ITEM item){
    assert (bote.datos != NULL);
    assert (bote.pendiente_consumir < bote.capacidad);
    bote.datos[bote.ptr_final] = item;

    // Introducimos un retardo arbitrario
    if (item % 2 == 1) sleep(1);
    printf(">>> Inserción: pos = %d\n", bote.ptr_final);
    bote.ptr_final = (bote.ptr_final+1) % bote.capacidad;
    bote.pendiente_consumir++;
}

/**
 * Extrae un item del bote y actualiza los campos de datos del bote
 */
void bajarBote(ITEM* item){
    assert (bote.datos != NULL);
    assert (bote.pendiente_consumir > 0);
    *item = bote.datos[bote.ptr_frente];
    printf("<<< Extracción: pos = %d\n", bote.ptr_frente);
    bote.ptr_frente = (bote.ptr_frente + 1) % bote.capacidad;
    bote.pendiente_consumir--;
}

/**
 * Retorna el numero de items pendientes de consumir
 */
int boteInside(){
    assert (bote.datos != NULL);
    return bote.pendiente_consumir;
}

/**
 * Retorna la capacidad maxima del bote
 */
int boteCapacidad(){
    assert (bote.datos != NULL);
    return bote.capacidad;
}