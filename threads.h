#ifndef THREADS_H
#define THREADS_H

bool *check_if_my_turn(int *threadId);
void *reader_f();
void *analizer_f(void *arg);
void *printer_f(void *arg);

#endif