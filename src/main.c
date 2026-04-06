#include "restrictions.h"
#include "flag_handler.h"
#include "timer.h"
#include "questions.h"
#include "solver.h"

// Global variables
int qt_nodes, qt_edges;
int *ev;

// Flags
int debug, verbose, benchmark, N_THREADS = 1;
char* file_name, *file_path;

int main(int argc, char **argv) {
    handle_flags(argc, argv);

    scanf("%d %d", &qt_nodes, &qt_edges);
    getchar();

    int ev_len = qt_nodes * (qt_nodes - 1) / 2;

    ev = (int*)malloc(ev_len* sizeof(int));
    char **nodes = (char**)malloc(qt_nodes * sizeof(char*));
    Edges *edges = (Edges*)malloc(qt_nodes * sizeof(Edges));

    for (int i = 0; i < qt_nodes; i++) {
        nodes[i] = (char*)malloc(4 * sizeof(char));
    }

    for (int i = 0; i < ev_len; i++) {
        ev[i] = -1;
    }

    // Read input
    HANDLE_TIMER("RG", read_graph(qt_nodes, qt_edges, edges, nodes));

    if (debug || verbose) {
        print_graph_structure(nodes, edges, qt_nodes);
    }

    GraphHead *graph_head = (GraphHead*)malloc(1 * sizeof(GraphHead));
    graph_head->qt = 7;
    graph_head->gr = (Graph_Restriction*)malloc(7*sizeof(Graph_Restriction));

    // Restriction generation
    HANDLE_TIMER("GR", generate_restrictions(graph_head, edges));


    // Read Graph Questions
    HANDLE_TIMER("QR", handle_questions(nodes, graph_head));

    return 0;
}
