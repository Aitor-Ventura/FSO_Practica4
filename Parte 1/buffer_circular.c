// Archivo de definición de las funciones del búfer compartido

// Se incluye fichero cabecera 
// en el que se declara la interfaz pública del búfer
#include "buffer_circular.h"

#include <assert.h> 	// assert()
#include <stdlib.h>		// malloc()
#include <unistd.h>  	// sleep()  
#include <stdio.h>   	// printf()


// Variable interna con el búfer
// Las variables "static" solo son visibles en el módulo en el que se declaran
// - el campo "datos" lo inicializamos a NULL

struct Buffer {
	ITEM* datos;		
	int capacidad;		
	int ptr_frente;	
	int ptr_final;	
	int pendientes_consumir;
};

static struct Buffer the_buffer = { .datos = NULL };

// --- IMPLEMENTACIÓN DE LAS OPERACIONES

// Función de construcción del búfer circular
int buffer_inicia (int capacidad)
{
	the_buffer.datos = malloc( sizeof(ITEM)*capacidad );
	if (the_buffer.datos!=NULL)
	{
		the_buffer.capacidad = capacidad;
		the_buffer.ptr_frente = 0;
		the_buffer.ptr_final = 0;
		the_buffer.pendientes_consumir = 0;
		return 0;

	} else {
		return -1;
	}
}

// Inserta un item en el búfer y actualiza los campos de datos del búfer
void buffer_inserta (ITEM item)
{
	assert (the_buffer.datos != NULL);
	assert (the_buffer.pendientes_consumir<the_buffer.capacidad);

	the_buffer.datos[the_buffer.ptr_final] = item;

	// introducimos un retardo arbitrario 
	// si el ítem es un número impar
	if ( item % 2 == 1 ) { sleep(1); }

	printf(">>> inserción: item=%ld, pos=%d\n",
			item,the_buffer.ptr_final);

	the_buffer.ptr_final = (the_buffer.ptr_final+1) % the_buffer.capacidad;
	the_buffer.pendientes_consumir++;
}

// Extrae un elemento del búfer y actualiza los campos de datos del búfer
void buffer_extrae (ITEM* item)
{
	assert (the_buffer.datos != NULL);
	assert (the_buffer.pendientes_consumir>0);

	*item = the_buffer.datos[the_buffer.ptr_frente];
	printf("<<< extracción: item=%ld, pos=%d\n",
			*item, the_buffer.ptr_frente);
	the_buffer.ptr_frente = (the_buffer.ptr_frente+1) % the_buffer.capacidad;
	the_buffer.pendientes_consumir--;
}

// Función que retorna el número de ítems pendientes de consumir
int buffer_pendientes_consumir()
{
	assert (the_buffer.datos != NULL);
	return the_buffer.pendientes_consumir;
}

// Función que retorna la capacidad máxima del búfer
int buffer_capacidad()
{
	assert (the_buffer.datos != NULL);
	return the_buffer.capacidad;
}