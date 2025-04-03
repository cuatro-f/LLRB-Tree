#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include <stdio.h>
#include "../tree/tree.h"
void print_dot_null(char*, int, FILE*);
void print_dot_aux(Node*, FILE*);
void print_dot(Node*, FILE*);
#endif
