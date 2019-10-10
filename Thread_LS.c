#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "mythreads.c"
#include <stdlib.h>

#define TIME_TO_SLEEP 250000
#define Number_Of_Threads 3
#define STACK_SIZE 80096
#define QUANTUM 5000 //ms

int thread_counter = 0;
int next_thread;

struct thread_metadata {
  int bills;
  int workload;
  int result;
};

struct threads_type{
  int id;
  sigjmp_buf env;
  char stack[STACK_SIZE];
  void *(function);
  struct thread_metadata thread_data;
};

struct threads_type list_of_threads[Number_Of_Threads];
struct threads_type current_thread;
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

int rand_range(int low, int up)
{
  return (rand() %(up - low + 1)) + low;
}

int Scheduler_run = 0;

void LS_Scheduler(){
  printf("Estoy en el LS_Scheduler(%d)\n", Scheduler_run);
  Scheduler_run++;

//  int coming_back = sigsetjmp(list_of_threads[1].env,1);

  printf("Estoy en %s, %d \n", __FUNCTION__, __LINE__);

  current_thread = list_of_threads[1];

  if (current_thread.thread_data.workload != 0){
    // srand(time(0));
    // next_thread = rand_range(1, Number_Of_Threads-1);
    printf("Estoy en %s, %d IF 1\n", __FUNCTION__, __LINE__);
    siglongjmp(list_of_threads[1].env,1);
  } else {
    printf("Estoy en %s, %d ELSE \n", __FUNCTION__, __LINE__);
    siglongjmp(list_of_threads[2].env,1);
  }
}


void signal_handler(){
	printf("\nTiempo expiró, saltando hilo\n\n");
  LS_Scheduler();
}

void sumador(void){

//    timer_quantum(QUANTUM, signal_handler);

    int count = current_thread.thread_data.workload;

    printf("Estoy en %s, %d WORKLOAD %d\n", __FUNCTION__, __LINE__, count);

    for (int a = 0; a < count; a++) {
      printf("Estoy en FOR 1 %s, %d A %d, WORKLOAD %d\n", __FUNCTION__, __LINE__, a, current_thread.thread_data.workload);
      for (int b = 0; b < 25; b++) {
        printf("Estoy en FOR 2 %s, %d \n", __FUNCTION__, __LINE__);

        printf("RESULT %d\n", current_thread.thread_data.result);

        current_thread.thread_data.result++;
        printf("Resultado de Pi = (%d) en hilo (%d)\n",current_thread.thread_data.result, current_thread.id);
        usleep(TIME_TO_SLEEP);
      }
      current_thread.thread_data.workload--;
    }
    int a = 1;
    siglongjmp(list_of_threads[0].env,1);
}


void create_hilo(void *function, struct thread_metadata thread_data) {
  thread_counter++;

  printf("Creando Hilo %d\n", thread_counter);

  list_of_threads[thread_counter].id = thread_counter;
  list_of_threads[thread_counter].function = function;
  list_of_threads[thread_counter].thread_data = thread_data;
  list_of_threads[thread_counter].thread_data.result = 0;
  list_of_threads[thread_counter].thread_data.bills = 0;

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
  list_of_threads[thread_counter].thread_data.result = 0;

  int main_setup = sigsetjmp(list_of_threads[thread_counter].env,1);
  return main_setup;
}

void final(void) {
  printf("Final");
  exit(-1);
}


int main(){

  int main_setup = setup();
  int a = 0;

  if (main_setup == 0) {

    struct thread_metadata thread_1;
    struct thread_metadata thread_4;

    thread_1.workload = 2;

    create_hilo(sumador, thread_1);
    create_hilo(final, thread_4);


    siglongjmp(list_of_threads[0].env,1);
  }

    printf("Estoy en el main\n");

    LS_Scheduler();

    return 0;
}
