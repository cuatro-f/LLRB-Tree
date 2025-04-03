#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dialog.h"
#include "../tree/tree.h"
#include <readline/readline.h>
#include "../visualization/visualization.h"
#include "../input/read.h"

int get_key(char **key) {
	*key = readline("Enter key: ");
    	if (*key == NULL) {
		return EOF;
 	}
    	return 0;
}

int get_value(size_t value) {
	return 1;
}

int get_index(size_t index) {
    return (index >= 0 && index <= 10);
}

int get_int(size_t* number, int(*func)(size_t), char* str) {
    int flag = 1;
    do {
        printf("%s: ", str);
        int check = scanf("%zu", number);
        if (check == EOF) {
            return check;
        }
        if (func(*number) == 1 && check == 1) {
            return 0;
        }
	printf("\n");
        scanf("%*[^\n]");
        scanf("%*c");
    } while (flag);
    return 1;
}

int dialog(const char* msgs[], const int len) {
    char* errmsg = "";
    int check = 0;
    size_t number = 0;
    do {
        puts(errmsg);
        errmsg = "Repeate input";
        for (int i = 0; i < len; i++) {
            puts(msgs[i]);
        }
        check = get_int(&number, &get_index, "choose one option");
        if (check == EOF) {
            return 0;
        }
    } while (number < 0 || number >= len);
    return number;
}

int process(Tree* tree) {
    const char* msgs[] = { "0. Quit", "1. Add", "2. Remove by key", "3. Detour", "4. Find by key", "5. Special find", "6. Print tree", "7. Import", "8. Visualize", "9. Load fast find", "10. Fast find"};
    const int len = sizeof(msgs) / sizeof(msgs[0]);
    int check = 0;
    int flag = 1;
    int (*funcs[]) (Tree*) = {&dialog_exit, &dialog_add, &dialog_remove_by_key, &dialog_detour, &dialog_find_by_key, &dialog_special_find, &dialog_print, &dialog_import, &dialog_visualize, &dialog_load_fast_find, &dialog_fast_find}; 
    while (flag) {
        int index = dialog(msgs, len);
	check = funcs[index](tree);
	if (check == -1) {
		flag = 0;
		return EOF;
	}
    }
}

int dialog_exit(Tree* tree) {
	return -1;
}

int dialog_print(Tree *tree) {
	print_tree(tree->root, 0);
	return 0;
}

int dialog_remove_by_key(Tree *tree) {
    	char *key = NULL;
    	int check = get_key(&key);
    	size_t version = 0;
    	if (check == EOF) {
    	    return EOF;
    	}
    	check = get_int(&version, &get_value, "Input version");
    	if (check == EOF) {
		return EOF;
	}
    	check = remove_node(tree, key, version);
    	free(key);
    	return check;
}

int dialog_find_by_key(Tree *tree) {
    	char *key = NULL;
    	int check = get_key(&key);
    	if (check == EOF) {
        	return EOF;
    	}
	size_t version = 0;
	check = get_int(&version, &get_value, "Input version");
	if (check == EOF) {
		return EOF;
	}
    	Item *new_item = find_by_key(tree, key, version);
    	if (new_item == NULL) {
		free(key);
		printf("Not found\n");
		return 0;
	}
    	printf("Found item:\n");
	print_item(new_item);
	printf("\n");
    	free(key);
    	return 0;
}

int dialog_add(Tree *tree) {
    	size_t info_value = 0;
    	char* key = NULL;
   	int check = get_key(&key);
    	if (check == EOF) {
        	return EOF;
    	}
    	check = get_int(&info_value, &get_value, "input info");
    	if (check == EOF) {
	    	free(key);
        	return EOF;
    	}
    	check = insert(tree, key, info_value);
    	if (check == 1) {
        	return -2;
    	}
    	return check;
}

int dialog_special_find(Tree *tree) {
	char *key = NULL, *found_key = NULL;
	size_t version = 0;
	int check = get_key(&key);
	if (check == EOF) {
		return EOF;
	}
	check = get_int(&version, &get_value, "input version");
	if (check == EOF) {
		free(key);
		return EOF;
	}
	Node *node = special_find(tree, key, version, &found_key);
	if (node == NULL) {
		free(key);
		return 0;
	}
	Item *cur = node->info;
	while (cur) {
		if (cur->release == version) {
			printf("Found key %s\n", node->key);
			printf("Found item:\n");
			print_item(node->info);
			printf("\n");
			break;
		}
		cur = cur->next;
	}
	free(key);
	return 0;
}

int dialog_detour(Tree *tree) {
	char *key = NULL;
	int check = get_key(&key);
	if (check == EOF) {
		return EOF;
	}
	detour(tree, key);
	free(key);
	return 0;
}

int dialog_import(Tree *tree) {
	scanf("%*c");
	char *filename = readline("Input filename ");
	if (filename == NULL) {
		printf("filename error\n");
		return 0;
	}
	remove_endspace(filename);
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		free(filename);
		return 1;
	}
	fclose(file);
	int check = import_from_file(tree, filename);
	free(filename);
	return check;
}

int dialog_visualize(Tree* tree) {
	scanf("%*c");
	char *filename = readline("Input filename ");
	if (filename == NULL) {
		printf("filename error\n");
		return 0;
	}
	remove_endspace(filename);
	FILE *file = fopen(filename, "w");
	if (file == NULL) {
		free(filename);
		return 1;
	}
	print_dot(tree->root, file);
	fclose(file);
	free(filename);
	return 0;
}

int dialog_load_fast_find(Tree *tree) {
	scanf("%*c");
        char *filename = readline("Input filename ");
        if (filename == NULL) {
                printf("filename error\n");
                return 0;
        }
        remove_endspace(filename);
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
                free(filename);
                return 1;
        }
	fclose(file);
	char *word = NULL;
	int check = get_key(&word);
	if (check == EOF) {
		free(filename);
		return EOF;
	}
	size_t *arr = NULL;
	int len = 0;
	free_nodes(tree->root);
	tree->root = NULL;
	check = fast_find(tree, word, filename, &arr, &len);
	for (size_t i = 0; i < len; i++) {
		printf("%zu ", arr[i]);
	}
	if (arr) {
		free(arr);
	}
	printf("\n");
	free(word);
	free(filename);
}

int dialog_fast_find(Tree *tree) {
	char *word = NULL;
	int check = get_key(&word);
	if (check == EOF) {
		return EOF;
	}
	size_t *arr = NULL;
	int len = 0;
	check = fast_find(tree, word, NULL, &arr, &len);
	for (size_t i = 0; i < len; i++) {
		printf("%zu ", arr[i]);
	}
	if (arr) {
		free(arr);
	}
	printf("\n");
	free(word);
}
