#ifndef GRAPH_H
#define GRAPH_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define MIN(A,B) ((A < B) ? A : B)
#define MAX(A,B) ((A > B) ? A : B)

typedef struct edges {
    int *neighbors;
    int qtd;
} Edges;

extern char separator[15];

void print_graph_structure(char **nodes, Edges *edges, int qt_nodes);
void read_graph(int qt_nodes, int qt_edges, Edges *edges, char **nodes);

int get_node(char **nodes, char *n);
int get_ev_index(int node, int edge);

#endif
