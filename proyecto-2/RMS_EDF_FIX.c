#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUMBER_OF_TASKS 6
#define MAX_HYPERPERIOD 1000

int number_of_tasks = 2;

struct output_matrix {
	int temp_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
	int rm_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
	int edf_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
	int llf_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
	int rm_val;
	int edf_val;
	int llf_val;
	int rm_error;
	int edf_error;
	int llf_error;
};

// struct tasks {
// 	int id;
// 	int period;
// 	int ctime;
// 	int state;
// 	int laxity;
// 	int next_deadline;
// 	int ctime_pending;
// };

int tasks_ctime[MAX_NUMBER_OF_TASKS] = {3, 12};
int tasks_period[MAX_NUMBER_OF_TASKS] = {3, 6};

int tasks_id_original[MAX_NUMBER_OF_TASKS] = {0, 1, 2, 3 ,4 , 5};
int tasks_ctime_pending[MAX_NUMBER_OF_TASKS];
int task_state[MAX_NUMBER_OF_TASKS];
int tasks_queue_id[MAX_NUMBER_OF_TASKS];
int tasks_laxity[MAX_NUMBER_OF_TASKS];
int tasks_next_deadline[MAX_NUMBER_OF_TASKS];
int tasks_run[MAX_NUMBER_OF_TASKS];

struct output_matrix results;
// struct tasks list_of_tasks[MAX_NUMBER_OF_TASKS];


int lcm = 12;

void reset_vectors() {
	memcpy(tasks_queue_id, tasks_id_original, sizeof(tasks_queue_id));
	memcpy(tasks_ctime_pending, tasks_ctime, sizeof(tasks_ctime_pending));
	memset(task_state, 0, sizeof(task_state));
	memset(tasks_run, 0, sizeof(tasks_run));
	memset(results.temp_results, 0, sizeof(results.temp_results));
}

void id_priority_sort_ascending(int reference_array[]){
	int temp_array, temp_id;
	int temp_sorted_array[number_of_tasks];

	memcpy(temp_sorted_array, reference_array, sizeof(temp_sorted_array));

	memcpy(tasks_queue_id, tasks_id_original, sizeof(tasks_id_original)); //Clone ID vector so that it can be sorted based on reference array priority.

	for (int i = 0; i < number_of_tasks; i++)
	{
		for (int j = i + 1; j < number_of_tasks; j++)
		{
			if(reference_array[i] > reference_array[j])
			{
				temp_array = reference_array[i];
				reference_array[i] = reference_array[j];
				reference_array[j] = temp_array;


				temp_id = tasks_queue_id[i];
				tasks_queue_id[i] = tasks_queue_id[j];
				tasks_queue_id[j] = temp_id;
			}
		}
	}

	memcpy(reference_array, temp_sorted_array, sizeof(temp_sorted_array));

}

int enable_task_based_on_period(int current_cycle) {

	int non_schedulable = 0;

	for (int i = 0; i < number_of_tasks; i++) {
		if (current_cycle%tasks_period[i] == 0) {
			task_state[i] = 1;  //enable task to run;

			// printf("se cumplio periodo de la tarea %d, estado => %d\n", i, task_state[i]);

			/////////////// Only for EDF //////////////////
			tasks_next_deadline[i] = current_cycle+tasks_period[i]; //now that task has been re-enabled calculate next deadline.
			///////////////////////////////////////////////

			if (tasks_ctime_pending[i] == 0) { //if task finished then it should be reseted here.
				tasks_ctime_pending[i] = tasks_ctime[i];
			}  else if ( (0 < tasks_ctime_pending[i]) && (tasks_ctime_pending[i] < tasks_ctime[i])) {
				printf("Deadline of task %d not met\n" ,tasks_queue_id[i]);
				non_schedulable = 1;
				break;
			}
		}
	}
	return non_schedulable;
}

int check_pending_tasks(){

	int non_schedulable = 0;

	for (int i = 0; i < number_of_tasks; i++) {
		if (tasks_run[i] == 0) {
			printf("Deadline of task %d not met\n", i);
		}
	}
	return non_schedulable;
}

int deadline_finish (int non_schedulable) {

	int continue_loop;

	if (non_schedulable == 1) {
		continue_loop = 0;
	} else {
		continue_loop = 1;
	}
	return continue_loop;
}

int get_next_task_edf (){

	int avaliable_tasks = 0;
	int current_task_idx = 0;

	for (size_t i = 0; i < number_of_tasks; i++) { //Checks if there are tasks to run or not.
		avaliable_tasks = task_state[tasks_queue_id[i]] + avaliable_tasks;
	}

	if (avaliable_tasks > 0) { //If there are tasks to run then

		memcpy(tasks_queue_id, tasks_id_original, sizeof(tasks_queue_id)); // reset vector
		id_priority_sort_ascending(tasks_next_deadline); //Sorts the array based on the RM algorithim and get first task.

		while ( task_state[tasks_queue_id[current_task_idx]] == 0) {
			current_task_idx=(current_task_idx+1)%number_of_tasks;
		}

	} else {
	 current_task_idx = -1; //Enables a flag that means no tasks to run.
 }

	 return current_task_idx;
}

int get_next_task_rm() {

	int current_task_idx = 0;
	int avaliable_tasks = 0;

	for (size_t i = 0; i < number_of_tasks; i++) { //Checks if there are tasks to run or not.
		avaliable_tasks = task_state[tasks_queue_id[i]] + avaliable_tasks;
	}

	if (avaliable_tasks > 0) { //If there are tasks to run then

		while (task_state[tasks_queue_id[current_task_idx]] == 0) { //Searches for next avaliable task that is runnable.
			current_task_idx=(current_task_idx+1)%number_of_tasks;
		}

	} else {
		current_task_idx = -1; //Enables a flag that means no tasks to run.
	}

return current_task_idx;
}

int get_next_task_llf (){

	int avaliable_tasks = 0;
	int current_task_idx = 0;

	for (size_t i = 0; i < number_of_tasks; i++) { //Checks if there are tasks to run or not.
		avaliable_tasks = task_state[tasks_queue_id[i]] + avaliable_tasks;
	}


	if (avaliable_tasks > 0) { //If there are tasks to run then

		memcpy(tasks_queue_id, tasks_id_original, sizeof(tasks_queue_id)); // reset vector
		id_priority_sort_ascending(tasks_laxity); //Sorts the array based on the EDF algorithim and get first task.

		while ( task_state[tasks_queue_id[current_task_idx]] == 0) {
			current_task_idx=(current_task_idx+1)%number_of_tasks;
		}

	} else {
	 current_task_idx = -1; //Enables a flag that means no tasks to run.
 }

	 return current_task_idx;
}

void build_matrix(int current_cycle, int id_task) {
	printf("Current Task (%d) \n", id_task+1);
	results.temp_results[current_cycle][id_task] = id_task+1;
}

void execute_task(int current_task_idx, int current_cycle) {
	if (current_task_idx==-1) { //para periodos que no tienen tareas que ejecutar.
		printf("NOP\n");
	} else {
		int current_task_id = tasks_queue_id[current_task_idx];
		int current_task_ctime = tasks_ctime[current_task_id];
		int current_task_period = tasks_period[current_task_id];
		int current_tasks_ctime_pending = tasks_ctime_pending[current_task_id];

		if ( current_tasks_ctime_pending > 0 && task_state[current_task_id] == 1) {
			build_matrix(current_cycle, current_task_id);
			current_tasks_ctime_pending--; //Reduces ctime by one as it just executed.
			tasks_ctime_pending[current_task_id] = current_tasks_ctime_pending;
		}

		if (current_tasks_ctime_pending == 0) {
			tasks_ctime_pending[current_task_id] = current_task_ctime; //resets computation for next period.
			task_state[current_task_id] = 0;  //disable task to run;
			tasks_run[current_task_id] = 1;
		}
	}
}

void calculate_laxity(int current_cycle) {
	for (int i = 0; i < number_of_tasks; i++) {
		tasks_laxity[i] = tasks_next_deadline[i]	- current_cycle - tasks_ctime_pending[i];
	}
}

int  main(int argc, char const *argv[]) {

	int current_task_idx ,continue_loop, non_schedulable;

	id_priority_sort_ascending(tasks_period); //Sorts the array based on the RM algorithim and get first task.

	// for (int i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
	// 	printf("task %d\n", tasks_queue_id[i]);
	// }

	int algorithim[3] = {1,1,1}; //To enable the for loops for the algorithms.

	for (int current_cycle = 0; current_cycle < lcm && algorithim[0] != 0; current_cycle++) {
		printf("Estoy en ciclo %d -> ", current_cycle);
		non_schedulable  = enable_task_based_on_period(current_cycle);
		continue_loop     = deadline_finish (non_schedulable); if (continue_loop == 0) break;
		current_task_idx  = get_next_task_rm() ;
		execute_task(current_task_idx, current_cycle);
	}

	// for (int i = 0; i < number_of_tasks; i++) {
	// 	printf("tasks_ctime %d - tasks_ctime_pending %d\n", tasks_ctime[i], tasks_ctime_pending[i]);
	// }

	non_schedulable = check_pending_tasks();
	if (continue_loop == 0) results.rm_error = 1;

	memcpy(results.rm_results, results.temp_results, sizeof(results.rm_results));

	printf("RM Finished\n\n");

	reset_vectors();

	for (int current_cycle = 0; current_cycle < lcm && algorithim[1] != 0; current_cycle++) {
		printf("Estoy en ciclo %d ->", current_cycle);
		non_schedulable  = enable_task_based_on_period(current_cycle);
		continue_loop     = deadline_finish (non_schedulable); if (continue_loop == 0) break;
		current_task_idx  = get_next_task_edf() ;
		execute_task(current_task_idx, current_cycle);
	}

	non_schedulable = check_pending_tasks();
	if (continue_loop == 0) results.edf_error = 1;



	memcpy(results.edf_results, results.temp_results, sizeof(results.rm_results));

	printf("EDF Finished\n\n");

	reset_vectors();

	for (int current_cycle = 0; current_cycle < lcm && algorithim[2] != 0; current_cycle++) {
		printf("Estoy en ciclo %d ->", current_cycle);
		non_schedulable  = enable_task_based_on_period(current_cycle);
		continue_loop     = deadline_finish (non_schedulable); if (continue_loop == 0) break;
		calculate_laxity(current_cycle);
		current_task_idx  = get_next_task_llf() ;
		execute_task(current_task_idx, current_cycle);
	}

	non_schedulable = check_pending_tasks();
	if (continue_loop == 0) results.edf_error = 1;

	memcpy(results.llf_results, results.temp_results, sizeof(results.rm_results));

	printf("LLF Finished");


/////////////////////////////////////Printing Vectors ////////////////////////////////////////////////
	printf("\nRM TIME TABLE\n");

	for (int j = 0; j < number_of_tasks; j++) {
		for (int i = 0; i < lcm; i++) {
			printf("%d | ", results.rm_results[i][j]);
		}
		printf("\n");
	}

	printf("\nEDF TIME TABLE\n");

	for (int j = 0; j < number_of_tasks; j++) {
		for (int i = 0; i < lcm; i++) {
			printf("%d | ", results.edf_results[i][j]);
		}
		printf("\n");
	}

	printf("\nLLF TIME TABLE\n");

	for (int j = 0; j < number_of_tasks; j++) {
		for (int i = 0; i < lcm; i++) {
			printf("%d | ", results.llf_results[i][j]);
		}
		printf("\n");
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;

}
