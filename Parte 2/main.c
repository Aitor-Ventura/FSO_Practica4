#include "bote.h"
#include "misioneros.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Inicio y finalización del programa
 */
int main(int argc, char* argv[]){
    int argM, argC;
    checkArguments(argc, argv, &argM, &argC);
    int bote = boteStart(3);
    if (bote == -1) check_error(1);
    printf("---------------------\n");
    printf("INICIO DEL PROGRAMA\n");
    threadCreate(argM, argC);
    printf("FIN DEL PROGRAMA\n");
    printf("---------------------\n");
    exit(0);
}