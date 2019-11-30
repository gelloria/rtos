#ifndef CLASS_H
#define CLASS_H

#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define LINE_NUM 78
#define TESTS_START 28
#define TABLE_START 45
#define TABLE2_START 55
#define TABLE3_START 65
#define ROWS 6
#define COLUMNS 18
#define MAX_NUMBER_OF_TASKS 6
#define MAX_HYPERPERIOD 1000

int execute_scheduler();
float calculate_u();
float calculate_U(int n);

int all_in_one;
int use_rm;
int use_edf;
int use_llf;
int use_multiple_page;
int number_of_tasks;

int entry_0_exec_time_int;
int entry_1_exec_time_int;
int entry_2_exec_time_int;
int entry_3_exec_time_int;
int entry_4_exec_time_int;
int entry_5_exec_time_int;

int entry_0_text_period_int;
int entry_1_text_period_int;
int entry_2_text_period_int;
int entry_3_text_period_int;
int entry_4_text_period_int;
int entry_5_text_period_int;

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
