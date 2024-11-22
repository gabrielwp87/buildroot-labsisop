#ifndef SETPRIORITY_H
#define SETPRIORITY_H

#include <pthread.h> // Inclua o cabeçalho pthread.h

#define SCHED_LOW_IDLE 7
#define SCHED_IDLE 5
#define SCHED_RR 2
#define SCHED_FIFO 1

void print_sched(int policy);
int setpriority(pthread_t *thr, int newpolicy, int newpriority);

#endif // SETPRIORITY_H