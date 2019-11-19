#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *tex[] = {"\\documentclass{beamer}",
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
"\\begin{tabular}{ll}",
"c1f1 & c2f1 \\\\ c1f2 & c2f2",
"\\end{tabular}",
"\\end{table}",
"\\end{frame}",
"\\end{document}"};

int main()
{
    FILE* file = fopen("tmp.tex", "w");
    for(int i = 0; i < 18;i++){
      fprintf (file, "%s", tex[i]);
      fprintf (file, "\n");
    }

    fclose(file);
    system("pdflatex tmp.tex");
    system("atom tmp.pdf");

    return 0;
}
