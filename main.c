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
    pthread_create(&reader, NULL, reader_f, NULL);
    pthread_create(&analizer, NULL, analizer_f, NULL);
    pthread_create(&printer, NULL, printer_f, NULL);

    /* waits for user input to close threads and end the program */
    //printf("Press any key to continue...   \n");
    //getchar();
    //close_threads = true;
    pthread_join(reader, NULL);
    pthread_join(analizer, NULL);
    pthread_join(printer, NULL);
    printf("Program terminated gracefully.\n");
    sleep(1);
    exit(0);
}