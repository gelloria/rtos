#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "mythreads.c"
#include <stdlib.h>
#include <math.h>

#define Number_Of_Threads 4
#define TIME_TO_SLEEP 1
#define STACK_SIZE 40096
#define QUANTUM 200 //ms

int thread_counter = 0;
int next_thread = 0;

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

struct threads_type list_of_threads[Number_Of_Threads];
struct threads_type current_thread;

// function to find factorial of given number
long double factorial(int n)
{
    if (n == 0)
      return 1;

    return n * factorial(n - 1);
}

long double arcsen(int x) {
  long double result = 0, numerador = 0, denominador = 0;
    // TODO: Este numero deberia de ser variable
    for (int n = 0; n < 100000; n++) {
      result+= ( pow(-1, n) / (1+2*n));
    }

  return result;
}

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

	if(function != NULL) {signal(SIGALRM, function);}
	setitimer(ITIMER_REAL, &timer, NULL);
}

void save_current_state(){

  list_of_threads[current_thread.id].id = current_thread.id;
  list_of_threads[current_thread.id].function = current_thread.function;
  list_of_threads[current_thread.id].state = current_thread.state;
  list_of_threads[current_thread.id].data.result = current_thread.data.result;

}

void stop_timer(){
  timer_quantum(0, NULL);
}

int rand_range(int low, int up){
  return (rand() %(up - low + 1)) + low;
}

void program_finished(){
  printf("Program Finished!\n");
  for (int i = 1; i < Number_Of_Threads; i++) {
    printf("Resultado final del Thread %d es %.30Lf.\n", list_of_threads[i].id, list_of_threads[i].data.result );
  }
  siglongjmp(list_of_threads[0].env,1);
}

void get_next_thread(){

  int j=0;
  int rand_num = 0;
  int winner_bill=  0;
  int total_bills = 0;
  int available_bills[100];

  // Get available threads and bills
  for (int i = 1; i < Number_Of_Threads; i++) {
    if(list_of_threads[i].state != FINISHED){
      total_bills +=list_of_threads[i].data.bills;
      // Builds an array of bills with thread_ids
      for (int k = 0; k < list_of_threads[i].data.bills; k++) {
        available_bills[j]= list_of_threads[i].id;
        j++;
      }
    }
  }
  printf("total_amount_tickets = %d\n", total_bills);

  printf("total_bills = { " );
  for (int y = 0; y < total_bills; y++) {
    printf("%d, ", available_bills[y]);
  }
  printf(" } \n");

  if (total_bills == 0) {
    next_thread = 4;
  }else{
    // Lottery bill trow
    srand(time(0));
    rand_num = rand_range(0, total_bills-1);
    next_thread = available_bills[rand_num];
  }

  printf("next_thread will be = %d\n", next_thread);
}

void Scheduler() {

  int coming_back = sigsetjmp(list_of_threads[current_thread.id].env,1);

	if(coming_back == 1){return;}

  get_next_thread();
  current_thread = list_of_threads[next_thread];

  if (next_thread ==4) {
    program_finished(); //Finaliza el programa si current_id llego a ser el ultimo.
  } else {
    siglongjmp(current_thread.env, 1);
  }

}

void Thread_finished(){
  printf("Termine un thread\n");
  current_thread.state = FINISHED;
  save_current_state();
  Scheduler();
}


void signal_handler(){
	printf("\nTiempo expiró, saltando hilo\n\n");
  save_current_state();
  Scheduler();
}

void sumador(void){
    int count = current_thread.data.workload;
    timer_quantum(QUANTUM, signal_handler);
    long double result = 0;
      // TODO: Este numero deberia de ser variable
      for (int n = 0; n < count; n++) {
        current_thread.data.result += ( pow(-1, n) / (1+2*n));
        // printf("Resultado de Pi = (%.30Lf) en hilo (%d)\n",current_thread.data.result, current_thread.id);
        // sleep(TIME_TO_SLEEP);
        // current_thread.data.workload--;
      }
      current_thread.data.result= current_thread.data.result*4;
    Thread_finished();
}


void create_hilo(void *function, struct thread_metadata data) {
  thread_counter++;
  address_t sp, pc;

  printf("Creando Hilo %d\n", thread_counter);

  list_of_threads[thread_counter].id = thread_counter;
  list_of_threads[thread_counter].function = function;
  list_of_threads[thread_counter].data = data;
  list_of_threads[thread_counter].state = READY;

  sp = (address_t)list_of_threads[thread_counter].stack + STACK_SIZE - sizeof(address_t);
  pc = (address_t)list_of_threads[thread_counter].function;

  sigsetjmp(list_of_threads[thread_counter].env,1);
  (list_of_threads[thread_counter].env->__jmpbuf)[JB_SP] = translate_address(sp);
  (list_of_threads[thread_counter].env->__jmpbuf)[JB_PC] = translate_address(pc);
  sigemptyset(&list_of_threads[thread_counter].env->__saved_mask);
}

void setup(void) {
  printf("Creando Hilo del main, ID = %d\n", thread_counter);

  list_of_threads[thread_counter].id = thread_counter;
  list_of_threads[thread_counter].function = NULL;
  list_of_threads[thread_counter].data.result = 0;

  int main_setup = sigsetjmp(list_of_threads[thread_counter].env,1);

}

void final(void) {
  printf("Final\n");
  exit(-1);
}


int main(){

  setup();
  // thread 0 = setup
  //Thread 1-3 = operaciones
  struct thread_metadata thread_1;
  struct thread_metadata thread_2;
  struct thread_metadata thread_3;
  // Thread 4 = final
  struct thread_metadata thread_4;

  thread_1.workload = 1000;
  thread_1.bills = 1;
  thread_1.result = 0;

  thread_2.workload = 10000;
  thread_2.bills = 1;
  thread_2.result = 0;

  thread_3.workload = 100000;
  thread_3.bills = 1;
  thread_3.result = 0;

  create_hilo(sumador, thread_1);
  create_hilo(sumador, thread_2);
  create_hilo(sumador, thread_3);
  create_hilo(final, thread_4);

  Scheduler();

  printf("\nExecution finished, message from  main.\n");
  return 0;
}
