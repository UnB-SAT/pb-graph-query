#ifndef FLAG_HANDLER
#define FLAG_HANDLER

#include<getopt.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void handle_flags(int argc, char**argv);
void handle_file(char*file_path);

#endif
