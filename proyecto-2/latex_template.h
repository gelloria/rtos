#include <unistd.h>

#define LINE_NUM 43
#define TABLE_START 16
#define TABLE2_START 26
#define TABLE3_START 36
#define ROWS 3
#define COLUMNS 10

int lcm = 10;
int number_of_tasks = ROWS;

#define MAX_NUMBER_OF_TASKS 6
#define MAX_HYPERPERIOD 1000

struct output_matrix {
	int temp_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
	int rm_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
	int edf_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
	int llf_results[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS];
};

struct output_matrix results;
// struct tasks list_of_tasks[MAX_NUMBER_OF_TASKS];

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
"\\begin{frame}",
"EARLIEST DEADLINE FIRST",
"\\begin{table}[]",
"\\resizebox{\\textwidth}{!}{",
"c1f1 & c2f1 \\\\ c1f2 & c2f2",
"\\hline",
"\\end{tabular}",
"}",
"\\end{table}",
"\\end{frame}",
"\\begin{frame}",
"RATE MONOTONIC",
"\\begin{table}[]",
"\\resizebox{\\textwidth}{!}{",
"c1f1 & c2f1 \\\\ c1f2 & c2f2",
"\\hline",
"\\end{tabular}",
"}",
"\\end{table}",
"\\end{frame}",
"\\begin{frame}",
"LEAST LAXITY FIRST",
"\\begin{table}[]",
"\\resizebox{\\textwidth}{!}{",
"c1f1 & c2f1 \\\\ c1f2 & c2f2",
"\\hline",
"\\end{tabular}",
"}",
"\\end{table}",
"\\end{frame}",
"\\end{document}"};

// int llenar_matricita(){
//    /*Counter variables for the loop*/
//    int i, j;
//    for(i=0; i < ROWS; i++) {
//       for(j=0;j < COLUMNS;j++) {
//          execution_slots[i][j] = 1;
//       }
//    }
//    // Laying array elements
//    printf("Two Dimensional array elements:\n");
//    for(i=0; i < ROWS; i++) {
//       for(j=0;j < COLUMNS;j++) {
//          printf("%d ", execution_slots[i][j]);
//          if(j == COLUMNS){
//             printf("\n");
//          }
//       }
//    }
//    return 0;
// }

int table_write(FILE* file, int matrix[MAX_HYPERPERIOD][MAX_NUMBER_OF_TASKS]){
  /*Counter variables for the loop*/
  int i, j;
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
  return 0;
}
