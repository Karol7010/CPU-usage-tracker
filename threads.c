/* This library contains the thread's functions
    and their synchronization instructions. */

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "unistd.h"
#include "pthread.h"
#include "string.h"

/* Mutex and variables used for thread synchronization */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int currentThread = 0; //0-reader ; 1-analizer ; 2-printer
bool close_threads = false;
char *raw_data;

// struct CPU {
//     int core_count;
//     long cpu[];
//     long cpu_cores[core_count][];
//     long intr[];
// };

bool check_if_my_turn(int threadId){
    /* Pauses thread */
    while (currentThread != threadId) { 
            pthread_mutex_unlock(&mutex);
            pthread_mutex_lock(&mutex);
    };
}

void *reader_f(){
    /*
    This function peaks into a file that contains CPU usage 
    information and passes it to a global variable.
    */
    while(!close_threads){
        check_if_my_turn(0);
        printf("Entering reader\n");
        FILE *file = fopen("stat", "r"); //change to /proc/stat
        fseek(file, 0, SEEK_END); //determine length for buffer char array
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *buffer = (char*)malloc(fileSize * sizeof(char));
        fread(buffer, sizeof(char), fileSize, file); //reding stat file to buffer
        fclose(file);
        raw_data = buffer; //Sending current read to a global variable
        currentThread++;
    };
    pthread_exit(NULL);
}

void *analizer_f(void *arg){
    /*
    _Percentage = (totald - idled)/totald
    */
    while(!close_threads){
        check_if_my_turn(1);
        printf("Entering analizer\n");

        /* Split the raw data to a 2D matrix (split by new line) */
        char matrix[24][2048];
        int row = 0;
        int col = 0;
        int data_length = strlen(raw_data); // Get the length of the string
        for (int i = 0; i < data_length; i++) {
            if (raw_data[i] == '\n') {
                matrix[row][col] = '\0';  // Null-terminate the string
                row++;
                col = 0;
            } 
            else{
                matrix[row][col] = raw_data[i];
                col++;
            }
        }
        currentThread++;
    };
    pthread_exit(NULL);
}

void *printer_f(){
    /*
    aaa
    */
    while(!close_threads){
        check_if_my_turn(2);
        printf("Entering printer\n");

        //printf("%s\n", raw_data);

        currentThread++;
    };
    pthread_exit(NULL);
}