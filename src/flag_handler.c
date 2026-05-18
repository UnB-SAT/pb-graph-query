#include "flag_handler.h"

extern int debug, benchmark, verbose, N_THREADS;
extern char *file_path, *file_name;

void handle_flags(int argc, char **argv) {
    int opt;

    while ((opt = getopt_long(argc, argv, "vt:df:bh", long_options, NULL)) != -1) {
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
            case 'h':
                print_help(argv[0]);
                exit(0);
            default:
                print_help(argv[0]);
                exit(1);
        }
    }
}

void print_help(char*prog_name) {
    printf("Usage: %s [OPTIONS]\n\n", prog_name);

    printf("Options:\n");
    printf("  -h, --help            Show this help message and exit\n");
    printf("  -v, --verbose         Enable verbose output\n");
    printf("  -f, --file FILE       Input file\n");
    printf("  -t, --threads INT     Number of threads to be used\n");
    printf("  -b,                   Enables benchmarking\n");
    printf("\n");

    printf("Example:\n");
    printf("  %s -b -t 10\n", prog_name);
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
