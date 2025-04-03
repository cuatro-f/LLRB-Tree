#include <stdio.h>
#include <string.h>
#include "visualization.h"
#include "../tree/tree.h"

/*void print_dot_null(char *key, int nullcount, FILE* file)
{   	fprintf(file, "    null%d [shape=point;color=white;style=invise];\n", nullcount);
    	fprintf(file, "    %s -> null%d [color=white];\n", key, nullcount);
}*/

void print_dot_aux(Node* node, FILE* file)
{
    	static int nullcount = 0;
	char *color = node->color == red ? "red" : "black";
	if (strcmp(node->key, "elist") == 0) {
		return;
	}
	fprintf(file, "    %s [color=%s;style=invise];\n", node->key, color);
    	if (node->left) {
		if (strcmp(node->left->key, "elist") != 0) {
        		fprintf(file, "    %s -> %s;\n", node->key, node->left->key);
		}
		else {
			fprintf(file, "    %s;\n", node->key);
		}
        	print_dot_aux(node->left, file);
	}
    	if (node->right){
		if (strcmp(node->right->key, "elist") != 0) {
        		fprintf(file, "    %s -> %s;\n", node->key, node->right->key);
		}
		else {
			fprintf(file, "    %s;\n", node->key);
		}
        	print_dot_aux(node->right, file);
	}
}

void print_dot(Node* tree, FILE* file)
{
    	fprintf(file, "digraph BST {\n");
    	fprintf(file, "    node [fontname=\"Arial\"];\n");
    	if (tree == NULL) {
        	fprintf(file, "\n");
	}
    	else if (!tree->right && !tree->left) {
        	fprintf(file, "    %s;\n", tree->key);
	}
    	else {
        	print_dot_aux(tree, file);
	}
    	fprintf(file, "}\n");
}

