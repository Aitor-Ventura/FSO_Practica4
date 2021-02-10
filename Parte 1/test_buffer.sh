echo "$line"
echo ******INICIO DEL TEST*******
echo "$line"
echo 1 EJECUCION SIN PARAMETROS, ERROR
echo ./practica4
./practica4
echo "$line"
echo 2 EJECUCION CON UN PARAMETRO, ERROR
echo ./practica4 10
./practica4 10
echo "$line"
echo 3 EJECUCION CON DOS PARAMETROS, ERROR
echo ./practica4 10 5
./practica4 10 5
echo "$line"
echo 4 EJECUCION CON TRES PARAMETROS
echo ./practica4 10 10 10
./practica4 10 10 10
echo "$line"
echo 5 EJECUCION CON CAPACIDAD 0, ERROR
echo ./practica4 0 5 5
./practica4 0 5 5
echo "$line"
echo 6 EJECUCION CON CAPACIDAD 1
echo ./practica4 1 3 3
./practica4 1 3 3
echo "$line"
echo 7 EJECUCION CON CONSUMIDOR MAYOR QUE PRODUCTOR, ERROR
echo ./practica4 1 3 5
./practica4 1 3 5
echo "$line"
echo 8 EJECUCION CON CONSUMIDOR MENOR QUE PRODUCTOR, ERROR
echo ./practica4 1 5 3
./practica4 1 5 3
echo "$line"
echo 9 EJECUCION CON CAPACIDAD MAYOR A PRODUCTOR
echo ./practica4 10 5 5
./practica4 10 5 5
echo "$line"
echo 10 EJECUCION CON CAPACIDAD MENOR A 0, ERROR
echo ./practica4 -10 5 5
./practica4 -10 5 5
echo "$line"
echo 11 EJECUCION CON PRODUCTORES MENOR A 0, ERROR
echo ./practica4 10 -5 5
./practica4 10 -5 5
echo "$line"
echo 12 EJECUCION CON CONSUMIDORES MENOR A 0, ERROR
echo ./practica4 10 5 -5
./practica4 10 5 -5
echo "$line"
echo ******FIN DEL TEST*******
echo "$line"