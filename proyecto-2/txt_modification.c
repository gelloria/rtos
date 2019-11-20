#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_NUM 18
#define TABLE_START 15
#define ROWS 6
#define COLUMNS 3

int execution_slots[6][10];

char *tex[] = {"\\documentclass{beamer}",
"\\usepackage{amssymb}",
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
"\\begin{tabular}{|l|l|}",
"\\hline",
"c1f1 & c2f1 \\\\ c1f2 & c2f2",
"\\hline",
"\\end{tabular}",
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
         if(j == COLUMNS - 1){
            printf("\n");
         }
      }
   }

   return 0;
}

int preparar_codiguito(FILE* file){
  /*Counter variables for the loop*/
  int i, j;

  fprintf (file, "Task & Executed \\\\ \\hline \\\\");
  fprintf (file, "\n");

  for(i=0; i < ROWS; i++) {
     for(j=0;j < COLUMNS;j++) {

       // It means we have a hit
        if(execution_slots[i][j]){
          fprintf (file, "$\\blacksquare$");
        }

        if(j != COLUMNS - 1)
          fprintf (file, " &");

     }

     fprintf (file, " \\\\ ");
  }

  return 0;
}

int main()
{

  llenar_matricita();

    FILE* file = fopen("tmp.tex", "w");
    for(int i = 0; i < LINE_NUM;i++){
      printf("i %d\n", i);
      if (i == TABLE_START) {
        preparar_codiguito(file);
        i = i - ROWS;
      } else {
        fprintf (file, "%s", tex[i]);
        fprintf (file, "\n");
      }
    }

    fclose(file);
    system("pdflatex tmp.tex");
    system("atom tmp.pdf");

    return 0;
}
