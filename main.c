/*
This is a multi-threaded program for tracking CPU usage 
in a Linux environment. It initializes several threads 
for different tasks, such as reading CPU usage, analyzing 
the data, printing results, logging, and a watchdog thread.
*/

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdbool.h"
#include "pthread.h"
#include "signal.h"
#include "threads.h" 

volatile sig_atomic_t close_program = 0;

void handle_sigterm(int signum) {
    close_program = 1;
}

int main(){
    signal(SIGTERM, handle_sigterm);

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

    /* waits for user input or SIGTERM signal to end the program */
    while (!close_program && getchar() == EOF);
    close_threads = true;
    pthread_join(reader, NULL);
    pthread_join(analizer, NULL);
    pthread_join(printer, NULL);
    pthread_join(logger, NULL);
    pthread_join(watchdog, NULL);
    printf("\nThe program terminated gracefully\n");
    return 0;
}