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

struct CPU {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    unsigned long long total, idle_time, total_time, prev_idle, prev_total;
    float cpu_usage;
};
struct CPU cpu;

bool check_if_my_turn(int threadId){
    /* Pauses thread */
    while (currentThread != threadId) { 
            pthread_mutex_unlock(&mutex);
            pthread_mutex_lock(&mutex);
    };
}

void *reader_f(){
    /*
    This function peaks into a file /proc/stat that contains CPU usage 
    information and passes it to a global variable 'raw_data'.
    */
    while(!close_threads){
        check_if_my_turn(0);
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
    This function processes the raw_data, saves the int values to CPU struct,
    than calculates the CPU usage based on the idle and total time. 
    */
    cpu.prev_idle = 0; 
    cpu.prev_total = 0;
    while(!close_threads){
        check_if_my_turn(1);

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

        /* Find  'cpu ' line,  change char to int values and calculate the CPU usage */
        row = sizeof(matrix) / sizeof(matrix[0]);
        col = sizeof(matrix[0]) / sizeof(matrix[0][0]);
        for (int i = 0; i < row; i++) {
            if (strncmp(matrix[i], "cpu ", 4) == 0) { //extract int values from line starting with 'cpu '
                sscanf(matrix[i], "cpu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
                &cpu.user, &cpu.nice, &cpu.system, &cpu.idle, &cpu.iowait, &cpu.irq,
                &cpu.softirq, &cpu.steal, &cpu.guest, &cpu.guest_nice);
                cpu.total = cpu.user + cpu.nice + cpu.system + cpu.idle + cpu.iowait + cpu.irq + cpu.softirq + cpu.steal;
                cpu.idle_time = cpu.idle + cpu.iowait;
                cpu.total_time = cpu.total - (cpu.prev_total);

                /* Actual CPU usage calculation */
                cpu.cpu_usage = (float)(cpu.total_time - (cpu.prev_idle - cpu.idle_time)) / cpu.total_time * 100;

                cpu.prev_idle = cpu.idle_time;
                cpu.prev_total = cpu.total;
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

        printf("CPU Usage: %.2f%%\n", cpu.cpu_usage);
        sleep(1);

        currentThread = 0;
    };
    pthread_exit(NULL);
}