#ifndef FLAG_HANDLER
#define FLAG_HANDLER

#include<getopt.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

static struct option long_options[] = {
	{ "help", no_argument, 0, 'h' },
	{ "verbose", no_argument, 0, 'v' },
	{ "file", no_argument, 0, 'f' },
	{ "threads", no_argument, 0, 'f' },
	{0, 0, 0, 0}
};

void handle_flags(int argc, char**argv);
void handle_file(char*file_path);
void print_help(char*program_name);

#endif
