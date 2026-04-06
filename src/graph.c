#include"graph.h"

char separator[15] = "---------------";
extern int qt_nodes, qt_edges;
extern int *ev;

void read_graph(int qt_nodes, int qt_edges, Edges *edges, char **nodes) {
    int ev_counter = 0;
    for(int i = 0; i < qt_nodes; i++) {
        scanf("%s", nodes[i]);
        getchar();

        edges[i].neighbors = (int*)malloc(qt_nodes * sizeof(int));
        edges[i].qtd = 0;
    }

    for(int i = 0; i < qt_edges; i++) {
        char n1[5], n2[5];

        scanf("%s %s", n1, n2);
        getchar();

        int index = get_node(nodes, n1);
        int neighbor = get_node(nodes, n2);

        int ev_index = get_ev_index(index, neighbor);

        if(ev[ev_index] == -1) {
            ev[ev_index] = ev_counter++;

            int n_count = edges[index].qtd++;
            edges[index].neighbors[n_count] = neighbor;

            n_count = edges[neighbor].qtd++;
            edges[neighbor].neighbors[n_count] = index;
        }
    }
}

void print_graph_structure(char **nodes, Edges *edges, int qt_nodes) {
    printf("%s\n", separator);
    printf("NODES:\n");

    for(int i = 0; i < qt_nodes; i++) {
        printf("%s ", nodes[i]);
    }

    printf("\n%s\n", separator);
    printf("EDGES");

    for(int i = 0; i < qt_nodes; i++) {
        printf("\nVizinhos de %s(%d): ", nodes[i], edges[i].qtd);

        for(int j = 0; j < edges[i].qtd; j++) {
            printf("%s ", nodes[edges[i].neighbors[j]]);
        }
    }

    printf("\n%s\n", separator);
}

int get_node(char **nodes, char *n) {
  for(int i = 0; i < qt_nodes; i++) {
    if(strcmp(nodes[i], n) == 0) {
      return i;
    }
  }

  printf("Could not found value: %s in nodes.\n", n);
  exit(-1);
}

int get_ev_index(int node, int edge) {
    // i * (2*N - i - 1) / 2 + j - i - 1

    if (node == edge) {
        printf("Não pode haver aresta de um nó para ele mesmo.\nnode: %d\tedge:%d\n", node, edge);
        exit(-1);
    }
    int i = MIN(node, edge);
    int j = MAX(node, edge);

    return (i * (2 * qt_nodes - i - 1) / 2) + j - i - 1;
}
