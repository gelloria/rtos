#ifndef CLASS_H
#define CLASS_H

#include <unistd.h>

#define LINE_NUM 62
#define TABLE_START 30
#define TABLE2_START 40
#define TABLE3_START 50
#define ROWS 6
#define COLUMNS 18
#define MAX_NUMBER_OF_TASKS 6
#define MAX_HYPERPERIOD 1000

int execute_scheduler();
int all_in_one;
int use_rm;
int use_edf;
int use_llf;
int use_multiple_page;
int number_of_tasks;

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

struct output_matrix results;

int tasks_ctime[MAX_NUMBER_OF_TASKS];
int tasks_period[MAX_NUMBER_OF_TASKS];

#endif
