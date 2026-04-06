#include "solver.h"

void run_solver(char *file_name) {
	FILE *fp;
	char command[100];
	char buffer[1024];

	printf("Running solver...\n");

	sprintf(command, "clasp 0 -t %d --quiet=2 %s", N_THREADS, file_name);
	
	fp = popen(command, "r");

	if (fp == NULL) {
		printf("Fail to run solver.\n");
		exit(1);
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
	}

	pclose(fp);
}
