/**
 * Indica el error que se encuentra a través de un código de error que se pasa por parámetro
 * @param eCode código de error
 */
void check_error(int eCode);

/**
 * Indica el error que se encuentra a través de un código de error que se pasa por parámetro, además de la instancia
 * @param eCode código de error
 * @param i instancia de error
 */
void check_errori(int eCode, int i);

/**
 * Rutina de los Misioneros
 * Comentario: tratar las rutinas como las biografías de los misioneros, se suben, viajan, se bajan y continuan con sus vidas
 */
void* llegaMisionero();

/**
 * Rutina de los Canibales
 * Comentario: tratar las rutinas como las biografías de los canibales, se suben, viajan, se bajan y continuan con sus vidas
 */
void* llegaCanibal();

/**
 * Se encarga de crear los hilos
 * @param arg1 numero de hilos misioneros
 * @param arg2 numero de hilos canibales
 */
void threadCreate(int arg1, int arg2);

/**
 * Se encarga de comprobar que los argumentos son correctos, si no lo fuera, imprimiría por pantalla el mensaje de error correspondiente
 * Además, asignamos a las variables numero de misioneros y de los canibales los valores pasados por línea de órdenes
 */
void checkArguments(int argc, char *argv[], int *argM, int *argC);