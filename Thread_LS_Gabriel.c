#include "mylib.h"


// function to find factorial of given number
long double factorial(int n){
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

void compute_arcsin(void){
    int count = current_thread.data.workload;
    // timer_quantum(Quamtum, signal_handler);
    long double result = 0;
      for (int n = 0; n < 50*count; n++) {
        // numerador = factorial(2*n) * pow(x, 2*n+1);
        // denominador = pow(2, 2*n) * pow(factorial(n), 2) * (2*n + 1);
        current_thread.data.result += ( 2*pow(-1, n) / (1+2*n));
        // current_thread.data.result += numerador / denominador;
      }
      current_thread.data.result= current_thread.data.result*2;
    Thread_finished();
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
  printf("\nNo More Threads!\n\n");
  for (int i = 1; i < number_of_threads+1; i++) {
    printf("Final result of Thread #%d is %.30Lf.\n", list_of_threads[i].id, list_of_threads[i].data.result );
  }
  siglongjmp(list_of_threads[0].env,1);
}

void get_next_thread_1(){

  int j=0;
  int rand_num = 0;
  int winner_bill=  0;
  int total_bills = 0;
  int available_bills[100];


  // Get available threads and bills
  for (int i = 1; i < number_of_threads+1; i++) {
    if(list_of_threads[i].state != FINISHED){
      total_bills +=list_of_threads[i].data.bills;
      // Builds an array of bills with thread_ids
      for (int k = 0; k < list_of_threads[i].data.bills; k++) {
        available_bills[j]= list_of_threads[i].id;
        j++;
      }
    }
  }
//  printf("total_amount_tickets = %d\n", total_bills);

//  printf("total_bills = { " );
  // for (int y = 0; y < total_bills; y++) {
  //   printf("%d, ", available_bills[y]);
  // }
  // printf(" } \n");

  if (total_bills == 0) {
    next_thread = MAX_THREAD_NUMBER;
  }else{
    // Lottery bill trow
    srand(time(0));
    rand_num = rand_range(0, total_bills-1);
    next_thread = available_bills[rand_num];
  }

//  printf("next_thread will be = %d\n", next_thread);
}

void get_next_thread_2(){
  next_thread++;

  if (next_thread == number_of_threads+1) {
    next_thread = MAX_THREAD_NUMBER;
  }

  // int salio = 0;
  //
  //     for (size_t i = 1; i < number_of_threads; i++) {
  //       if(list_of_threads[i].state != FINISHED){
  //         next_thread = i;
  //         salio = 1;
  //         break;
  //       }
  //     }
  //
  //     if(!salio) next_thread = MAX_THREAD_NUMBER;
  //
  //     // if (next_thread==Number_Of_Threads-1 && list_of_threads[next_thread-1].state == 0 ){
  //     //   next_thread--;
  //     // }
  //
  // //    next_thread = queue_thread[0];

  stop_timer();
}

void signal_handler(){
	printf(" -> Time expired, jumping to next thread!, Partial Result = %LF\n\n", current_thread.data.result);
  save_current_state();
  Scheduler();
}

void Scheduler() {

  int coming_back = sigsetjmp(list_of_threads[current_thread.id].env,1);

	if(coming_back == 1){return;}

  if (algorithm == 1) {
    get_next_thread_1();
   timer_quantum(Quamtum, signal_handler);
  } else {
    get_next_thread_2();
  }

  current_thread = list_of_threads[next_thread];

  if (next_thread == MAX_THREAD_NUMBER) {
    program_finished(); //Finaliza el programa si current_id llego a ser el ultimo.
  } else {
    printf("Running Thread (%d)", current_thread.id);
    siglongjmp(current_thread.env, 1);
  }

}

void Thread_finished(){
  printf(" -> Thread (%d) Finished!\n", current_thread.id);
  current_thread.state = FINISHED;
  save_current_state();
  Scheduler();
}


void create_hilo(void *function, struct thread_metadata data) {
  thread_counter++;
  address_t sp, pc;

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

  list_of_threads[thread_counter].id = thread_counter;
  list_of_threads[thread_counter].function = NULL;
  list_of_threads[thread_counter].data.result = 0;

  int main_setup = sigsetjmp(list_of_threads[thread_counter].env,1);

}

int menu( int argc, char **argv, char **in_file){
	int option_index = 0;
  FILE *fp;
	while (( option_index = getopt(argc, argv, ":f:a:h")) != -1){
		switch (option_index) {
			case 'f':
        fp = fopen(optarg, "r");
        if(fp == NULL){
            printf("Error opening file\n");
            exit(1);
        }
        fscanf(fp, "Algorithm = %d\n\nNumber_Of_Threads = %d\nQuamtum = %d", &algorithm, &number_of_threads, &Quamtum);

        if (algorithm == 1) {
          printf("Using Lottery Scheduler - Number of Threads = %d\n\n" , number_of_threads);
        } else if (algorithm == 2){
            printf("Using FCFS - Number of Threads = (%d)\n\n" , number_of_threads);
        }
          else {
            printf("Invalid Scheduling Algorithim: Using Lottery Scheduler as default\n");
            algorithm = 1;
          }

				for (int i = 0; i < number_of_threads; i++){
		        fscanf(fp, "%d,", &thread_bills[i] );
		    }

				for (int i = 0; i < number_of_threads; i++){
						fscanf(fp, "%d,", &thread_workloads[i] );
				}
        // for (int i = 0; i < number_of_threads; i++){
        //     printf("thread_bills is: %d\n", thread_bills[i]);
        // }
				// for (int i = 0; i < number_of_threads; i++){
				// 		printf("thread_workload is: %d\n", thread_workloads[i]);
				// }
        fclose(fp);

				break;
			case 'a':
				printf("Authors of the program:\n\tLeonardo Araya\n\tGabriel Loria\n\tRolando Moraga\n");
				break;
			case 'h':
				printf("Use '-f configuration_file' to set the configuration file.\n");
				return 1;
				break;
			default:
				printf("Not recognized option. Please use '-h' for help.\n");
				return 1;
		}
	}
	return 0;
}


int main(int argc, char **argv){

  char *config_file_name;
	int status = 0;

	status = menu(argc,argv, &config_file_name);
	if (status) {
    printf("Status is %d\n", status);
		return status;
	}

  setup();
  struct thread_metadata thread_generic_tmp;


  for (size_t i = 1; i < number_of_threads+1; i++) {
    thread_generic_tmp.workload = thread_workloads[i-1];
    thread_generic_tmp.bills = thread_bills[i-1];
    thread_generic_tmp.result = 0;
    create_hilo(compute_arcsin, thread_generic_tmp);
  }


  Scheduler();

  printf("\nExecution finished, message from  main.\n");
  return 0;
}
