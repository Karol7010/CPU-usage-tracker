/* This library contains threads functions
    and their synchronization instructions. */

#include "threads.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "unistd.h"
#include "pthread.h"
#include "string.h"
#include "time.h"

/* Mutex and variables used for thread synchronization */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int currentThread = 0; //0-reader ; 1-analizer ; 2-printer
bool close_threads = false;
char *raw_data;
struct tm *localTime;
time_t currentTime;
char timeString[50];

/* struct for keeping numerical values read from the stat file */
struct CPU {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    unsigned long long total, idle_time, total_time, prev_idle, prev_total;
    float cpu_usage;
};
struct CPU cpu;

int check_if_my_turn(int threadId){
    /* Pauses thread until ok to run */
    while (currentThread != threadId) { 
            pthread_mutex_lock(&mutex);
            pthread_mutex_unlock(&mutex);
    };
}

void reader_f(){
    /*
    This function peaks into a file /proc/stat that contains CPU usage 
    information and passes it to a global variable 'raw_data'.
    */
    while(!close_threads){
        check_if_my_turn(0);
        FILE *file = fopen("/proc/stat", "r");
        if (file == NULL) {
            printf("Failed to open /proc/stat.\n");
            system("pause");
        }
        char buffer[100];
        fgets(buffer, sizeof(buffer), file); //reading only first line of stat file
        fclose(file);
        raw_data = buffer; //Sending current read to a global variable
        currentThread++;
    };
    pthread_exit(NULL);
}

void analizer_f(){
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
                cpu.cpu_usage = (float)(cpu.total_time - (cpu.prev_idle - cpu.idle_time)) / cpu.total_time * 50;

                cpu.prev_idle = cpu.idle_time;
                cpu.prev_total = cpu.total;
            }
        }
        currentThread++;
    };
    pthread_exit(NULL);
}

void printer_f(){
    /*
    This function prints 'cpu.cpu_usage' in a nice way
    */
    printf("Press ENTER to exit...\n");
    while(!close_threads){
        check_if_my_turn(2);
        printf("CPU Usage: [");
        for (int i = 0; i < 100; i += 10) { //bar display
            if (i < cpu.cpu_usage) {
                printf("=");
            } else {
                printf(" ");
            }
        }
        printf("] %.2f%%\r", cpu.cpu_usage);
        fflush(stdout);
        sleep(1); //refresh after a second
        currentThread++;
    };
    pthread_exit(NULL);
}

void logger_f(){
    /*
    Logs current time and CPU usage into a file (log_data.txt)
    */
    check_if_my_turn(3);
    FILE *file = fopen("log_data", "w");
    if (file == NULL) {
        printf("Failed to create the logger file.\n");
        pthread_exit(NULL);
    }
    while(!close_threads){
        check_if_my_turn(3);
        localTime = localtime(&currentTime);
        currentTime = time(NULL);
        strftime(timeString, sizeof(timeString), "%c", localTime);
        fprintf(file, "Time: %s ; CPU: %.2f%%\r\n", timeString, cpu.cpu_usage);
        currentThread = 0;
    }
    fclose(file);
    pthread_exit(NULL);
}

void watchdog_f(){
    /*
    Checks if time variable is updated every 2 seconds, if not
    the program is terminated.
    */
    while(!close_threads){
        time_t time_1 = currentTime;
        sleep(2);
        time_t time_2 = currentTime;
        if (time_1 == time_2){
            printf("\nThe program is stuck.\n");
            sleep(0.5);
            exit(0);
        }
    }
    pthread_exit(NULL);
}