#include"restrictions.h"

extern int qt_nodes, qt_edges, benchmark;
extern int *ev;
extern char* file_name;

char *restriction_name[20] = {
    "C0",
    "Initial",
    "Final",
    "Path",
    "Type",
    "Restrict Initial",
    "Restrict Final"
};

void write_file(GraphHead *head) {
    char* _file_name = malloc(50*sizeof(char));
    char * _null_case = malloc(10* sizeof(char));

    if (file_name == NULL) {
        sprintf(_null_case, "%d-%d", qt_nodes, qt_edges);
    }
    
    sprintf(_file_name, "../restricoes/SAT-%s.pbs", file_name == NULL ? _null_case : file_name);
    printf("file_name: %s\n", _file_name);

    FILE *fp = fopen(_file_name, "w");

    if(fp == NULL) {
        printf("Cannot open file\n");
        exit(1);
    }

    fprintf(fp, "* #variable=%d #constraint=%d\n", qt_nodes * 4 + qt_edges, 10 * qt_nodes + 4);

    for (int c = 0; c < head->qt; c++){
        fprintf(fp, "* %s\n", restriction_name[c]);

        for(int i = 0; i < head->gr[c].qt; i++) {            
            Restriction * current_restriction = head->gr[c].restrictions[i];

            for(int j = 0; j < current_restriction->qt; j++){
                fprintf(fp, "+%d %sx%d ", \
                    current_restriction->vars[j].offset,\
                    current_restriction->vars[j].is_positive ? "" : "~",\
                    current_restriction->vars[j].value);
            }

            fprintf(fp, ">= %d;\n", current_restriction->comparison);
        }
    }

    printf("File written\n");

    fclose(fp);
}

void *generate_restriction(void * args) {
    thread_args *t_args = (thread_args*) args;

    GraphHead *head = t_args->head;
    int index = t_args->index;
    Edges *edges = t_args->edges;

    int positive_index = 2 * index;
    int negative_index = 2 * index + 1;

    // C_0 ( N( V_a ) )
    head->gr[R_C0].restrictions[positive_index] = c_0(index, edges);
    head->gr[R_C0].restrictions[negative_index] = c_0_n(index, edges);

    // Define initial
    head->gr[R_INITIAL].restrictions[positive_index] = def_initial(index, edges);
    head->gr[R_INITIAL].restrictions[negative_index] = def_initial_n(index, edges);

    // Define final
    head->gr[R_FINAL].restrictions[positive_index] = def_final(index, edges);
    head->gr[R_FINAL].restrictions[negative_index] = def_final_n(index, edges);

    // Define path
    head->gr[R_PATH].restrictions[positive_index] = def_path(index, edges);
    head->gr[R_PATH].restrictions[negative_index] = def_path_n(index, edges);

    // Define node type
    head->gr[R_TYPE].restrictions[positive_index] = def_node_type(index);
    head->gr[R_TYPE].restrictions[negative_index] = def_node_type_n(index);

    return NULL;
}

void generate_restrictions(GraphHead *graph_head, Edges *edges) {
    graph_head->gr->qt = TOTAL_TYPES + 2;

    for(int i = 0; i < TOTAL_TYPES; i++) {
        graph_head->gr[i].qt = 2*qt_nodes;
        graph_head->gr[i].restrictions = (Restriction**)malloc(2*qt_nodes*sizeof(Restriction*));
    }

    pthread_t threads[N_THREADS];
    int restrictions_done = 0;

    while (restrictions_done < qt_nodes){
        int restante = qt_nodes - restrictions_done;

        int n = MIN(restante, N_THREADS);

        thread_args t_args[n];

        for(int i = 0; i < n; i++) {            
            t_args[i].head = graph_head;
            t_args[i].index = restrictions_done + i;
            t_args[i].edges = edges;

            pthread_create(&threads[i], NULL, generate_restriction, &t_args[i]);
        }

        for(int i = 0; i < n; i++) {
            pthread_join(threads[i], NULL);
        }

        restrictions_done += n;
    }

    graph_head->gr[RESTRICT_INITIAL].qt = 2;
    graph_head->gr[RESTRICT_INITIAL].restrictions = (Restriction**)malloc(2*sizeof(Restriction*));
    graph_head->gr[RESTRICT_INITIAL].restrictions[0] = restrict_initial();
    graph_head->gr[RESTRICT_INITIAL].restrictions[1] = restrict_initial_n();

    graph_head->gr[RESTRICT_FINAL].qt = 2;
    graph_head->gr[RESTRICT_FINAL].restrictions = (Restriction**)malloc(2*sizeof(Restriction*));
    graph_head->gr[RESTRICT_FINAL].restrictions[0] = restrict_final();
    graph_head->gr[RESTRICT_FINAL].restrictions[1] = restrict_final_n();

    if(!benchmark){
        write_file(graph_head);
    }
}

Restriction *get_restrictions(int qt, Var *vars, int comparison) {
    Restriction *restriction = (Restriction*) malloc(sizeof(Restriction));

    restriction->qt = qt;
    restriction->vars = (Var*) malloc(qt * sizeof(Var));

    for(int i = 0; i < qt; i++) {
        restriction->vars[i] = vars[i];
    }

    restriction->comparison = comparison;

    return restriction;
}

/*
 *   V_0 | V
 * C0_v + sum_{i=0}^{E|v|} x_{v_i} >= 1;
 */
Restriction *c_0(int node_idx, Edges *edges) {
    NODE_TYPES type = C0;
    int v0_ip = TRUE, v0_o = 1;
    int v_ip = TRUE;
    int comparison = 1;

    return formuleitor(node_idx, edges, type, v0_ip, v0_o, v_ip, comparison);
}

/*
 * E|v| ~C_0 + sum_{i=0}^{E|v|} ~x_{v_i} >= E|v|;
 */
Restriction *c_0_n(int node_idx, Edges* edges) {
    NODE_TYPES type = C0;
    int v0_ip = FALSE, v0_o = edges[node_idx].qtd;
    int v_ip = FALSE;
    int comparison = edges[node_idx].qtd;

    return formuleitor(node_idx, edges, type, v0_ip, v0_o, v_ip, comparison);
}

/*
 *   V_0 | V
 * ~I_v + sum_{i=0}^{E|v|} x_{v_i} >= 1
 */
Restriction *def_initial(int node_idx, Edges *edges) {
    NODE_TYPES type = INITIAL;
    int v0_ip = FALSE, v0_o = 1;
    int v_ip = TRUE;
    int comparison = 1;

    return formuleitor(node_idx, edges, type, v0_ip, v0_o, v_ip, comparison);
}

/*
 *   V_0 | V
 * E|v| * ~I_v + sum_{i=0}^{E|v|} ~x_{v_i} >= E|v| - 1
 */
Restriction *def_initial_n(int node_idx, Edges *edges) {
    NODE_TYPES type = INITIAL;
    int v0_ip = FALSE, v0_o = edges[node_idx].qtd;
    int v_ip = FALSE;
    int comparison = edges[node_idx].qtd - 1;

    return formuleitor(node_idx, edges, type, v0_ip, v0_o, v_ip, comparison);
}

/*
 *   V_0 | V
 * ~F_v + sum_{i=0}^{E|v|} x_{v_i} >= 1
 */
Restriction *def_final(int node_idx, Edges *edges) {
    NODE_TYPES type = FINAL;
    int v0_ip = FALSE, v0_o = 1;
    int v_ip = TRUE;
    int comparison = 1;

    return formuleitor(node_idx, edges, type, v0_ip, v0_o, v_ip, comparison);
}

/*
 *   V_0 | V
 * E|v| ~Fv + sum_{i=0}^{E|v|} ~x_{v_i} >= E|v| - 1
 */
Restriction *def_final_n(int node_idx, Edges *edges) {
    NODE_TYPES type = FINAL;
    int v0_ip = FALSE, v0_o = edges[node_idx].qtd;
    int v_ip = FALSE;
    int comparison = edges[node_idx].qtd - 1;

    return formuleitor(node_idx, edges, type, v0_ip, v0_o, v_ip, comparison);
}

/*
 * V_0 | V
 * 2 ~Pv + sum_{i=0}^{E|v|} x_{v_i} >= 2
 */
Restriction *def_path(int node_idx, Edges *edges) {
    NODE_TYPES type = PATH;
    int v0_ip = FALSE, v0_o = 2;
    int v_ip = TRUE;
    int comparison = 2;

    return formuleitor(node_idx, edges, type, v0_ip, v0_o, v_ip, comparison);
}

/*
 * V_0      | V
 * E|v| ~Pv + sum_{i=0}^{E|v|} ~x_{v_i} >= E|v| - 2
 */
Restriction *def_path_n(int node_idx, Edges *edges) {
    NODE_TYPES type = PATH;
    int v0_ip = FALSE, v0_o = edges[node_idx].qtd;
    int v_ip = FALSE;
    int comparison = edges[node_idx].qtd - 2;

    return formuleitor(node_idx, edges, type, v0_ip, v0_o, v_ip, comparison);
}

 /*
  * ~C0v + ~Iv + ~Fv + ~Pv >= 3
  */
Restriction *def_node_type(int node_idx) {
    int num_restrictions = TOTAL_TYPES - 1;

    Var *vars = (Var*)malloc(num_restrictions*sizeof(Var));

    for(int i = C0; i < TOTAL_TYPES; i++) {
        vars[i - 1] = INIT_VARS(map_nodes_to_variable(node_idx, i), 1, FALSE);
    }

    return get_restrictions(num_restrictions, vars, 3);
}

 /*
  * C0v + Iv + Fv + Pv >= 1
  */
Restriction *def_node_type_n(int node_idx) {
    int num_restrictions = TOTAL_TYPES - 1;

    Var *vars = (Var*)malloc(num_restrictions*sizeof(Var));

    for(int i = C0; i < TOTAL_TYPES; i++) {
        vars[i - 1] = INIT_VARS(map_nodes_to_variable(node_idx, i), 1, TRUE);
    }

    return get_restrictions(num_restrictions, vars, 1);
}

 /*
  * sum_{i=0}^{E|v|} I_v >= 1
  */
Restriction *restrict_initial() {
    int num_restrictions = qt_nodes;
    NODE_TYPES type = INITIAL;

    Var *vars = (Var*)malloc(num_restrictions*sizeof(Var));

    for (int i = 0; i < num_restrictions; i++) {
        vars[i] = INIT_VARS(map_nodes_to_variable(i, type), 1, TRUE);
    }

    return get_restrictions(num_restrictions, vars, 1);
}

/*
 * sum_{i=0}^{E|v|} ~I_v >= |v| - 1
 */
Restriction *restrict_initial_n() {
     int num_restrictions = qt_nodes;
     NODE_TYPES type = INITIAL;

     Var *vars = (Var*)malloc(num_restrictions*sizeof(Var));

     for (int i = 0; i < num_restrictions; i++) {
         vars[i] = INIT_VARS(map_nodes_to_variable(i, type), 1, FALSE);
     }

     return get_restrictions(num_restrictions, vars, qt_nodes - 1);
 }

 /*
  * sum_{i=0}^{E|v|} F_v >= 1
  */
Restriction *restrict_final() {
    int num_restrictions = qt_nodes;
    NODE_TYPES type = FINAL;

    Var *vars = (Var*)malloc(num_restrictions*sizeof(Var));

    for (int i = 0; i < num_restrictions; i++) {
        vars[i] = INIT_VARS(map_nodes_to_variable(i, type), 1, TRUE);
    }

    return get_restrictions(num_restrictions, vars, 1);
}

/*
 * sum_{i=0}^{E|v|} ~F_v >= |v| - 1
 */
Restriction *restrict_final_n() {
     int num_restrictions = qt_nodes;
     NODE_TYPES type = FINAL;

     Var *vars = (Var*)malloc(num_restrictions*sizeof(Var));

     for (int i = 0; i < num_restrictions; i++) {
         vars[i] = INIT_VARS(map_nodes_to_variable(i, type), 1, FALSE);
     }

     return get_restrictions(num_restrictions, vars, qt_nodes - 1);
 }

Restriction *formuleitor(int node_idx, Edges *edges, NODE_TYPES type, int v0_ip, int v0_o, int v_ip, int comparison) {
    int num_restrictions = edges[node_idx].qtd + 1;

    Var *vars = (Var*)malloc(num_restrictions*sizeof(Var));

    vars[0] = INIT_VARS(map_nodes_to_variable(node_idx, type), v0_o, v0_ip);

    for (int i = 0; i < num_restrictions - 1; i++) {
        int idx = get_idx(node_idx, edges[node_idx].neighbors[i]);
        vars[i+1] = INIT_VARS(map_edges_to_variable(idx), 1, v_ip);
    }

    return get_restrictions(num_restrictions, vars, comparison);
}

/*
 * Utils functions
 */
int get_idx(int node, int edge) {
    int idx = get_ev_index(node, edge);

    if(ev[idx] != -1) return ev[idx];

    printf("Could not find edge: %d - %d\nEV_idx: %d\nEV: %d\n", node, edge, idx, ev[idx]);

    exit(-1);
}

int map_nodes_to_variable(int idx, NODE_TYPES type) {
    return idx * 4 + type;
}

int map_edges_to_variable(int idx) {
    return qt_nodes * 4 + idx + 1;
}

void free_restriction(Graph_Restriction * gr) {
    for (int i = 0; i < gr->qt; i++) {
        int num_gr_restrictions = gr[i].qt;

        for (int j = 0; j < num_gr_restrictions; j++) {
            Restriction * curr_restriction = gr[i].restrictions[j];

            free(curr_restriction->vars);
            free(curr_restriction);
        }
    }
    free(gr);
}
