#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "mythreads.c"
#include <stdlib.h>
#include <time.h>

#define SECOND 1000000
#define Number_Of_Threads 5
#define STACK_SIZE 40096

int thread_counter = 0;
int next_thread;
int rand_num;
int lower = 1, upper = 4;

struct threads_type{
  int id;
  sigjmp_buf env;
  char stack[STACK_SIZE];
  void *(function);
  float arg1;
  float result;
};

struct threads_type list_of_threads[Number_Of_Threads];
struct threads_type current_thread;
struct threads_type previous_thread;

int rand_range(int low, int up)
{
  return (rand() %(up - low + 1)) + low;
}

void lottery_scheduler(){
   if (next_thread == Number_Of_Threads) {
     next_thread = Number_Of_Threads;
   } else {
     next_thread = current_thread.id+1;
   }
   current_thread =list_of_threads[next_thread];
   siglongjmp(current_thread.env,1);
}

void lottery_scheduler(){
  while (current_thread.arg1 != 0) {
    srand(time(0));
    next_thread = rand_range(lower, upper);
    current_thread =list_of_threads[next_thread];
    siglongjmp(current_thread.env,1);
  }
}

void sumador(void){
    float arg = current_thread.arg1;
    int current_thread_id = current_thread.id;

    list_of_threads[current_thread_id].result +=0.01;
    list_of_threads[current_thread_id].arg1 -=1;
    printf("Resultado parcial para el hilo (%d) es %f\n",  current_thread_id, list_of_threads[current_thread_id].result);
    usleep(SECOND/100);
    lottery_scheduler();
  }

void final(void) {
  for (size_t i = 1; i < Number_Of_Threads-1; i++) {
    printf("Resultado en Hilo (%d) es %f\n",  list_of_threads[i].id, list_of_threads[i].result);
  }
  siglongjmp(list_of_threads[0].env,1);
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
  int main_setup = sigsetjmp(list_of_threads[thread_counter].env,1);
  return main_setup;
}


int main(){
    int main_setup = setup();
     if (main_setup == 0) {
       create_hilo(sumador, 314);
       create_hilo(sumador, 314);
       create_hilo(sumador, 314);
       create_hilo(final, 3);
       current_thread = list_of_threads[1];
       lottery_scheduler();
     }
     printf("Estoy en el main\n");
    return 0;
}
