/**
 * Problema de sincronización de hilos
 * En el África profunda viven junto a un río una tribu de caníbales y un grupo de misioneros. El río se puede cruzar mediante un bote, con capacidad para tres personas. Como las aguas son bravas
 * el bote siempre tiene que transportar carga completa. Si se sientan dos misioneros y un caníbal, éstos le comen el tarro el caníbal hasta el punto de que el caníbal se arroja al agua: hay que evitar
 * esta situación. Las demás combinaciones no dan problemas.
 * 
 * El sistema debe lanzar varios hilos, uno por cada persona. Escriba dos procedimientos: llegaMisionero() y llegaCanibal(), para cuando cada persona llega al bote. Estos procedimientos dejan al hilo
 * esperando hasta que el bote se encuentre lleno y seguro. Cuando esto ocurra, el procedimiento retorna y se considera que el barco vuelve a quedar vacío.
 * 
 * v4. 
 */

/**
 * Librerías usadas
 */
#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include "bote.h"
#include "misioneros.h"

// Definición abajo, función interna
void threadJoin(pthread_t* thread, int nhilos);

/**
 * Variables globales
 * @var mutex variable cerrojo que controlará el acceso compartido a las variables
 * @cond waitLand y waitBoat que controlan la espera tanto en tierra como en el bote y necesitemos que hilos de otros tipos nos salven del apuro
 * @var nMis numero de misioneros TOTALES, nCan numero de canibales TOTALES
 * @var misOnBoard numero de misioneros abordo del bote, canOnBoard numero de canibales abordo del bote
 * @var booleanas booleanw que indica si se ha dado un warning al usuario o no, booleanr que indica si el bote está listo para viajar
 */
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t waitLand = PTHREAD_COND_INITIALIZER;
//pthread_cond_t  waitBoat= PTHREAD_COND_INITIALIZER;
int nMis, nCan, misOnBoard = 0, canOnBoard = 0, booleanw = 0, booleanr = 0;

/**
 * Indica el error que se encuentra a través de un código de error que se pasa por parámetro
 * @param eCode código de error
 */
void check_error(int eCode){
    switch (eCode){
        case 0:
            fprintf(stderr, "[!] Invocación incorrecta: ./misioneros numMis nCan\n");
            exit(-1);
        case 1:
            printf("[!] Error al crear el bote.\n");
            exit(-1);
    }
}

/**
 * Indica el error que se encuentra a través de un código de error que se pasa por parámetro, además de la instancia
 * @param eCode código de error
 * @param i instancia de error
 */
void check_errori(int eCode, int i){
    switch (eCode){
        case 2:
            fprintf(stderr, "[!] Error en pthread_join con el hilo %d - %s\n", i, strerror(errno));
            exit(-1);
        case 3:
            fprintf(stderr, "[!] Error en pthread_create con el hilo misionero %d - %s\n", i, strerror(errno));
            exit(-1);
        case 4:
            fprintf(stderr, "[!] Error en pthread_create con el hilo canibal %d - %s\n", i, strerror(errno));
            exit(-1);
    }
}

/**
 * Rutina de los Misioneros
 * Comentario: tratar las rutinas como las biografías de los misioneros, se suben, viajan, se bajan y continuan con sus vidas
 */
void* llegaMisionero(){
    //pthread_mutex_lock(&mutex);
    // Si el bote está lleno, o ya hay un misionero y un canibal y queremos evitar la combinación prohibida, se espera en tierra
    // En la segunda situación, solo un canibal será capaz de salvarnos, luego liberamos el mutex
    //while (booleanr == 1 || (misOnBoard == 1 && canOnBoard == 1)) pthread_cond_wait(&waitLand, &mutex);

    /**
     * Checkeo de casos extraordinarios y finales con el numero total de misioneros y canibales
     */
    if (nMis + nCan <= 3){
        // Si se da la combinación prohibida
        if (nMis == 2 && nCan == 1 && booleanw == 0){
            booleanw = 1;
            printf("[!] Dos misioneros y un canibal no pueden viajar juntos.\n");
        // Que se de el caso que no tengamos suficientes canibales y misioneros como para llenar el bote
        } else if (nMis + nCan < 3 && booleanw == 0){
            booleanw = 1;
            printf("[!] El bote no se puede llenar.\n");
        }

        /* 
         * Si se ha avisado al usuario de los errores y estamos en las situaciones finales, si queremos que el programa finalice bonito salimos del hilo,
         * si no, comentar la línea de pthread_exit y espera infinitivamente por que llegue un recurso
         */
        //if (booleanw == 1){
        //    pthread_cond_broadcast(&waitLand);
        //    pthread_mutex_unlock(&mutex);
        //    pthread_exit(NULL);
        //}
    }

    /**
     * Funcionamiento de la rutina: subirse al bote
     */
    ITEM mis;
    subirBote((ITEM) mis);
    misOnBoard++;
    printf("EVENTO: Sube un misionero al bote. Num de personas en el bote: %d, Misioneros abordo: %d, Canibales abordo: %d\n",
            boteInside(), misOnBoard, canOnBoard);

    /*
     * Funcionamiento de la rutina: viajar
     */
    if (boteInside() == boteCapacidad()) { booleanr = 1; printf("\nEVENTO: VIAJAMOS\n\n");}

    /* Mientras el bote no se llene, bloqueamos el hilo con la condición del bote, liberando el mutex y permitiendo que tanto misioneros como caníbales puedan subirse
     * hasta que se llene, que cuando lo haga, todos aquellos hilos que se encontraban bloqueados puedan seguir con el funcionamiento */
    while (boteInside() != boteCapacidad()){
        //pthread_cond_wait(&waitBoat, &mutex);
        if (booleanr == 1) break;
    }

    /*
     * Funcionamiento de la rutina: bajarse del bote
     */
    bajarBote(&mis);
    printf("EVENTO: Se baja un misionero del bote.\n");

    // Decremento tanto del numero de misioneros totales (que necesitan viajar), como los que están abordo (pues empiezan a bajarse)
    nMis--;
    misOnBoard--;

    // Cuando el bote esté vacío, indicarlo, mostrar quien falta por ser trasladado, y avisar a todos los que estaban en tierra que ya pueden empezar a subir
    if (boteInside() == 0){
        // Avisar que el bote se encuentra vacío
        booleanr = 0;
        printf("\nEVENTO: El bote vuelve vacío. ¡Las corrientes estaban a nuestro favor!\n");
        printf("RESTANTES: Misioneros %d, Canibales %d\n\n", nMis, nCan);
        //pthread_cond_broadcast(&waitLand);
      // Ir liberando uno a uno a aquellos hilos condición que se encontraban bloqueados
    } //else pthread_cond_signal(&waitBoat);

    //pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

/**
 * Rutina de los Canibales
 * Comentario: tratar las rutinas como las biografías de los canibales, se suben, viajan, se bajan y continuan con sus vidas
 */
void* llegaCanibal(){
    //pthread_mutex_lock(&mutex);
    /* Si el bote está lleno, o ya se encuentran dos misioneros en el bote y queremos evitar la combinación prohibida, o únicamente se halla un único caníbal
     * y queremos que se suban misioneros porque su combinación es posible y no queremos entrar en un bucle infinito, se espera en tierra */
    //while (booleanr == 1 || (misOnBoard == 2 || nCan == 1 && nMis >= 3)) pthread_cond_wait(&waitLand, &mutex);

    /**
     * Checkeo de casos extraordinarios y finales con el numero total de misioneros y canibales
     */
    if (nMis + nCan <= 3){
        // Si se da la combinación prohibida
        if (nMis == 2 && nCan == 1 && booleanw == 0){
            booleanw = 1;
            printf("[!] Dos misioneros y un canibal no pueden viajar juntos.\n");
        // Que se de el caso que no tengamos suficientes canibales y misioneros como para llenar el bote
        } else if (nMis + nCan < 3 && booleanw == 0){
            booleanw = 1;
            printf("[!] El bote no se puede llenar.\n");
        }

        /* 
         * Si se ha avisado al usuario de los errores y estamos en las situaciones finales, si queremos que el programa finalice bonito salimos del hilo,
         * si no, comentar la línea de pthread_exit y espera infinitivamente por que llegue un recurso
         */
        //if (booleanw == 1){
        //    pthread_cond_broadcast(&waitLand);
        //    pthread_mutex_unlock(&mutex);
        //    pthread_exit(NULL);
        //}
    }

    /**
     * Funcionamiento de la rutina: subirse al bote
     */
    ITEM can;
    subirBote((ITEM) can);
    canOnBoard++;
    printf("EVENTO: Sube un canibal al bote. Num de personas en el bote: %d, Misioneros abordo: %d, Canibales abordo: %d\n",
            boteInside(), misOnBoard, canOnBoard);

    /*
     * Funcionamiento de la rutina: viajar
     */
    if (boteInside() == boteCapacidad()) { booleanr = 1; printf("\nEVENTO: VIAJAMOS\n\n"); }

    /* Mientras el bote no se llene, bloqueamos el hilo con la condición del bote, liberando el mutex y permitiendo que tanto misioneros como caníbales puedan subirse
     * hasta que se llene, que cuando lo haga, todos aquellos hilos que se encontraban bloqueados puedan seguir con el funcionamiento */
    while (boteInside() != boteCapacidad()){
        //pthread_cond_wait(&waitBoat, &mutex);
        if (booleanr == 1) break;
    }

    /*
     * Funcionamiento de la rutina: bajarse del bote
     */
    bajarBote(&can);
    printf("EVENTO: Se baja un canibal del bote.\n");

    // Decremento tanto del numero de canibales totales (que necesitan viajar), como los que están abordo (pues empiezan a bajarse)
    nCan--;
    canOnBoard--;

    // Cuando el bote esté vacío, indicarlo, mostrar quien falta por ser trasladado, y avisar a todos los que estaban en tierra que ya pueden empezar a subir
    if (boteInside() == 0){
        // Avisar que el bote se encuentra vacío
        booleanr = 0;
        printf("\nEVENTO: El bote vuelve vacío. ¡Las corrientes estaban a nuestro favor!\n");
        printf("RESTANTES: Misioneros %d, Canibales %d\n\n", nMis, nCan);
        //pthread_cond_broadcast(&waitLand);
    // Ir liberando uno a uno a aquellos hilos condición que se encontraban bloqueados
    } //else pthread_cond_signal(&waitBoat);

    //pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

/**
 * Se encarga de que todos los hilos indicados terminen
 * @param thread hilo de cualquier tipo
 * @param nhilos numero de hilos por los que se debe esperar
 */
void threadJoin(pthread_t* thread, int nhilos){
    void* dummy;
    int i, res;
    
    for (i = 0; i < nhilos; i++){
        res = pthread_join(thread[i], &dummy);
        if (res != 0) check_errori(2, i);
    }
}

/**
 * Se encarga de crear los hilos
 * @param arg1 numero de hilos misioneros
 * @param arg2 numero de hilos canibales
 */
void threadCreate(int arg1, int arg2){
    pthread_t hiloMisionero[arg1];
    pthread_t hiloCanibal[arg2];

    int i, res;
    for (i = 0; i < arg1; i++){
        res = pthread_create(&hiloMisionero[i], NULL, llegaMisionero, NULL);
        if (res != 0) check_errori(3, i);
    }

    for (i = 0; i < arg2; i++){
        res = pthread_create(&hiloCanibal[i], NULL, llegaCanibal, NULL);
        if (res != 0) check_errori(4, i);
    }

    threadJoin(hiloMisionero, arg1);
    threadJoin(hiloCanibal, arg2);
}

/**
 * Se encarga de comprobar que los argumentos son correctos, si no lo fuera, imprimiría por pantalla el mensaje de error correspondiente
 * Además, asignamos a las variables numero de misioneros y de los canibales los valores pasados por línea de órdenes
 */
void checkArguments(int argc, char *argv[], int *argM, int *argC){
    if (argc != 3) check_error(0);
    *argM = atoi(argv[1]);
    nMis = atoi(argv[1]);
    //if (*argM < 0) check_error(1);
    *argC = atoi(argv[2]);
    nCan = atoi(argv[2]);
    //if (*argC < 0) check_error(2);
    //if (((*argM + *argC) % 3) != 0) check_error(3);
}