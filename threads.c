/* This library contains the thread's functions
    and their synchronization instructions. */

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "unistd.h"
#include "pthread.h"

/* Mutex and variables used for thread synchronization */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int currentThread = 0; //0-reader ; 1-analizer ; 2-printer
bool close_threads = false;
char *raw_data;

struct CPU_s{ //printer thread data
    int total_usage;
    int core_count;
};

void *reader_f(void *arg){
    /*
    This function reads a file that contains CPU usage information
    and passes it to a global variable. 
    */
    int threadId = 0;
    while(!close_threads){
        pthread_mutex_lock(&mutex);
        FILE *file = fopen("stat", "r"); //change to /proc/stat
        fseek(file, 0, SEEK_END); //determine length of buffer char array
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *buffer = (char*)malloc(fileSize * sizeof(char));
        fread(buffer, sizeof(char), fileSize, file); //reding stat file to buffer
        fclose(file);
        raw_data = buffer; //Sending current read to a global variable
        free(buffer);

        currentThread++;
        pthread_mutex_unlock(&mutex);
        
        /* Check if it's reader's turn */
        while (currentThread != threadId) { 
            pthread_mutex_unlock(&mutex);
            pthread_mutex_lock(&mutex);
        };
    };
    pthread_exit(NULL);
}