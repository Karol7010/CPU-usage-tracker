#include "stdio.h"
#include "stdbool.h"
#include <unistd.h>

void *reader_f(void *arg){
    bool thread_exit = false;
    char buffer[256];  // Buffer to hold the read line
    while(1){
        printf("reader:\n");
        FILE *file = fopen("/stat", "r"); //change to /proc/stat
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("\n %s", buffer);
            sleep(1);
        };
        fclose(file);
        sleep(3);
    };
    printf("end of thread\n");
    return NULL;
}