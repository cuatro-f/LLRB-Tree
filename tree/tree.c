#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "tree.h"
#include "../input/read.h"
#include "../debug/debug.h"

Data* create_data(size_t info) {
	Data *data = calloc(1, sizeof(Data));
	if (data == NULL) {
		return NULL;
	}
	data->info = info;
	return data;
}

Item* create_item(size_t data) {
	Item *item = calloc(1, sizeof(Item));
	item->data = create_data(data);
	if (item->data == NULL) {
		return NULL;	
	}
	item->release = 1;
	return item;
}

Tree* create_tree() {
	Tree *tree = calloc(1, sizeof(Tree));
	tree->root = NULL;
}

Node* create_node(char *key, size_t data, Node *parent) {
	Node *leaf = calloc(1, sizeof(Node));
	leaf->info = create_item(data);
	leaf->key = key;
	leaf->parent = parent;
	// у только созданных узлов красный цвет
	leaf->color = red;
	leaf->left = NULL;
	leaf->right = NULL;
	return leaf;
}

int add_node(Item **item, size_t data) {
	Item *head = *item, *new = create_item(data);
	new->release = (head->release) + 1;
	new->next = head;
	*item = new;
	return 0;
}

void switch_color(Node *node, Node *other) {
	int color = node->color;
	node->color = other->color;
	other->color = color;
}

void right_rotate(Tree *tree, Node *x) {
	Node *y = x->left;
    	x->left = y->right;
    	if (y->right != NULL) {
        	y->right->parent = x;
    	}
    	y->parent = x->parent;
    	if (x == tree->root) {
		tree->root = y;
		y->parent = NULL;
	}
	else if (x == x->parent->right) {
        	x->parent->right = y;
    	}
	else {
        	x->parent->left = y;
    	}
    	y->right = x;
    	x->parent = y;
}

void left_rotate(Tree *tree, Node *node) {
	Node *right_node = node->right;
	Node *parent = node->parent;
	node->right = right_node->left;
	if (right_node->left != NULL) {
		right_node->left->parent = node;
	}
	right_node->parent = parent;
	// то есть это корень
	if (tree->root == node) {
		tree->root = right_node;
		right_node->parent = NULL;
	}
	else {
		if (parent->left == node) {
			parent->left = right_node;
		}
		else {
			parent->right = right_node;
		}
	}
	right_node->left = node;
	node->parent = right_node;
}

int insert(Tree *tree, char *key, size_t data) {
	// обычная вставка в rb дерево
	Node *prev = NULL;
	Node *ptr = tree->root;
	while (ptr != NULL) {
		prev = ptr;
		if (strcmp(ptr->key, key) == 0) {
			int check = add_node(&(ptr->info), data);
			free(key);
			return 0;
		}
		ptr = strcmp(ptr->key, key) > 0 ? ptr->left : ptr->right;
	}
	Node *new = create_node(key, data, prev);
	if (prev == NULL) {
		tree->root = new;
	}
	else {
		if (strcmp(new->key, prev->key) > 0) {
			prev->right = new;
		}
		else {
			prev->left = new;
		}
	}
	insert_fixup(tree, new);
	return 0;
}

void insert_fixup(Tree *tree, Node *node) {
	// родитель у root = NULL
	while (node != NULL) {
		if (isRed(node->right) && !isRed(node->left)) {
			left_rotate(tree, node);
			node = node->parent;
			switch_color(node, node->left);
		}
		if (isRed(node->left) && isRed(node->left->left)) {
			right_rotate(tree, node);
			node = node->parent;
			switch_color(node, node->right);
		}
		if (isRed(node->left) && isRed(node->right)) {
			node->color = node->color == red ? black : red;
			node->left->color = black;
			node->right->color = black;
		}
		node = node->parent;
	}
	tree->root->color = black;
}

void delete_item(Node *node, size_t version) {
	if (node == NULL || node->info == NULL) {
        	return;
	}
    	if (node->info->release == version) {
        	Item *current = node->info;
        	node->info = current->next;
        	free_data(current->data);
		free(current);
        	return;
    	}
    	Item *current = node->info;
    	while (current->next) {
     		if (current->next->release == version) {
        		Item *tmp = current->next;
            		current->next = tmp->next;
            		free_data(tmp->data);
			free(tmp);
            		return;
        	}
        	current = current->next;
    	}
}

// Функции поворота для балансировки дерева
Node* rotateLeft(Tree *tree, Node *h) {
    	Node *x = h->right;
    	h->right = x->left;
	x->parent = h->parent;
	h->parent = x;
    	if (x->left != NULL) {
		x->left->parent = h;
    	}
	if (tree->root = h) {
		tree->root = x;
	}
    	x->left = h;
    	x->color = h->color;
    	h->color = red;
    	return x;
}

Node* rotateRight(Tree *tree, Node *h) {
    	Node *x = h->left;
    	h->left = x->right;
	x->parent = h->parent;
	h->parent = x;
	if (x->right != NULL) {
		x->right->parent = h;
	}
	if (h == tree->root) {
		tree->root = x;
	}
    	x->right = h;
    	x->color = h->color;
    	h->color = red;
    	return x;
}

int isRed(Node *node) {
	if (node == NULL) {
		return 0;
	}
	return node->color == red;
}

void flipColors(Node *h) {
	if (h == NULL) {
		return;
	}
    	h->color = h->color == red ? black : red;
    	h->left->color = h->left->color == red ? black : red;
    	h->right->color = h->right->color == red ? black : red;
}

Node* moveRedLeft(Tree* tree, Node *h) {
    	flipColors(h);
    	if (isRed(h->right->left)) {
        	h->right = rotateRight(tree, h->right);
        	h = rotateLeft(tree, h);
        	flipColors(h);
    	}
    	return h;
}

Node* moveRedRight(Tree *tree, Node *h) {
    	flipColors(h);
    	if (isRed(h->left->left)) {
        	h = rotateRight(tree, h);
        	flipColors(h);
    	}
    	return h;
}

Node* fixUp(Tree *tree, Node *h) {
    	if (isRed(h->right)) {
        	h = rotateLeft(tree, h);
    	}
    	if (isRed(h->left) && isRed(h->left->left)) {
        	h = rotateRight(tree, h);
    	}
    	if (isRed(h->left) && isRed(h->right)) {
        	flipColors(h);
    	}
    	return h;
}

Node* deleteMin(Tree *tree, Node *h) {
    	if (h->left == NULL) {
		if (h->right) {
			h->right->parent = h->parent;
		}
		if (h->parent->left == h) {
			h->parent->left = NULL;
		}
		else {
			h->parent->right = NULL;
		}
		// если есть segfault, то он тут
		free_node(h);
        	return NULL;
    	}
    	if (!isRed(h->left) && !isRed(h->left->left)) {
        	h = moveRedLeft(tree, h);
    	}
    	h->left = deleteMin(tree, h->left);
    	return fixUp(tree, h);
}

int remove_node(Tree *tree, char *key, size_t version) {
	Node *found = find_node_by_key(tree, key);
	if (found == NULL) {
		return 1;
	}
	delete_item(found, version);
	if (found->info != NULL) {
		return 0;
	}
	tree->root = remove_by_key(tree, tree->root, key);
	if (tree->root) {
		tree->root->color = black;
		return 0;
	}
	tree->root = NULL;
	return 0;
}

Node* remove_by_key(Tree *tree, Node *h, char *key) {
	if (h == NULL) {
		return h;
	}
	if (strcmp(key, h->key) < 0) {
        	if (!isRed(h->left) && !isRed(h->left->left)) {
            		h = moveRedLeft(tree, h);
        	}
        	h->left = remove_by_key(tree, h->left, key);
    	}
	else {
        	if (isRed(h->left)) {
            		h = rotateRight(tree, h);
        	}
        	if (strcmp(key, h->key) == 0 && (h->right == NULL)) {
			if (h->left) {
				h->left->parent = h->parent;
			}
			free_node(h);
            		return NULL;
        	}
        	if (!isRed(h->right) && !isRed(h->right->left)) {
            		h = moveRedRight(tree, h);
        	}
        	if (strcmp(key, h->key) == 0) {
			/*if (h->left == NULL) {
				Node *tmp = h->right;
				if (tmp) {
					tmp->parent = h->parent;
				}
				free_node(h);
				return tmp;
			}
			else if (h->right == NULL) {
				Node *tmp = h->left;
				if (tmp) {
					tmp->parent = h->parent;
				}
				free_node(h);
				return tmp;
			}*/
			Node *p = h;
			Node *tmp = replace_node(h->right, &p);
			free(h->key);
			h->key = strdup(tmp->key);
			// тут возможно надо помнять родителя
            		h->right = deleteMin(tree, h->right);
        	}
		else {
            		h->right = remove_by_key(tree, h->right, key);
        	}
    	}
    	return fixUp(tree, h);
}

Node* replace_node(Node* node, Node **parent) {
	Node* current = node;
	if (current == NULL) {
		return current;
	}
	while (current && current->left != NULL) {
		*parent = current;
		current = current->left;
	}
	return current;
}

Node *find_min(Node *node) {
	Node *min = node;
	if (min == NULL) {
		return NULL;
	}
	while (min->left) {
		min = min->left;
	}
	return min;
}

void detour(Tree *tree, char *key) {
	inorder(tree->root, key);
}

void inorder(Node *node, char *key) {
	if (node == NULL) {
		return;
	}
	if (strcmp(node->key, "elist") == 0) {
		return;
	}
	inorder(node->left, key);
	if (strcmp(node->key, key) < 0) {
		printf("%s ", node->key);
	}
	inorder(node->right, key);
}

void detour_wo_print(Node *tree) {
	if (tree == NULL) {
		return;
	}
	detour_wo_print(tree->left);
	detour_wo_print(tree->right);
}

Node* special_find(Tree *tree, char *key, size_t version, char **found_key) {
	Node *prev = NULL, *result = NULL;
	int delta = INT_MIN;
	rec_find(tree->root, prev, &result, key, &delta);
	return result;
}

void rec_find(Node *node, Node *prev, Node **result, char *key, int *delta) {
	if (node == NULL) {
		return;
	}
	if (strcmp(node->key, key) < 0 && strcmp(node->key, key) > *delta) {
		printf("delta for %s equal %d\n", node->key, strcmp(node->key, key));
		*delta = strcmp(node->key, key);
		*result = node;
	}
	rec_find(node->left, node, result, key, delta);
	rec_find(node->right, node, result, key, delta);
}

Item* find_by_key(Tree *tree, char *key, size_t version) {
	Node *root = tree->root;
	while (root) {
		if (strcmp(root->key, key) == 0) {
			Item *current = root->info;
			while (current) {
				if (current->release == version) {
					return current;
				}
				current = current->next;
			}
			return NULL;
		}
		else if (strcmp(root->key, key) < 0) {
			root = root->right;
			continue;
		}
		root = root->left;
	}
	return NULL;
}

Node* find_node_by_key(Tree *tree, char *key) {
	Node *root = tree->root;
	while (root) {
		if (strcmp(root->key, key) == 0) {
			return root;
		}
		root = strcmp(root->key, key) > 0 ? root->left : root->right;
	}
	return NULL;
}

void print_tree(Node *node, int depth) {
	if (node == NULL) {
		return;
	}
	for (int i = 0; i < depth; i++) {
		printf("    ");
	}
	char *par = node->parent == NULL ? "NULL" : node->parent->key;
	printf("── color %s %s parent %s ", node->color == 0 ? "black" : "red", node->key, par);
	print_item(node->info);
	printf("\n");
	depth++;
	print_tree(node->right, depth);
	print_tree(node->left, depth);
}

void free_data(Data *data) {
	free(data);
}

void free_item(Item *item) {
	while (item) {
		Item *prev = item;
		item = item->next;
		free_data(prev->data);
		free(prev);
	}
}

void free_node(Node *node) {
	if (node) {
		if (node->info) {
			free_item(node->info);
		}
		if (node->key) {
			free(node->key);
		}
		free(node);
	}
}

void free_nodes(Node *node) {
	if (node) {
		if (node->left) {
			free_nodes(node->left);
		}
		if (node->right) {
			free_nodes(node->right);
		}
		free_node(node);
	}
}

void free_tree(Tree *tree) {
	free_nodes(tree->root);
	free(tree);
}

int import_from_file(Tree *tree, char *filename) {
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return 0;
	}
	char *key = NULL, *data = NULL;
	do {
		key = myreadfile(file);
		if (key == NULL) {
			free(key);
			break;
		}
		data = myreadfile(file);
		if (data == NULL) {
			free(key);
			break;
		}
		size_t num = atoi(data);
		free(data);
		int check = insert(tree, key, num);
	} while (key != NULL && data != NULL);
	fclose(file);
	return 0;
}

void print_list(Item *item) {
	while (item) {
		printf("data %zu, ver. %zu| ", item->data->info, item->release);
		item = item->next;
	}
}

void print_item(Item *item) {
	if (item == NULL) {
		return;
	}
	if (debug == 0) {
		printf("");
	}
	else if (debug == 2) {
		print_list(item);
	}
	else if (debug == 1) {
		printf("ver. %zu, data %zu", item->release, item->data->info);
	}
}

void parse_file(Tree *tree, char *filename) {
	FILE *file = fopen(filename, "r");
	char *string = myreadfile(file);
	char PROMPT[] = "!-—,./'&? 	;:#$%^@№`()[]«»\"…";
	size_t number = 0;
	while (string) {
		number++;
		char *word = strtok(string, PROMPT);
		while (word) {
			char *key = lower(word);
			int check = insert(tree, key, number);
			word = strtok(NULL, PROMPT);
		}
		free(string);
		string = myreadfile(file);
	}
}

char* lower(char *key) {
	char *new_key = strdup(key);
	char eng_high = 'Z', eng_low = 'z', eng_top = 'A';
	for (int i = 0; i < strlen(key); i++) {
		if (key[i] >= eng_top && key[i] <= eng_high){
			new_key[i] += eng_low - eng_high;
		}
	}
	return new_key;
}

int fast_find(Tree *tree, char *word, char *filename, size_t **arr, int *len) {
	if (filename != NULL) { 
		parse_file(tree, filename);
	}
	Node *found = find_node_by_key(tree, word);
	if (found == NULL) {
		return 0;
	}
	Item *item = found->info;
	while (item) {
		if (*len > 0) {
			if (item->data->info == (*arr)[*len - 1]) {
				item = item->next;
				continue;
			}
		}
		(*len)++;
		*arr = realloc(*arr, sizeof(size_t) * (*len));
		(*arr)[*len - 1] = item->data->info;
		item = item->next;
	}
	return 0;
}
