#ifndef TREE_H
#define TREE_H
#include <stdlib.h>

enum COLORS {
	black = 0,
	red = 1,
};

typedef struct Data {
	size_t info;
} Data;

typedef struct Item {
	Data *data;
	struct Item *next;
	size_t release;
} Item;

typedef struct Node {
	// 0 - черный, 1 - красный
	int color;
	struct Node *right;
	struct Node *left;
	struct Node *parent;
	char *key;
	Item *info;
} Node;

typedef struct Tree {
	Node *root;
} Tree;


Node* moveRedLeft(Tree*, Node*);
Node *moveRedRight(Tree*, Node*);
Node *fixup(Tree*, Node*);
/*
void deleteMin(Tree*);
Node *recdeleteMin(Tree*, Node*);
Node *recDelete(Tree*, Node*, char*, size_t);
Node *moveRedLeft(Tree*, Node*);
Node *moveRedRight(Tree*, Node*);
void deleteNode(Tree*, char*, size_t);*/
int isRed(Node*);
int remove_node(Tree*, char*, size_t);
Node* deleteMin(Tree*, Node*);
int isRed(Node*); //+
Node* rotateLeft(Tree*, Node*); //+
Node* rotateRight(Tree*, Node*); //+
void flipColors(Node*);
Node *remove_by_key(Tree*, Node*, char*);
//int remove_by_key(Tree*, char*, size_t);
void refresh_root(Tree*);
Data *create_data(size_t); //+
Item *create_item(size_t); //+
Node *create_root(); //+
Tree *create_tree(); //+
Node *create_node(char*, size_t, Node*);
void left_rotate(Tree*, Node*); // +
void right_rotate(Tree*, Node*); // +
void switch_color(Node*, Node*); //+
void colorflip(Node*);
int insert(Tree*, char*, size_t); // написать
void insert_fixup(Tree*, Node*);
int add_node(Item**, size_t); // +
void detour(Tree*, char*); // переделать
Item* find_by_key(Tree*, char*, size_t); // скорее всего работает верно
Node* special_find(Tree*, char*, size_t, char**); // скорее всего работает
void print_tree(Node*, int); //+
void print_item(Item *item); //+
void free_tree(Tree*); // +
void free_data(Data*); // +
void free_nodes(Node*); // +
void free_node(Node*); // +
void free_item(Item*); // +
void inorder(Node*, char*);
int import_from_file(Tree*, char*); // +
void rec_find(Node*, Node*, Node**, char*, int*);
Node* replace_node(Node*, Node**); // возможно не нужно, зависит от реализации удаления
void print_list(Item*); // +
void delete_item(Node*, size_t); // написать
Node *find_min(Node*);
Node *find_node_by_key(Tree*, char*);
void detour_wo_print(Node*); //+
int fast_find(Tree*, char*, char*, size_t**, int*);

void parse_file(Tree*, char*);
char *lower(char*);
#endif
