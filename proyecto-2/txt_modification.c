#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_NUM 22
#define TABLE_START 15
#define ROWS 6
#define COLUMNS 18

int execution_slots[6][10];

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
"\\begin{table}[]",
"\\resizebox{\\textwidth}{!}{",
"c1f1 & c2f1 \\\\ c1f2 & c2f2",
"\\hline",
"\\end{tabular}",
"}",
"\\end{table}",
"\\end{frame}",
"\\end{document}"};

int llenar_matricita(){
   /*Counter variables for the loop*/
   int i, j;
   for(i=0; i < ROWS; i++) {
      for(j=0;j < COLUMNS;j++) {
         execution_slots[i][j] = 1;
      }
   }
   // Laying array elements
   printf("Two Dimensional array elements:\n");
   for(i=0; i < ROWS; i++) {
      for(j=0;j < COLUMNS;j++) {
         printf("%d ", execution_slots[i][j]);
         if(j == COLUMNS){
            printf("\n");
         }
      }
   }
   return 0;
}

int preparar_codiguito(FILE* file){
  /*Counter variables for the loop*/
  int i, j;
  fprintf (file, "\\begin{tabular}{");
  for (i = 0; i < COLUMNS+1; i++) {fprintf (file, "|l");}
  fprintf (file, "|} \n");
  fprintf (file, "\\hline \n");
  fprintf (file, "T");
  for (i = 0; i < COLUMNS; i++) {fprintf (file, " &");}
  fprintf (file, " \\\\ \\hline \n");

  for(i=0; i < ROWS; i++) {
    fprintf (file, "%d &", i+1);
     for(j=0;j < COLUMNS;j++) {
       if(execution_slots[i][j]){
         fprintf (file, "$\\blacksquare$");
       }
      if(j != COLUMNS - 1)
        fprintf (file, " &");
     }
     fprintf (file, " \\\\ ");
  }
  fprintf (file, "\n");
  return 0;
}

int main()
{
  llenar_matricita();
    FILE* file = fopen("tmp.tex", "w");
    for(int j = 0; j < LINE_NUM;j++){
      printf("i %d\n", j);
      if (j == TABLE_START) {
        preparar_codiguito(file);
      } else {
        fprintf (file, "%s", tex[j]);
        fprintf (file, "\n");
      }
    }
    fclose(file);
    system("pdflatex tmp.tex");
    system("atom tmp.pdf");
    return 0;
}
