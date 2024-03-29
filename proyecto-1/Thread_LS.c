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
#define STACK_SIZE 80096
#define QUANTUM 2000 //ms

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
  // exit(-1);
  siglongjmp(list_of_threads[0].env,1);
}

void get_next_thread(){

  int salio = 0;

      for (size_t i = 1; i < Number_Of_Threads - 1; i++) {
        if(list_of_threads[i].state != FINISHED){
          next_thread = i;
          salio = 1;
          break;
        }
      }

      if(!salio) next_thread = Number_Of_Threads - 1;

      // if (next_thread==Number_Of_Threads-1 && list_of_threads[next_thread-1].state == 0 ){
      //   next_thread--;
      // }

  //    next_thread = queue_thread[0];

}

void Scheduler() {

  int coming_back = sigsetjmp(list_of_threads[current_thread.id].env,1);

	if(coming_back == 1){return;}

  get_next_thread();
  current_thread = list_of_threads[next_thread];

	if(current_thread.id != Number_Of_Threads-1){
		siglongjmp(current_thread.env, 1);
	}

program_finished(); //Finaliza el programa si current_id llego a ser el ultimo.

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

// function to find factorial of given number
long double factorial(int n)
{
    if (n == 0)
      return 1;

    return n * factorial(n - 1);
}

/*
long double arcsen(int x) {
  long double result = 0, numerador = 0, denominador = 0;

    // TODO: Este numero deberia de ser variable
    for (int n = 0; n < 878; n++) {
      //result+=(factorial(2*n) * 2*x**(2*n+1))/(   (2**(2*n))   *   (factorial(n)**2)  *  (2*n + 1));
      //result+=(factorial(2*n) * 2*pow(x, 2*n+1))/(   (pow(2, 2*n))   *   (pow(factorial(n), 2))  *  (2*n + 1));
      numerador = factorial(2*n) * pow(x, 2*n+1);
      denominador = pow(2, 2*n) * pow(factorial(n), 2) * (2*n + 1);
      //printf("NUMERADOR %Lf \n", numerador);
      //printf("DENOMINADOR %Lf \n", denominador);
      result += numerador / denominador;
    }

  return result;
}
*/

void sumador(void){

    int count = current_thread.data.workload;
    long double result = 0, numerador = 0, denominador = 0;

    timer_quantum(QUANTUM, signal_handler);

      for (int n = 0; n < count*50; n++) {
        // current_thread.data.result++;
        // printf("Resultado de Pi = (%d) en hilo (%d)\n",current_thread.data.result, current_thread.id);
        // sleep(TIME_TO_SLEEP);

        //    numerador = factorial(2*n) * pow(x, 2*n+1);
        //    denominador = pow(2, 2*n) * pow(factorial(n), 2) * (2*n + 1);

        numerador = 2*pow(-1, n);
        denominador = (1+2*n);
        //printf("NUMERADOR %Lf \n", numerador);
        //printf("DENOMINADOR %Lf \n", denominador);
        result += numerador / denominador;

      }

      //current_thread.data.workload--;

    current_thread.data.result = result*2;
    Thread_finished();
}


void create_hilo(void *function, struct thread_metadata data) {
  thread_counter++;

  printf("Creando Hilo %d\n", thread_counter);

  list_of_threads[thread_counter].id = thread_counter;
  list_of_threads[thread_counter].function = function;
  list_of_threads[thread_counter].data = data;
  list_of_threads[thread_counter].state = READY;
  list_of_threads[thread_counter].data.result = 0;
  list_of_threads[thread_counter].data.bills = 0;

  address_t sp, pc;

  sp = (address_t)list_of_threads[thread_counter].stack + STACK_SIZE - sizeof(address_t);
  pc = (address_t)list_of_threads[thread_counter].function;

  sigsetjmp(list_of_threads[thread_counter].env,1);
  (list_of_threads[thread_counter].env->__jmpbuf)[JB_SP] = translate_address(sp);
  (list_of_threads[thread_counter].env->__jmpbuf)[JB_PC] = translate_address(pc);
  sigemptyset(&list_of_threads[thread_counter].env->__saved_mask);
}

void setup(void) {
  printf("Creando Hilo del main, ID = %d\n", thread_counter);

  // sigset_t block_alarm;
  // sigemptyset (&block_alarm);
  // sigaddset (&block_alarm, SIGALRM);
  // sigprocmask (SIG_UNBLOCK, &block_alarm, NULL);

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

  struct thread_metadata thread_1;
  struct thread_metadata thread_2;
  struct thread_metadata thread_4;

  thread_1.workload = 100000;
  thread_2.workload = 3;

  create_hilo(sumador, thread_1);
  create_hilo(sumador, thread_2);
  create_hilo(final, thread_4);

  Scheduler();

  printf("\nExecution finished, message from  main.\n");

  fprintf(stderr, "Result 1 %.70Lf\n", list_of_threads[1].data.result);
  fprintf(stderr, "Result 2 %.70Lf\n", list_of_threads[2].data.result);
  return 0;
}
