#ifndef MY_LIB_H
#define MY_LIB_H

#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>

#include "gui.h"

#define MAX_THREAD_NUMBER 26
#define TIME_TO_SLEEP 1
#define STACK_SIZE 80096
#define TOTAL_TICKETS_AMOUNT 1000
#define WORKLOAD_UNIT 50

struct thread_metadata {
  int bills;
  int workload;
  int actual_workload;
  long double result;
};

enum thread_state {
  READY,
  WAITING,
  RUNNING,
  FINISHED,
  OPENED
};

struct threads_type{
  int id;
  sigjmp_buf env;
  char stack[STACK_SIZE];
  void *(function);
  enum thread_state state;
  struct thread_metadata data;
};


int number_of_threads;
int algorithm;
int quantum;
int work_limit;
int opt_mode;
int thread_workloads[MAX_THREAD_NUMBER-1];
int thread_bills[MAX_THREAD_NUMBER-1];
struct threads_type list_of_threads[MAX_THREAD_NUMBER];
struct threads_type current_thread;

void timer_quantum(int quantum_ms, void *function);
void save_current_state();
void stop_timer();
int rand_range(int low, int up);
void program_finished();
void get_next_thread_1();
void get_next_thread_2();
void signal_handler();
void Scheduler();
void Thread_finished();

void compute_arcsin(void);
void create_hilo(void *function, struct thread_metadata data);
void setup(void);

#endif
