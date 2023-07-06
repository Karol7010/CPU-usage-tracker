#ifndef THREADS_H
#define THREADS_H

#include "stdbool.h"

extern bool close_threads;
int check_if_my_turn(int threadId);
void reader_f();
void analizer_f();
void printer_f();
void logger_f();
void watchdog_f();

#endif