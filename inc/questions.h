#ifndef QUESTIONS_H
#define QUESTIONS_H

#include "restrictions.h"
#include "solver.h"

#define QUESTION_PATH 0
#define QUESTION_FOAF 1
#define QUESTION_FRIENDS 2
#define QUESTION_EXIT -1

extern int qt_nodes, qt_edges;
extern char* file_name;
extern char **restriction_name;

/*
 * Question based restrictions
*/
void handle_questions(char **nodes, GraphHead *gr);
QRestrictions* handle_path(char *n1, char *n2, char **nodes);
QRestrictions* handle_foaf(char *n, char **nodes);
QRestrictions* handle_friends(char *n, char **nodes);

char *write_question_file(QRestrictions *qr, GraphHead *gr, char *n);

#endif
