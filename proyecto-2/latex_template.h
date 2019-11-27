#include <unistd.h>

#define LINE_NUM 43
#define TABLE_START 12
#define TABLE2_START 22
#define TABLE3_START 32
#define ROWS 3
#define COLUMNS 18

int lcm = COLUMNS;
int number_of_tasks = ROWS;


#define MAX_NUMBER_OF_TASKS 6
#define MAX_HYPERPERIOD 1000

struct output_matrix {
	int temp_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
	int rm_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
	int edf_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
	int llf_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
	int rm_val;
	int edf_val;
	int llf_val;
};

struct output_matrix results;
// struct tasks list_of_tasks[MAX_NUMBER_OF_TASKS];

int tasks_ctime[MAX_NUMBER_OF_TASKS] = {1, 2, 3};
int tasks_period[MAX_NUMBER_OF_TASKS] = {6, 9, 18};

int all_in_one = 1;

char *tex[] = {"\\documentclass{beamer}",
"\\usepackage{amssymb}",
"\\usepackage{graphicx}",
"\\title{Basic presentation}",
"\\begin{document}",
"\\begin{frame}",
"\\titlepage",
"\\end{frame}",
"\\begin{frame}",
"\\section{INTRODUCTION}",
"INTRODUCTION",
"\\end{frame}",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"\\end{document}"};


int table_write(FILE* file, int matrix[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS], int rm, int edf, int llf){
  /*Counter variables for the loop*/
  int i, j;

	if (!all_in_one) {
		fprintf (file, "\\begin{frame}\n");
	}
	if (rm) {
		fprintf (file, "RATE MONOTONIC\n");
	}
	if (edf) {
		fprintf (file, "EARLIEST DEADLINE FIRST\n");
	}
	if (llf) {
		fprintf (file, "LEAST LAXITY FIRST\n");
	}

	fprintf (file, "\\begin{table}[]\n");
	fprintf (file, "\\resizebox{\\textwidth}{!}{\n");
  fprintf (file, "\\begin{tabular}{");
  for (i = 0; i < lcm+1; i++) {fprintf (file, "|l");}
  fprintf (file, "|} \n");
  fprintf (file, "\\hline \n");
  fprintf (file, "T");
  for (i = 0; i < lcm; i++) {fprintf (file, " &");}
  fprintf (file, " \\\\ \\hline \n");

  for(i=0; i < number_of_tasks; i++) {
    fprintf (file, "%d &", i+1);
     for(j=0;j < lcm;j++) {
       if(matrix[j][i]){
         fprintf (file, "$\\blacksquare$");
       }
      if(j != lcm - 1)
        fprintf (file, " &");
     }
     fprintf (file, " \\\\ ");
  }
  fprintf (file, "\n");
	fprintf (file, "\\hline\n");
	fprintf (file, "\\end{tabular}\n");
	fprintf (file, "}\n");
	fprintf (file, "\\end{table}\n");

	if (!all_in_one) {
		fprintf (file, "\\end{frame}\n");
	}
  return 0;
}
