#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>

// #define NUMBER_OF_TASKS 4 //ejemplo rolando que falla
//
// int tasks_id_sorted[NUMBER_OF_TASKS] = {0, 1, 2, 3};
// int tasks_id_original[NUMBER_OF_TASKS] = {0, 1, 2, 3};
//
// int tasks_period[NUMBER_OF_TASKS] = {10, 5, 20, 10};
//
// int tasks_ctime[NUMBER_OF_TASKS] = {4, 2, 3, 1};
// int tasks_ctime_pending[NUMBER_OF_TASKS] = {4, 2, 3, 1};
//
//
// int task_ready[NUMBER_OF_TASKS] = {0, 0, 0, 0};

// #define NUMBER_OF_TASKS 3 //ejemplo de rm de clase
//
// int tasks_id_sorted[NUMBER_OF_TASKS] = {0, 1, 2};
// int tasks_id_original[NUMBER_OF_TASKS] = {0, 1, 2};
//
// int tasks_period[NUMBER_OF_TASKS] = {6,9, 18};
//
// int tasks_ctime[NUMBER_OF_TASKS] = {1, 2, 6};
// int tasks_ctime_pending[NUMBER_OF_TASKS] = {1, 2, 6};
//
// int task_ready[NUMBER_OF_TASKS] = {0, 0, 0};

// #define NUMBER_OF_TASKS 3
//
// int tasks_id_sorted[NUMBER_OF_TASKS] = {0, 1, 2};
// int tasks_id_original[NUMBER_OF_TASKS] = {0, 1, 2};
//
// int tasks_period[NUMBER_OF_TASKS] = {30, 40, 52};
//
// int tasks_ctime[NUMBER_OF_TASKS] = {10, 10, 12};
// int tasks_ctime_pending[NUMBER_OF_TASKS] = {10, 10, 12};
//
// int task_ready[NUMBER_OF_TASKS] = {0,0,0};
//
// int tasks_next_deadline[NUMBER_OF_TASKS] = {0,0,0};

#define NUMBER_OF_TASKS 2

int tasks_id_sorted[NUMBER_OF_TASKS] = {0, 1};
int tasks_id_original[NUMBER_OF_TASKS] = {0, 1};

int tasks_period[NUMBER_OF_TASKS] = {6, 9};

int tasks_ctime[NUMBER_OF_TASKS] = {3, 4};
int tasks_ctime_pending[NUMBER_OF_TASKS] = {3, 4};

int task_ready[NUMBER_OF_TASKS] = {0,0};

int tasks_next_deadline[NUMBER_OF_TASKS] = {0,0};

int lcm = 18;

void reset_vectors() {
	memcpy(tasks_id_sorted, tasks_id_original, sizeof(tasks_id_sorted));
	memcpy(tasks_ctime_pending, tasks_ctime, sizeof(tasks_ctime_pending));
	memset(task_ready, 0, sizeof(task_ready));
}

void id_priority_sort_ascending(int reference_array[], int Size){
	int temp_array, temp_id;
	int temp_sorted_array[Size];

	memcpy(temp_sorted_array, reference_array, sizeof(temp_sorted_array));

	for (int i = 0; i < Size; i++)
	{
		for (int j = i + 1; j < Size; j++)
		{
			if(reference_array[i] > reference_array[j])
			{
				temp_array = reference_array[i];
				reference_array[i] = reference_array[j];
				reference_array[j] = temp_array;

				temp_id = tasks_id_sorted[i];
				tasks_id_sorted[i] = tasks_id_sorted[j];
				tasks_id_sorted[j] = temp_id;
			}
		}
	}

	memcpy(reference_array, temp_sorted_array, sizeof(temp_sorted_array));

}

int enable_task_based_on_period(int current_cycle) {

	int non_schedulable = 0;

	for (int i = 0; i < NUMBER_OF_TASKS; i++) {
		if (current_cycle%tasks_period[i] == 0) {
			task_ready[i] = 1;  //enable task to run;

			// printf("se cumplio periodo de la tarea %d, estado => %d\n", i, task_ready[i]);

			/////////////// Only for EDF //////////////////
			tasks_next_deadline[i] = current_cycle+tasks_period[i]; //now that task has been re-enabled calculate next deadline.
			///////////////////////////////////////////////

			if (tasks_ctime_pending[i] == 0) { //if task finished then it should be reseted here.
				tasks_ctime_pending[i] = tasks_ctime[i];
			}  else if ( (0 < tasks_ctime_pending[i]) && (tasks_ctime_pending[i] < tasks_ctime[i])) {
				printf("Deadline of task %d not met\n" ,tasks_id_sorted[i]);
				non_schedulable = 1;
				break;
			}
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

	for (size_t i = 0; i < NUMBER_OF_TASKS; i++) { //Checks if there are tasks to run or not.
		avaliable_tasks = task_ready[tasks_id_sorted[i]] + avaliable_tasks;
	}


	if (avaliable_tasks > 0) { //If there are tasks to run then

		memcpy(tasks_id_sorted, tasks_id_original, sizeof(tasks_id_sorted)); // reset vector
		id_priority_sort_ascending(tasks_next_deadline, NUMBER_OF_TASKS); //Sorts the array based on the RM algorithim and get first task.

		while ( task_ready[tasks_id_sorted[current_task_idx]] == 0) {
			current_task_idx=(current_task_idx+1)%NUMBER_OF_TASKS;
		}

		// for (int i = 0; i < NUMBER_OF_TASKS; i++) {
		// 	printf("task %d, tasks_next_deadline %d, ready %d \n", i, tasks_next_deadline[i], task_ready[i]);
	 // }
	 //
		// for (int i = 0; i < NUMBER_OF_TASKS; i++) {
		// 	printf("task order %d\n", tasks_id_sorted[i]);
	 // }


	} else {
	 current_task_idx = -1; //Enables a flag that means no tasks to run.
 }

	 return current_task_idx;
}

int get_next_task_rm() {

	int current_task_idx = 0;
	int avaliable_tasks = 0;

	for (size_t i = 0; i < NUMBER_OF_TASKS; i++) { //Checks if there are tasks to run or not.
		avaliable_tasks = task_ready[tasks_id_sorted[i]] + avaliable_tasks;
	}

	if (avaliable_tasks > 0) { //If there are tasks to run then

		while (task_ready[tasks_id_sorted[current_task_idx]] == 0) { //Searches for next avaliable task that is runnable.
			current_task_idx=(current_task_idx+1)%NUMBER_OF_TASKS;
		}

	} else {
		current_task_idx = -1; //Enables a flag that means no tasks to run.
	}

return current_task_idx;
}

void execute_task(int current_task_idx) {
	if (current_task_idx==-1) { //para periodos que no tienen tareas que ejecutar.
		printf("NOP\n");
	} else {
		int current_task_id = tasks_id_sorted[current_task_idx];
		int current_task_ctime = tasks_ctime[current_task_id];
		int current_task_period = tasks_period[current_task_id];
		int current_tasks_ctime_pending = tasks_ctime_pending[current_task_id];

		if ( current_tasks_ctime_pending > 0 && task_ready[current_task_id] == 1) {
			printf("Current Task (%d) \n", current_task_id);
			current_tasks_ctime_pending--; //Reduces ctime by one as it just executed.
			tasks_ctime_pending[current_task_id] = current_tasks_ctime_pending;
		}

		if (current_tasks_ctime_pending == 0) {
			tasks_ctime_pending[current_task_id] = current_task_ctime; //resets computation for next period.
			task_ready[current_task_id] = 0;  //disable task to run;
		}
	}
}

int  main(int argc, char const *argv[]) {

	int current_task_idx ,continue_loop, non_schedulable;

	id_priority_sort_ascending(tasks_ctime, NUMBER_OF_TASKS); //Sorts the array based on the RM algorithim and get first task.

	int algorithim[3] = {1,1,1};

	for (int current_cycle = 0; current_cycle < lcm && algorithim[0] != 0; current_cycle++) {
		printf("Estoy en ciclo %d -> ", current_cycle);
		non_schedulable  = enable_task_based_on_period(current_cycle);
		continue_loop     = deadline_finish (non_schedulable); if (continue_loop == 0) break;
		current_task_idx  = get_next_task_rm() ;
		execute_task(current_task_idx);
	}

	printf("RM Finished!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n");

	reset_vectors();

	id_priority_sort_ascending(tasks_period, NUMBER_OF_TASKS); //Sorts the array based on the EDF algorithim and get first task.

	for (int current_cycle = 0; current_cycle < lcm && algorithim[1] != 0; current_cycle++) {
		printf("Estoy en ciclo %d ->", current_cycle);
		non_schedulable  = enable_task_based_on_period(current_cycle);
		continue_loop     = deadline_finish (non_schedulable); if (continue_loop == 0) break;
		current_task_idx  = get_next_task_edf() ;
		execute_task(current_task_idx);
	}

	printf("EDF Finished!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

	return 0;
}
