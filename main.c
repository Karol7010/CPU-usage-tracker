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
    pthread_create(&reader, NULL, reader_f, NULL);


    /* aaa */
    while(1){
        sleep(10);
    };
}