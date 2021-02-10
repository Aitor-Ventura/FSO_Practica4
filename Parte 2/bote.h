#ifndef _BOTE
#define _BOTE

/* Interfaz pública del bote
 * El bote contiene un número variable de números enteros long
 * Los elementos serán de tipo entero largo long
 * Se define un alias "ITEM" para facilitar en un futuro
 */

// Cambiar el tipo de datos, ej: float, double, un struct, etc.
typedef long ITEM;

/**
 * Inicializa el bote
 * @param capacidad: numero de items que pueden guardarse en el bote
 * @return 0 sin error, <0 si hay error
 */
int boteStart(int capacidad);

/**
 * Inserta un item en el bote
 * @param item a insertar
 */
void subirBote(ITEM item);

/**
 * Extrae un item del bote
 * @param puntero donde se copiará el objeto extraido
 */
void bajarBote(ITEM* item);

/**
 * Devuelve cuantos objetos están guardados actualmente en el bote
 */
int boteInside();

/**
 * Devuelve la capacidad del bote
 */
int boteCapacidad();

#endif