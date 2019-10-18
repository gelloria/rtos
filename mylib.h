#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include "mythreads.c"

#define MAX_THREAD_NUMBER 26
#define TIME_TO_SLEEP 1
#define STACK_SIZE 40096

struct thread_metadata {
  int bills;
  int workload;
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

int thread_counter = 0;
int next_thread = 0;
int number_of_threads;
int algorithm;
int Quamtum;
int thread_workloads[MAX_THREAD_NUMBER];
int thread_bills[MAX_THREAD_NUMBER];
struct threads_type list_of_threads[MAX_THREAD_NUMBER];
struct threads_type current_thread;


void timer_quantum(int quantum_ms, void *function);
void save_current_state();
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
