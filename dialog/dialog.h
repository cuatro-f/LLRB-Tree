#ifndef DIALOG_H
#define DIALOG_H
#include <stdlib.h>
#include "../tree/tree.h"

int dialog_exit(Tree*); //+ 
int dialog_remove_by_key(Tree*); //+ 
int dialog_add(Tree*); //+
int dialog_detour(Tree*); //+
int dialog_print(Tree*); //+
int dialog_find_by_key(Tree*); //+
int dialog_special_find(Tree*); //+
int dialog_import(Tree*);
int dialog_visualize(Tree*);
int dialog_load_fast_find(Tree*);
int dialog_fast_find(Tree*);
int process(Tree*);
int get_key(char**);
int get_value(size_t);
int get_int(size_t*, int(*)(size_t), char*);
int dialog(const char**, const int);
#endif 
