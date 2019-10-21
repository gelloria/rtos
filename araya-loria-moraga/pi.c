#include "mylib.h"
#include "mythreads.c"

int thread_counter = 0;
int next_thread = 0;

void sleepsec(int n) {
	for (size_t i = 0; i < n; i++) {
		//HELLO WORLD
	}
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
    int actual_workload = 0;

    for (int i = 0; i < count; i++) {
      for (int n = 0; n < 50; n++) {
        current_thread.data.result += 2*( 2*pow(-1, actual_workload) / (1+2*actual_workload));
				current_thread.data.actual_workload = actual_workload;

				// Sleep used to avoid a super-fast termination
				sleepsec(4000);
        actual_workload++;
      }
      if ( ((i+1)%work_limit == 0) && (algorithm == 1) && (opt_mode == 0)) {
          printf("Current Workload (%d) , Workload Expired!\n", i);
          printf("Workload expired, jumping to next thread!, Partial Result = %LF\n\n", current_thread.data.result);
					current_thread.state = WAITING;
					current_thread.data.actual_workload = actual_workload;
          save_current_state();
          Scheduler();
      }
    }
      // current_thread.data.result = current_thread.data.result*2;
    Thread_finished();
}

void save_current_state(){

  list_of_threads[current_thread.id].id = current_thread.id;
  list_of_threads[current_thread.id].function = current_thread.function;
  list_of_threads[current_thread.id].state = current_thread.state;
  list_of_threads[current_thread.id].data.result = current_thread.data.result;
	list_of_threads[current_thread.id].data.actual_workload = current_thread.data.actual_workload;

}

void stop_timer(){
  timer_quantum(0, NULL);
}

int rand_range(int low, int up){
	srand(time(0));
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
  // int winner_bill=  0;
  int total_bills = 0;
  int available_bills[TOTAL_TICKETS_AMOUNT];



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

  if (total_bills == 0) {
    next_thread = MAX_THREAD_NUMBER;
  }else{
    // Lottery bill trow
    rand_num = rand_range(0, total_bills-1);
    next_thread = available_bills[rand_num];
  }

}

void get_next_thread_2(){
  next_thread++;

  if (next_thread == number_of_threads+1) {
    next_thread = MAX_THREAD_NUMBER;
  }

  stop_timer();
}

void signal_handler(){
	printf(" -> Time expired, jumping to next thread!, Partial Result = %.5LF\n\n", current_thread.data.result);
	current_thread.state = WAITING;
  save_current_state();
  Scheduler();
}

void Scheduler() {

  int coming_back = sigsetjmp(list_of_threads[current_thread.id].env,1);

	if(coming_back == 1){return;}

  if (algorithm == 1) {
    get_next_thread_1();
		if (opt_mode==1) {
			timer_quantum(quantum, signal_handler);
		}
      } else {
    get_next_thread_2();
  }

  if (next_thread == MAX_THREAD_NUMBER) {
		stop_timer();
    program_finished(); //Finaliza el programa si current_id llego a ser el ultimo.
  } else {
		current_thread = list_of_threads[next_thread];
		list_of_threads[current_thread.id].state = RUNNING;
    printf("Running Thread (%d) ", current_thread.id);
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
	list_of_threads[thread_counter].data.actual_workload = 0;

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

  sigsetjmp(list_of_threads[thread_counter].env,1);

}

int menu( int argc, char **argv, char **in_file){
	int option_index = 0;
	int total_amount_tickets = 0;
  FILE *fp;
	while (( option_index = getopt(argc, argv, ":f:a:h")) != -1){
		switch (option_index) {
			case 'f':
        fp = fopen(optarg, "r");
        if(fp == NULL){
            printf("Error opening file\n");
            exit(1);
        }
        fscanf(fp, "Algorithm = %d\nNumber_Of_Threads = %d\nOperation_Mode = %d\nQuantum = %d\nWorkload_Fraction = %d", &algorithm, &number_of_threads, &opt_mode, &quantum, &work_limit);

				//Number of thread sanity checks
        if (4 >= number_of_threads || number_of_threads >= MAX_THREAD_NUMBER) {
          printf("Digite una cantidad de hilos entre 5 y 25\n");
          exit(1);
        }

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
						total_amount_tickets += thread_bills[i];
		    }

				//Bills tickets sanity checks
				if (TOTAL_TICKETS_AMOUNT <= total_amount_tickets) {
          printf("La maxima cantidad de tiquetes es %d, la cantidad de tiquites actual es: %d\n", TOTAL_TICKETS_AMOUNT, total_amount_tickets);
          exit(1);
        }

				for (int i = 0; i < number_of_threads; i++){
						fscanf(fp, "%d,", &thread_workloads[i] );
				}

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

	// Initialize GTK
	gtk_initialize(argc, argv);
	gtk_model_initialize();

	// Initialize the GUI
	gtk_view_initialize(&Scheduler);

  //Scheduler();


  printf("\nExecution finished, message from  main.\n");
  return 0;
}
