#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree/tree.h"

void time_insert();
void time_detour();
void time_delete();
void time_find();

int main(int argc, char* argv[]) {
	srand(time(NULL));
	if (argc != 2) {
		printf("wrong arguments count\n");
		return 1;
	}
	if (strcmp(argv[1], "insert") == 0) {
		time_insert();
	}
	else if (strcmp(argv[1], "delete") == 0) {
		time_delete();
	}
	else if (strcmp(argv[1], "detour") == 0) {
		time_detour();
	}
	else if (strcmp(argv[1], "find") == 0) {
		time_find();
	}
}
char *random_str(int length) {
	char symbols[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
	char *new_str = calloc(length + 1, sizeof(char));
	if (new_str == NULL) {
		return NULL;
	}
	for (int i = 0; i < length; i++) {
		int symbol = rand() % (int)(sizeof(symbols) - 1);
		new_str[i] = symbols[symbol];
	}
	new_str[length] = '\0';
	return new_str;
}

void generate_tree(Tree* tree, int count) {
	for (int i = 0; i < count; i++) {
		char *key = random_str(rand() % 10);
		size_t data = rand() % 1000000;
		int check = insert(tree, key, data);
	}
}

void time_find() {
	for (int i = 50000; i < 310000; i += 20000) {
		clock_t total = 0;
		Tree *tree = create_tree();
		generate_tree(tree, i);
		for (int j = 0; j < 1000; j++) {
			clock_t start = clock();
			char *key = random_str(rand() % 10);
			find_by_key(tree, key, 1);
			clock_t end = clock();
			free(key);
			total += end - start;
		}
		free_tree(tree);
		printf("%d %ld\n", i, total / 1000);
	}
}

void time_delete() {
        for (int i = 1000; i < 21000; i += 1000) {
                clock_t total = 0;
		for (int num = 0; num < 15; num++) {
			Tree *tree = create_tree();
			generate_tree(tree, i);
                	for (int j = 0; j < 1000; j++) {
                        	clock_t start = clock();
				char *key = random_str(rand() % 10);
                        	remove_node(tree, key, 1);
                        	clock_t end = clock();
				free(key);
                        	total += end - start;
                	}
			free_tree(tree);
		}
                printf("%d %ld\n", i, total / 15);
        }
}

void time_insert() {
	for (int i = 1000; i < 21000; i += 1000) {
		clock_t total = 0;
		for (int num = 0; num < 15; num++) {
			Tree *tree = create_tree();
			generate_tree(tree, i);
			for (int j = 0; j < 1000; j++) {
				char *key = random_str(rand() % 10);
				size_t data = rand() % 10000000;
				clock_t start = clock();
				int check = insert(tree, key, data);
				clock_t end = clock();
				total += end - start;
				//free(key);
			}
			free_tree(tree);
		}
		printf("%d %ld\n", i, total / 15);
	}
}

void time_detour() {
	for (int i = 20000; i < 350000; i += 20000) {
		clock_t total = 0;
		Tree *tree = create_tree();
		generate_tree(tree, i);
		for (int j = 0; j < 15; j++) {
			clock_t start = clock();
			detour_wo_print(tree->root);
			clock_t end = clock();
			total += end - start;
		}
		free_tree(tree);
		printf("%d %ld\n", i, total / 15);
	}
}
