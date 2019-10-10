#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "mythreads.c"
#include <stdlib.h>

#define SECOND 1000000
#define Number_Of_Threads 5
#define STACK_SIZE 40096

int thread_counter = 0;
int next_thread;

struct threads_type{
  int id;
  sigjmp_buf env;
  char stack[STACK_SIZE];
  void *(function);
  int arg1;
  int result;
};

struct threads_type list_of_threads[Number_Of_Threads];
struct threads_type current_thread;
struct threads_type previous_thread;

void FCFS_Scheduler(){
   if (next_thread == Number_Of_Threads) {
     next_thread = Number_Of_Threads;
   } else {
     next_thread = current_thread.id+1;
   }
   current_thread =list_of_threads[next_thread];
   siglongjmp(current_thread.env,1);
}

void sumador(void){

    int arg = current_thread.arg1;
    int current_thread_id = current_thread.id;

    for (int i = 0; i < 3; i++) {
      printf("Sumando (%d) en hilo (%d)\n",arg, current_thread_id);
      arg++;
     usleep(SECOND);
    }

    list_of_threads[current_thread_id].result = arg;
    FCFS_Scheduler();
}

void final(void) {
  for (size_t i = 1; i < Number_Of_Threads-1; i++) {
    printf("Resultado en Hilo (%d) es %d\n",  list_of_threads[i].id, list_of_threads[i].result);
  }
  exit(-1);
}


void create_hilo(void *function, int arg) {
  thread_counter++;

  printf("Creando Hilo %d\n", thread_counter);

  list_of_threads[thread_counter].id = thread_counter;
  list_of_threads[thread_counter].function = function;
  list_of_threads[thread_counter].arg1 = arg;
  list_of_threads[thread_counter].result = 0;

  address_t sp, pc;

  sp = (address_t)list_of_threads[thread_counter].stack + STACK_SIZE - sizeof(address_t);
  pc = (address_t)list_of_threads[thread_counter].function;

  sigsetjmp(list_of_threads[thread_counter].env,1);
  (list_of_threads[thread_counter].env->__jmpbuf)[JB_SP] = translate_address(sp);
  (list_of_threads[thread_counter].env->__jmpbuf)[JB_PC] = translate_address(pc);
  sigemptyset(&list_of_threads[thread_counter].env->__saved_mask);


}

int setup(void) {
  printf("Creando Hilo del main, ID = %d\n", thread_counter);

  list_of_threads[thread_counter].id = thread_counter;
  list_of_threads[thread_counter].function = NULL;
  list_of_threads[thread_counter].arg1 = thread_counter;
  list_of_threads[thread_counter].result = 0;

  sigset_t block_alarm;
	sigemptyset (&block_alarm);
	sigaddset (&block_alarm, SIGALRM);
	sigprocmask (SIG_UNBLOCK, &block_alarm, NULL);

  int main_setup = sigsetjmp(list_of_threads[thread_counter].env,1);
  return main_setup;
}


int main(){

    int main_setup = setup();

     if (main_setup == 0) {

       create_hilo(sumador, 10);
       create_hilo(sumador, 20);
       create_hilo(sumador, 30);
       create_hilo(final, 30);

       current_thread = list_of_threads[1];

       siglongjmp(current_thread.env,1);
     }

     printf("Estoy en el main\n");


    return 0;
}
