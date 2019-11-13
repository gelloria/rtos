#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>

#define NUMBER_OF_TASKS 3

int tasks_ctime[NUMBER_OF_TASKS] = {4, 2, 3};
int tasks_period[NUMBER_OF_TASKS] = {10, 5, 10};
int task_id[NUMBER_OF_TASKS] = {1,2,3};
int periods_per_task[NUMBER_OF_TASKS];
int lcm = 10;

void get_rate_monotonic_sort(int array_of_ctime[], int Size){
	int temp_c_sort, temp_id;
  int temp_ctime[NUMBER_OF_TASKS];

  memcpy(temp_ctime, array_of_ctime, sizeof(temp_ctime));

	for (int i = 0; i < Size; i++)
	{
		for (int j = i + 1; j < Size; j++)
		{
			if(array_of_ctime[i] > array_of_ctime[j])
			{
				temp_c_sort = array_of_ctime[i];
				array_of_ctime[i] = array_of_ctime[j];
				array_of_ctime[j] = temp_c_sort;

        temp_id = task_id[i];
        task_id[i] = task_id[j];
        task_id[j] = temp_id;
			}
		}
	}
  memcpy(array_of_ctime, temp_ctime, sizeof(temp_ctime));
}

void get_number_periods() {
  for (int i = 0; i < NUMBER_OF_TASKS; i++) {
    periods_per_task[i] = lcm/tasks_period[i];
  }
}

int  main(int argc, char const *argv[]) {

  int k = 0;

  get_rate_monotonic_sort(tasks_ctime, NUMBER_OF_TASKS);
  get_number_periods();

  for (int i = 0; i < NUMBER_OF_TASKS; i++){
    printf("Compute time array is[%d] = %d \n", i, tasks_ctime[i]);
  };

  for (int i = 0; i < NUMBER_OF_TASKS; i++){
    printf("Orden de tareas[%d] = %d \n", i, task_id[i]);
  };

  for (int i = 0; i < NUMBER_OF_TASKS; i++){
    printf("periodos[%d] = %d \n", i, periods_per_task[i]);
  };

  printf("\n");

 for (int i = 1; i < lcm+1; i++) {
    printf("Ciclo %d\n", i);

    int current_task = task_id[k]-1;
    int ctime_current_task = tasks_ctime[current_task];
    printf("next task is %d with idx %d\n", task_id[k], current_task);

  while (ctime_current_task > 0) {
    if (periods_per_task[current_task] > 0) {
      printf("Current Task (%d), period %d, ctime %d\n", task_id[k], tasks_period[current_task], tasks_ctime[current_task]);
    }
    ctime_current_task--;
  }
  periods_per_task[current_task]--;
  k++;

  printf("\n");
}


  return 0;
}
