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
  char name[50];
  char last[50];
  int id = 0;
	while (( option_index = getopt(argc, argv, "i:o:ha")) != -1){
		switch (option_index) {
			case 'f':
        fp = fopen(optarg, "r");
        if(fp == NULL){
            printf("Error opening file\n");
            exit(1);
        }
        while( fscanf(fp, "Name = %s\nLast = %s\nId = %d\n", name, last, &id) != EOF ){
            printf("Name is %s\nLast is %s\nId is %d\n", name, last, id);
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
