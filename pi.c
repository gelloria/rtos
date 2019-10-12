#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
// #include <iostream>
// #include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>


int menu( int argc, char **argv, char **in_file){
	int option_index = 0;
  FILE *fp;
  char algorithm[50];
	int thread_bills[10];
	int thread_workload[10];
  int number_of_threads;
	while (( option_index = getopt(argc, argv, ":f:a:h")) != -1){
		switch (option_index) {
			case 'f':
        fp = fopen(optarg, "r");
        if(fp == NULL){
            printf("Error opening file\n");
            exit(1);
        }
        fscanf(fp, "Algorithm = %s\nNumber_Of_Threads = %d", algorithm, &number_of_threads);
        printf("Algorithm is %s\nNumber Of Threads is %d\n", algorithm, number_of_threads);

				for (int i = 0; i < 10; i++){
		        fscanf(fp, "%d,", &thread_bills[i] );
		    }
				for (int i = 0; i < 10; i++){
						printf("thread_bills is: %d\n", thread_bills[i]);
				}

				for (int i = 0; i < 10; i++){
						fscanf(fp, "%d,", &thread_workload[i] );
				}
				for (int i = 0; i < 10; i++){
						printf("thread_workload is: %d\n", thread_workload[i]);
				}
        fclose(fp);

				break;
			case 'a':
				printf("Authors of the program:\n\tLeonardo Araya\n\tGabriel Loria\n\tRolando Moraga\n");
				break;
			case 'h':
				printf("Use '-f configuration_file' to set the configuration file.\n");
				return 1;
				break;
			default:
				printf("Not recognized option. Please use '-h' for help.\n");
				return 1;
		}
	}
	return 0;
}

// Main code starts here
int main(int argc, char **argv){
	char *config_file_name;
	int status = 0;

	status = menu(argc,argv, &config_file_name);
	if (status) {
		// fprintf(stderr, "status :%d\n", status);
		return status;
	}



  printf("Hello, World!");

  return 0;
}
