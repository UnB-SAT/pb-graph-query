#ifndef RESTRICTIONS_H
#define RESTRICTIONS_H

#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

#include"graph.h"

#define TRUE 1
#define FALSE 0
#define R_C0 0
#define R_INITIAL 1
#define R_FINAL 2
#define R_PATH 3
#define R_TYPE 4
#define RESTRICT_INITIAL 5
#define RESTRICT_FINAL 6

extern int N_THREADS;

typedef struct var {
    int value;
    int offset;
    int is_positive;
} Var;

#define INIT_VAR() (Var){.value = 0, .offset = 1, .is_positive = TRUE}
#define INIT_VARS(V, O, I) (Var){.value = V, .offset = O, .is_positive = I}

typedef struct restriction {
    int qt;
    Var *vars;
    int comparison;
} Restriction;

typedef struct graph_restriction {
    Restriction** restrictions;
    int qt;
} Graph_Restriction;

typedef struct graph_head {
    Graph_Restriction * gr;
    int qt;
}GraphHead;

typedef struct question_restrictions {
    Graph_Restriction *graph_restrictions;
    Restriction *question_restrictions;
    int question;
    int qt;
}QRestrictions;

typedef enum NODE_TYPES {
    C0 = 1,
    INITIAL,
    FINAL,
    PATH,
    TOTAL_TYPES
} NODE_TYPES;

typedef struct thread_args {
    int index;
    GraphHead *head;
    Edges* edges;
} thread_args;

void write_file(GraphHead* r);
Restriction *get_restrictions(int qt, Var *vars, int comparison);
void generate_restrictions(GraphHead*graph_head, Edges *edges);
void *generate_restriction(void* args);

/*
 * Graph based restrictions
*/
Restriction *c_0(int node_idx, Edges *edges);
Restriction *c_0_n(int node_idx, Edges *edges);
Restriction *def_initial(int node_idx, Edges *edges);
Restriction *def_initial_n(int node_idx, Edges *edges);
Restriction *def_final(int node_idx, Edges *edges);
Restriction *def_final_n(int node_idx, Edges *edges);
Restriction *def_path(int node_idx, Edges *edges);
Restriction *def_path_n(int node_idx, Edges *edges);
Restriction *def_node_type(int node_idx);
Restriction *def_node_type_n(int node_idx);
Restriction *restrict_initial();
Restriction *restrict_final();
Restriction *restrict_initial_n();
Restriction *restrict_final_n();

/*
 * Utils functions
 */
Restriction *formuleitor(int node_idx, Edges *edges, NODE_TYPES type, int v0_ip, int v0_o, int v_ip, int is_negate);
int get_idx(int node, int edge);
int map_nodes_to_variable(int idx, NODE_TYPES type);
int map_edges_to_variable(int idx);

void free_restriction(Graph_Restriction *gr);

#endif
