// Comprueba errores e imprime mensaje si encuentra alguno
void check_error(int return_code, const char* msg);
// Hilo Productor: inserta un item en el búfer
void *Productor(void* item);
// Hilo Consumidor: extrae un item del búfer
void *Consumidor(void* no_se_usa);
// Espera a que termine un lote de hilos; el lote es un vector de pthread_t
void espera_lote_hilos (pthread_t* lote, int nhilos);
// Rutina que lanza los hilos productores y consumidores
void test_hilos (int nprod, int ncons);