#include "questions.h"

char *question_name[20] = {
    "PATH",
    "FOAF",
    "FRIENDS"
};

extern int benchmark;

void handle_questions(char **nodes, GraphHead *head) {
    int option;
    char n1[5], n2[5], n[5];

    // printf("Handling questions...\n");

    while(scanf("%d", &option) != -1) {
        QRestrictions *qr = NULL;
        char node[15];

        switch(option) {
            case QUESTION_PATH:
                scanf("%s %s", n1, n2);
                qr = handle_path(n1, n2, nodes);
                sprintf(node, "%d-%s-%s", QUESTION_PATH, n1, n2);
                break;
            case QUESTION_FOAF:
                scanf("%s", n);
                qr = handle_foaf(n, nodes);
                sprintf(node, "%d-%s", QUESTION_FOAF, n);
                break;
            case QUESTION_FRIENDS:
                scanf("%s", n);
                qr = handle_friends(n, nodes);
                sprintf(node, "%d-%s", QUESTION_FRIENDS, n);
                break;
            case QUESTION_EXIT:
                return;
            default:
                printf("Option \"%d\" not found.\n", option);
                exit(1);
                break;
        }

        qr->question = option;

        char * _file_name = malloc(100*sizeof(char));

        if (!benchmark) {
            _file_name = write_question_file(qr, head, node);

            // run_solver(_file_name);
        }

        free(_file_name);

        if(qr != NULL) {        
            free(qr->question_restrictions);
            free(qr);
        }
    }
}

QRestrictions* handle_path(char *n1, char *n2, char **nodes) {
    int idx_1, idx_2;
    int qt = 2;

    idx_1 = get_node(nodes, n1);
    idx_2 = get_node(nodes, n2);

    Restriction *r = (Restriction*)malloc(qt*sizeof(Restriction));

    r[0].vars = (Var*)malloc(sizeof(Var));
    r[1].vars = (Var*)malloc(sizeof(Var));

    r[0].vars[0] = INIT_VARS(map_nodes_to_variable(idx_1, INITIAL), 1, TRUE);
    r[0].comparison = 1;
    r[0].qt = 1;

    r[1].vars[0] = INIT_VARS(map_nodes_to_variable(idx_2, FINAL), 1, TRUE);
    r[1].comparison = 1;
    r[1].qt = 1;

    QRestrictions* qr = (QRestrictions*)malloc(sizeof(QRestrictions));
    qr->question_restrictions = r;
    qr->qt = qt;

    return qr;
}

QRestrictions *handle_foaf(char *n, char **nodes){
    int idx = get_node(nodes, n);
    int qt = 3;

    Restriction *r = (Restriction*)malloc(qt * sizeof(Restriction));

    r[0].vars = (Var*) malloc(sizeof(Var));
    r[1].vars = (Var*) malloc(qt_nodes * sizeof(Var));
    r[2].vars = (Var*) malloc(qt_nodes * sizeof(Var));

    r[0].vars[0] = INIT_VARS(map_nodes_to_variable(idx, INITIAL), 1, TRUE);
    r[0].comparison = 1;
    r[0].qt = 1;

    for (int i = 0; i < qt_nodes; i++) {
        r[1].vars[i] = INIT_VARS(map_nodes_to_variable(i, PATH), 1, TRUE);
        r[2].vars[i] = INIT_VARS(map_nodes_to_variable(i, PATH), 1, FALSE);
    }

    r[1].comparison = 1;
    r[1].qt = qt_nodes;

    r[2].comparison = qt_nodes - 1;
    r[2].qt = qt_nodes;

    QRestrictions *qr = (QRestrictions*)malloc(sizeof(QRestrictions));
    qr->question_restrictions = r;
    qr->qt = qt;

    return qr;
}

QRestrictions *handle_friends(char *n, char **nodes){
    int idx = get_node(nodes, n);
    int qt = 3;

    Restriction *r = (Restriction*)malloc(qt*sizeof(Restriction));

    r[0].vars = (Var*)malloc(sizeof(Var));
    r[0].vars[0] = INIT_VARS(map_nodes_to_variable(idx, INITIAL), 1, TRUE);
    r[0].comparison = 1;
    r[0].qt = 1;

    r[1].vars = (Var*)malloc(qt_nodes*sizeof(Var));
    r[2].vars = (Var*)malloc(qt_nodes*sizeof(Var));

    for (int i = 0; i < qt_nodes; i++) {
        r[1].vars[i] = INIT_VARS(map_nodes_to_variable(i, PATH), 1, TRUE);
        r[2].vars[i] = INIT_VARS(map_nodes_to_variable(i, PATH), 1, FALSE);
    }

    r[1].comparison = 0;
    r[1].qt = qt_nodes;

    r[2].comparison = qt_nodes;
    r[2].qt = qt_nodes;

    QRestrictions *qr = (QRestrictions*)malloc(sizeof(QRestrictions));
    qr->question_restrictions = r;
    qr->qt = qt;

    return qr;
}

char* write_question_file(QRestrictions *qr, GraphHead *head, char *n) {
    char *_file_name = malloc(50*sizeof(char));
    char *_null_case = malloc(10* sizeof(char));

    if (file_name == NULL) {
        sprintf(_null_case, "%d-%d", qt_nodes, qt_edges);
    }
    
    sprintf(_file_name, "./restricoes/SAT-%s.%s.pbs", file_name == NULL ? _null_case : file_name, n);

    free(_null_case);
    
    printf("Writing into: %s\n", _file_name);

    FILE *fp = fopen(_file_name, "w");

    if(fp == NULL) {
        printf("Cannot open file\n");
        exit(1);
    }

    fprintf(fp, "* #variable=%d #constraint=%d\n", (qt_nodes * 4) + qt_edges, 10 * qt_nodes + 4 + qr->qt);

    if (qr->question == QUESTION_PATH) {
        fprintf(fp, "min: ");

        for(int i = 0; i < qt_nodes; i++) {
            fprintf(fp, " +1 x%d", map_nodes_to_variable(i, PATH));
        }

        fprintf(fp, ";\n");
    }

    for (int c = 0; c < head->qt; c++){
        for(int i = 0; i < head->gr[c].qt; i++) {
            Restriction * current_restriction = head->gr[c].restrictions[i];

            for(int j = 0; j < current_restriction->qt; j++){
                fprintf(fp, "+%d %sx%d ", \
                    current_restriction->vars[j].offset,\
                    current_restriction->vars[j].is_positive == TRUE ? "" : "~",\
                    current_restriction->vars[j].value);
            }
            fprintf(fp, ">= %d;\n", current_restriction->comparison);
        }
    }

    fprintf(fp, "* %s\n", question_name[qr->question]);

    for (int i = 0; i < qr->qt; i++) {
        Restriction * current_restriction = &qr->question_restrictions[i];

        for(int j = 0; j < current_restriction->qt; j++){
            fprintf(fp, "+%d %sx%d ", \
                current_restriction->vars[j].offset,\
                current_restriction->vars[j].is_positive == TRUE ? "" : "~",\
                current_restriction->vars[j].value);
        }
        fprintf(fp, ">= %d;\n", current_restriction->comparison);
    }

    fclose(fp);

    return _file_name;
}
