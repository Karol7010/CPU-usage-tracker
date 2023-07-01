/*
The CPU usage tracker exclusively operates within the Linux environment.
*/

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdbool.h"
#include "pthread.h"
#include "threads.h" //This library contains all threads used in this program

struct CPU_s{   //'Class-like' structure used for storing CPU info and status
    int total_usage;
    int core_count;
};

void main(){
    /* Initializing threads, variables etc. */
    pthread_t reader;
    pthread_create(&reader, NULL, reader_f, NULL);

    /* aaa */
    while(1){
        printf("CPU usage tracker\n");
        sleep(10);
    };
    sleep(1);
    return NULL;
}