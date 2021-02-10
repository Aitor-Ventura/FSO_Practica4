// Prueba concurrente de acceso al búfer
// Se lanzan hilos productores y consumidores
// Al no haber sincronización entre los hilos,
// el resultado es que se puede corromper el búfer

// La compilacion se realiza mediante la orden: c99 -o practica4 test_hilos.c buffer_circular.c -lpthread

//Archivos cabecera del sistema
#include <stdlib.h> // atoi(), exit()...
#include <stdio.h>  // printf(), fputs(), stderr...
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "buffer_circular.h"
// Añadido por el alumno
#include "test_hilos.h"

  //Variables globales
// Cierre de exclusión mutua
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// Condiciones
pthread_cond_t condProd = PTHREAD_COND_INITIALIZER;
pthread_cond_t condCons = PTHREAD_COND_INITIALIZER;


// Función auxiliar para comprobar errores
// e imprimir un mensaje si hay error
void check_error(int return_code, const char* msg)
{
  if (return_code!=0)
  {
    if (msg != NULL) {
      fprintf(stderr,"%s\n",msg);
    } else {
      fprintf(stderr,"error %d: %s\n",errno,strerror(errno));
    }
    exit(-1);
  }
}

// Hilo Productor: inserta un item en el búfer
void* Productor (void *item)
{
  // espera mientras el búfer está lleno
  //while ( buffer_pendientes_consumir() == buffer_capacidad() ) {}
    
	//buffer_inserta((ITEM)item);
	//pthread_exit(NULL);

  // Añadido por el alumno
  // Región crítica
  // Bloquea para que nadie mas pueda acceder al recurso
  pthread_mutex_lock(&mutex);
  // Espera mientras el búfer está lleno
  while (buffer_pendientes_consumir() == buffer_capacidad()){
    // Bloquea el hilo de productores y libera el mutex
    pthread_cond_wait(&condProd, &mutex);
  }

  buffer_inserta((ITEM) item);
  // Desbloquea los hilos de consumidores
  pthread_cond_signal(&condCons); 
  // Desbloquea el mutex
  pthread_mutex_unlock(&mutex);
  // Fin región crítica
  pthread_exit(NULL);
}

// Hilo Consumidor: extrae un item del búfer
void* Consumidor (void* no_se_usa)
{
  // espera si el búfer está vacío
  //while ( buffer_pendientes_consumir() == 0 ) {}

  //ITEM item;
	//buffer_extrae(&item);
  //printf("Consumidor extrae: %ld\n",item);
	//pthread_exit(NULL);

  // Añadido por el alumno
  // Región crítica
  // Bloquea para que nadie más pueda acceder al recurso
  pthread_mutex_lock(&mutex);
  // Espera mientras el búfer está vacío
  while (buffer_pendientes_consumir() == 0){
    // Bloquea el hilo de consumidores y libera el mutex
    pthread_cond_wait(&condCons, &mutex);
  }
  ITEM item;
  buffer_extrae(&item);
  // Desbloquea los hilos de productores
  //pthread_cond_signal(&condCons); 
  pthread_cond_signal(&condProd);
  // Desbloquea el mutex
  pthread_mutex_unlock(&mutex);
  // Fin región crítica
  pthread_exit(NULL);
}

// Función auxiliar:
// espera a que termine un lote de hilos
// el lote es un vector de pthread_t
void espera_lote_hilos (pthread_t* lote, int nhilos)
{
	void* dummy; // pthread_join obliga a pasar un void*
	
	for (int i=0;i<nhilos;i++)
	{
		int ret=pthread_join(lote[i],&dummy);
		check_error(ret,0);
	}
}

// Rutina que lanza los hilos productores y consumidores
void test_hilos (int nprod, int ncons)
{
  // vectores de hilos (de tamaño variable) 
  pthread_t productores[nprod];
  pthread_t consumidores[ncons];

  // Lanzamos los hilos  
  for(long i=0; i<nprod; i++) 
  {
    // hay que convertir el long en void*
    // porque lo exige la función pthread_create
    void* item = (void*)i;
    int ret = pthread_create(&productores[i], NULL, Productor, item);
    check_error(ret,0);
  }

  for (int i=0; i<ncons; i++) 
  {
    int ret = pthread_create(&consumidores[i], NULL, Consumidor, NULL);
    check_error(ret,0);
  }
 
  // Esperamos a que todos los hilos finalicen
  espera_lote_hilos(productores,nprod);
  espera_lote_hilos(consumidores,ncons);
}


// Estructura con los parámetros de configuración
// para la simulación (se rellenan desde
// los argumentos argc y argv)
struct config {
	int capacidad;
	int nprod;
	int ncons;
};


// Función que comprueba y captura los parámetros pasados por línea de órdenes
void lee_argumentos (int argc, char *argv[], struct config* config)
{
	check_error( argc != 4, 
    "Invocación incorrecta. Sintaxis: ./fich_ejec capacidad num_prod num_cons");

	config->capacidad = atoi(argv[1]);
	check_error( config->capacidad < 1, 
    "Invocación incorrecta. La dimensión del búfer debe ser mayor que cero.");
	
	config->nprod = atoi(argv[2]);
	check_error( config->nprod < 0, 
    "Invocación incorrecta. El número de hilos productores debe ser mayor que cero.");

	config->ncons = atoi( argv[3] );
	check_error( config->ncons < 0, 
    "Invocación incorrecta. El número de hilos consumidores debe ser mayor que cero.");

  // Añadido por alumno
  // Si consumidores > productores, esperamos infinitamente por un hilo productor
  check_error(config->nprod < config->ncons,
    "Invocación incorrecta. El número de consumidores debe ser menor al de productores.");

  // Si la diferencia entre el número de productores y el número de consumidores es menor a la capacidad del buffer, se quedará esperando infinitamente
  check_error((config->nprod - config->ncons) > config->capacidad,
    "Invocación incorrecta. El número de hilos consumidores debe ser menor o igual a la dimensión del buffer.");
}

// Función main:
// Invocación:
// 	# ./fich_ejec capacidad num_prod num_cons
// 	fich_ejec: nombre del archivo que contiene el código ejecutable
// 	capacidad: capacidad máxima del búfer (>0)
// 	num_prod: número de hilos productores (>=0)
// 	num_cons: número de hilos consumidores (>=0)
//
int main(int argc, char *argv[])
{
	struct config configuracion;

	puts("»»» Comienza la prueba del búfer...");
	lee_argumentos(argc,argv,&configuracion);

	int ret = buffer_inicia(configuracion.capacidad);
	check_error(ret,"Error al crear el búfer");

	test_hilos(configuracion.nprod,configuracion.ncons);
	puts("»»» Fin de la prueba del búfer");
	exit(0);
}