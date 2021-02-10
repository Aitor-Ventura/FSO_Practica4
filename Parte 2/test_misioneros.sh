#! /bin/bash
echo ******INICIO DEL TEST*******
echo "$line"
echo 1 EJECUCION SIN PARAMETROS
echo ./misioneros
./misioneros
echo "$line"
echo 2 EJECUCION CON 1 PARAMETRO
echo ./misioneros 3
./misioneros 3
echo "$line"
echo 3 EJECUCION CON 2 PARAMETROS
echo ./misioneros 3 3
./misioneros 3 3
echo "$line"
echo 4 EJECUCION CON 2 PARAMETROS PERO NO LLENA BOTE
echo ./misioneros 1 1
./misioneros 1 1
echo "$line"
echo 5 EJECUCION CON 3 PARAMETROS
echo ./misioneros 1 1 1
./misioneros 1 1 1
echo "$line"
echo 6 EJECUCION COMBINACION PROHIBIDA
echo ./misioneros 2 1
./misioneros 2 1
echo 7 EJECUCION CORRECTA
echo ./misioneros 4 9
./misioneros 4 9