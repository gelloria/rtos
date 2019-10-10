#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "mythreads.c"

#define TIME_TO_SLEEP 250000 //us
#define Number_Of_Threads 3
#define STACK_SIZE 40096
#define QUANTUM 2000 //ms

int thread_counter = 0;
int next_thread;

struct thread_metadata {
  int bills;
  int workload;
};

struct threads_type{
  int id;
  sigjmp_buf env;
  char stack[STACK_SIZE];
  void *(function);
  struct thread_metadata thread_data;
};

void timer_quantum(int quantum_ms, void *function){
	struct itimerval timer;

  int sec = quantum_ms / 1000;
  int usec = (quantum_ms % 1000 ) * 1000;

   /* Configure the timer to expire after X sec... */
   timer.it_value.tv_sec = sec;
   timer.it_value.tv_usec = usec;
   /* ... and every X sec after that. */
   timer.it_interval.tv_sec = sec;
   timer.it_interval.tv_usec = usec;

	signal(SIGALRM, function);
	setitimer(ITIMER_REAL, &timer, NULL);
}

struct threads_type list_of_threads[Number_Of_Threads];
struct threads_type current_thread;
struct threads_type previous_thread;

void Scheduler(){
    int coming_back = sigsetjmp(list_of_threads[current_thread.id].env,1);

    if (coming_back == 1) {return;} //Returns no normal execution of the function.
    next_thread = current_thread.id+1;

    if (next_thread == Number_Of_Threads) {next_thread = 0;} //Loops the functions.
    current_thread =list_of_threads[next_thread];

    siglongjmp(current_thread.env,1);
}

void signal_handler(){
	printf("\nTiempo expiró, saltando hilo\n\n");
  Scheduler();
}


void sumador(void){
    int i=0;

    timer_quantum(QUANTUM, signal_handler);

    while(1) {
        ++i;
        printf("Sumando (%d) en hilo (%d)\n",i, current_thread.id);
        usleep(TIME_TO_SLEEP);
    }
}


void create_hilo(void *function, struct thread_metadata thread_data) {

  printf("Creando Hilo %d\n", thread_counter);

  list_of_threads[thread_counter].id = thread_counter;
  list_of_threads[thread_counter].function = function;
  list_of_threads[thread_counter].thread_data = thread_data;

  address_t sp, pc;

  sp = (address_t)list_of_threads[thread_counter].stack + STACK_SIZE - sizeof(address_t);
  pc = (address_t)list_of_threads[thread_counter].function;

  sigsetjmp(list_of_threads[thread_counter].env,1);
  (list_of_threads[thread_counter].env->__jmpbuf)[JB_SP] = translate_address(sp);
  (list_of_threads[thread_counter].env->__jmpbuf)[JB_PC] = translate_address(pc);
  sigemptyset(&list_of_threads[thread_counter].env->__saved_mask);

  thread_counter++;
}

int main(){

  struct thread_metadata thread_1;
  struct thread_metadata thread_2;
  struct thread_metadata thread_3;

  thread_1.workload = 50;
  thread_2.workload = 250;
  thread_3.workload = 150;

    create_hilo(sumador, thread_1);
    create_hilo(sumador, thread_2);
    create_hilo(sumador, thread_3);


    current_thread = list_of_threads[1];

    siglongjmp(current_thread.env,1);
    return 0;
}
