#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "tree/tree.h"
#include "dialog/dialog.h"
#include "debug/debug.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("wrong arguments count\n");
		return 1;
	}
	for (int i = 0; i < strlen(argv[1]); i++) {
		if (isdigit(argv[1][i]) == 0) {
			printf("argument should be an integer\n");
			return 1;
		}
	}
	debug = atoi(argv[1]);
	printf("debug %d\n", debug);
	if (debug > 2) {
		printf("your argument is wrong. using standart debug leve: 0");
		debug = 0;
	}
	Tree *tree = create_tree();
	if (tree == NULL) {
		return 1;
	}
	int check = process(tree);
	free_tree(tree);
	return 0;
}
