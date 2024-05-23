#include <assert.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Node {
	int val;
	struct Node* left;
	struct Node* right;

	int traversed;
} TNode;

TNode *alloc_node(int value) {
	TNode *node = malloc(sizeof(TNode));

	assert(node != NULL && "Buy more RAM lol");

	node->val = value;
	node->traversed = 0;
	node->left = NULL;
	node->right = NULL;

	return node;
}

void free_node(TNode **node) {
	if (*node == NULL) {
		return;
	}

	TNode *root = *node;

	free_node(&root->left);
	free_node(&root->right);
	free(root);

	// root is now freed
	*node = NULL;
}

void traverseBBLR_recursive(TNode *root) {
	if (root == NULL) {
		return;
	}

	traverseBBLR_recursive(root->left);
	printf("value: %d\n", root->val);
	traverseBBLR_recursive(root->right);
}

void traverseBBLR_imperative(TNode *root, int depth) {
	if (root == NULL) {
		return;
	}

	int node_count = (int)powf(2, depth) - 1;
	TNode *stack[node_count];
	int stack_top = 0;

	while (root != NULL || stack_top != 0) {
		if (root != NULL) {
			// push and go left
			stack[stack_top] = root;
			++stack_top;
			root = root->left;
		} else {
			// pop, print and go right
			--stack_top;
			root = stack[stack_top];

			printf("value: %d\n", root->val);

			root = root->right;
		}
	}
}


/* This function takes an empty pointer and fills it with a tree of level N */
TNode *fillTree(int N) {
	if (N == 0) {
		return NULL;
	}

	TNode *node = alloc_node(rand() % 100);
	node->left = fillTree(N - 1);
	node->right = fillTree(N - 1);

	return node;
}

int treeDepth(TNode *root) {
	if (root == NULL) {
		return 0;
	}

	// adding 1 to take the current depth into account
	int leftDepth = treeDepth(root->left) + 1;
	int rightDepth = treeDepth(root->right) + 1;

	if (leftDepth > rightDepth) {
		return leftDepth;
	}

	return rightDepth;
}

int main(void) {

	// initialize the randomization process
	srand(time(NULL));

	int depth = 5;

	// generate a tree with 5 levels of depth
	TNode *root = fillTree(depth);

	printf("The calculated depth of the tree is: %d\n", treeDepth(root));
	printf("Recursive:\n");
	traverseBBLR_recursive(root);
	printf("------------------------------\n");
	printf("Imperative:\n");
	traverseBBLR_imperative(root, depth);

	free_node(&root);

	return 0;
}
