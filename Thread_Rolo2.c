#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "mythreads.c"

#define SECOND 1000000
#define Number_Of_Threads 3
#define STACK_SIZE 40096

int thread_counter = 0;
int next_thread;

struct threads_type{
  int id;
  sigjmp_buf env;
  char stack[STACK_SIZE];
  void *(function);
  int arg1;
  float result;
  int lottery_weight;

};

struct threads_type list_of_threads[Number_Of_Threads];
struct threads_type current_thread;
struct threads_type previous_thread;

void dummy_scheduler(){
    int coming_back = sigsetjmp(list_of_threads[current_thread.id].env,1);

    if (coming_back == 1) {return;} //Returns no normal execution of the function.
    next_thread = current_thread.id+1;

    if (next_thread == Number_Of_Threads) {next_thread = 0;} //Loops the functions.
    current_thread =list_of_threads[next_thread];

    siglongjmp(current_thread.env,1);
}



void sumador(void){

    int arg = current_thread.arg1;
    int i=arg;
    while(1) {
        ++i;
        printf("Sumando (%d) en hilo (%d), arg (%d)\n",i, current_thread.id, arg);
        if (i % 3 == 0) {
            printf("sumador: switching\n");
            signal(SIGALRM, SIG_DFL);
            dummy_scheduler();
        }
        usleep(SECOND);
    }
}


void create_hilo(void *function, int arg) {

  printf("Creando Hilo %d\n", thread_counter);

  list_of_threads[thread_counter].id = thread_counter;
  list_of_threads[thread_counter].function = function;
  list_of_threads[thread_counter].arg1 = arg;

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


    create_hilo(sumador, 69);
    create_hilo(sumador, 80);
    create_hilo(sumador, 30);

    int Thread_To_Run = 0;

    current_thread = list_of_threads[Thread_To_Run];

    siglongjmp(current_thread.env,1);
    return 0;
}
