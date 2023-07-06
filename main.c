/*
The CPU usage tracker exclusively operates within 
the Linux environment.
*/

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdbool.h"
#include "pthread.h"
#include "threads.h" 

void main(){
    /* Initializing threads */
    pthread_t reader;
    pthread_t analizer;
    pthread_t printer;
    pthread_t logger;
    pthread_t watchdog;
    pthread_create(&reader, NULL, (void*)reader_f, NULL);
    pthread_create(&analizer, NULL, (void*)analizer_f, NULL);
    pthread_create(&printer, NULL, (void*)printer_f, NULL);
    pthread_create(&logger, NULL, (void*)logger_f, NULL);
    pthread_create(&watchdog, NULL, (void*)watchdog_f, NULL);

    /* waits for user input to close threads and end the program */
    getchar();
    close_threads = true;
    pthread_join(reader, NULL);
    pthread_join(analizer, NULL);
    pthread_join(printer, NULL);
    pthread_join(logger, NULL);
    pthread_join(watchdog, NULL);
    printf("Program terminated gracefully.\n");
    sleep(1);
    exit(0);
}