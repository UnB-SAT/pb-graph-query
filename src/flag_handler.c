#include "flag_handler.h"

extern int debug, benchmark, verbose, N_THREADS;
extern char *file_path, *file_name;

void handle_flags(int argc, char **argv) {
    int opt;

    while ((opt = getopt(argc, argv, "vt:df:b")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case 'b':
                benchmark = 1;
                break;
            case 't':
                N_THREADS = atoi(optarg);
                break;
            case 'd':
                debug = 1;
                break;
            case 'f':
                file_path = optarg;
                printf("file_path: %s\n", file_path);

                handle_file(file_path);
                printf("file_name: %s\n", file_name);
                break;
        }
    }
}

void handle_file(char * file_path) {
    char *delimiter = "/";

    char* token = strtok(file_path, delimiter);
    char* _file_name;

    while(token != NULL) {
        _file_name = token;

        token = strtok(NULL, delimiter);
    }

    file_name = _file_name;
}
