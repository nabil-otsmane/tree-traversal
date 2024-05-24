#include <assert.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

typedef struct {
	TNode **stack;
	int capacity;
	int position;
} LinearStack;

LinearStack *createStack(int capacity) {
	LinearStack *stack = malloc(sizeof(LinearStack));

	stack->stack = malloc(sizeof(TNode*) * capacity);
	stack->capacity = capacity;
	stack->position = 0;
	
	return stack;
}

bool push(LinearStack *stack, TNode *node) {
	if (stack == NULL || stack->position >= stack->capacity - 1) {
		fprintf(stderr, "Could not push node '%d' to stack\n", node->val);
		return false;
	}
	
	++stack->position;

	stack->stack[stack->position] = node;

	return true;
}

TNode *pop(LinearStack *stack) {
	if (stack == NULL || stack->position == 0) {
		fprintf(stderr, "Could not pop from stack\n");
		return NULL;
	}

	TNode *node = stack->stack[stack->position];
	--stack->position;

	return node;
}

bool isEmpty(LinearStack *stack) {
	if (stack == NULL || stack->position == 0) {
		return true;
	}

	return false;
}

void traverseBBLR_recursive(TNode *root) {
	if (root == NULL) {
		return;
	}

	traverseBBLR_recursive(root->left);
	printf("%d -> ", root->val);
	traverseBBLR_recursive(root->right);
}

void traverseBBLR_imperative(TNode *root, int depth) {
	if (root == NULL) {
		return;
	}

	int node_count = (int)powf(2, depth) - 1;
	LinearStack *stack = createStack(node_count);

	while (root != NULL || !isEmpty(stack)) {
		if (root != NULL) {
			// push and go left
			if (!push(stack, root)) {
				fprintf(stderr, "Unable to push to the stack.");
				return;
			}
			root = root->left;
		} else {
			// pop, print and go right
			root = pop(stack);

			if (root == NULL) {
				fprintf(stderr, "Unable to pop from the stack.");
				return;
			}

			printf("%d -> ", root->val);

			root = root->right;
		}
	}
}

void traverseLLLR_recursive(TNode *root) {
	if (root == NULL) {
		return;
	}

	traverseLLLR_recursive(root->left);
	traverseLLLR_recursive(root->right);
	printf("%d -> ", root->val);
}

void traverseLLLR_imperative(TNode *root, int depth) {
	if (root == NULL) {
		return;
	}

	int node_count = (int)powf(2, depth) - 1;
	LinearStack *stack = createStack(node_count);

	while (root != NULL || !isEmpty(stack)) {
		if (root != NULL) {
			// push and go left
			if (!push(stack, root)) {
				fprintf(stderr, "Unable to push to the stack.");
				return;
			}
			root = root->left;
		} else {
			// pop
			root = pop(stack);

			if (root == NULL) {
				fprintf(stderr, "Unable to pop from the stack.");
				return;
			}

			// if traversed, then we already went right
			if (root->traversed == 1) {
				printf("%d -> ", root->val);
				root->traversed = 0;
				root = NULL;
			} else {
				if (!push(stack, root)) {
					fprintf(stderr, "Unable to push to the stack.");
					return;
				}
				root->traversed++;
				root = root->right;
			}
		}
	}
}

void traverseBBRL_recursive(TNode *root) {
	if (root == NULL) {
		return;
	}

	traverseBBRL_recursive(root->right);
	printf("%d -> ", root->val);
	traverseBBRL_recursive(root->left);
}

void traverseBBRL_imperative(TNode *root, int depth) {
	if (root == NULL) {
		return;
	}

	int node_count = (int)powf(2, depth) - 1;
	LinearStack *stack = createStack(node_count);

	while (root != NULL || !isEmpty(stack)) {
		if (root != NULL) {
			// push and go right
			if (!push(stack, root)) {
				fprintf(stderr, "Unable to push to the stack.");
				return;
			}
			root = root->right;
		} else {
			// pop, print, and go left
			root = pop(stack);

			if (root == NULL) {
				fprintf(stderr, "Unable to pop from the stack.");
				return;
			}

			printf("%d -> ", root->val);
			
			root = root->left;
		}
	}
}

void traverseLLRL_recursive(TNode *root) {
	if (root == NULL) {
		return;
	}

	traverseLLRL_recursive(root->right);
	traverseLLRL_recursive(root->left);
	printf("%d -> ", root->val);
}

void traverseLLRL_imperative(TNode *root, int depth) {
	if (root == NULL) {
		return;
	}

	int node_count = (int)powf(2, depth) - 1;
	LinearStack *stack = createStack(node_count);

	while (root != NULL || !isEmpty(stack)) {
		if (root != NULL) {
			// push and go right
			if (!push(stack, root)) {
				fprintf(stderr, "Unable to push to the stack.");
				return;
			}
			root = root->right;
		} else {
			// pop
			root = pop(stack);

			if (root == NULL) {
				fprintf(stderr, "Unable to pop from the stack.");
				return;
			}

			// if traversed, then we already went left
			if (root->traversed == 1) {
				printf("%d -> ", root->val);
				root->traversed = 0;
				root = NULL;
			} else {
				if (!push(stack, root)) {
					fprintf(stderr, "Unable to push to the stack.");
					return;
				}
				root->traversed++;
				root = root->left;
			}
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

typedef struct {
	// first in layer
	bool fil;
	// last in layer
	bool lil;

	int layer;
	int depth;
	int dataWidth;
} PrintMetadata;

int nodeCount(int depth) {
	if (depth <= 0) {
		return 0;
	}

	return (int)powf(2, depth) - 1;
}

void printNode(TNode *node, PrintMetadata m) {
	int space_between = m.layer == 0
		? 0
		: nodeCount(m.depth - m.layer) * m.dataWidth;
	int space_before = nodeCount(m.depth - m.layer - 1) * m.dataWidth;
	int dash_count = nodeCount(m.depth - m.layer - 2) * m.dataWidth;
	int max_width = nodeCount(m.depth - 1) * m.dataWidth;

	char space_txt[max_width];

	if (m.fil) {
		memset(space_txt, ' ', space_before - dash_count);
		if (dash_count > 0) {
			memset(space_txt + space_before - dash_count, '_', dash_count);
		}
		space_txt[space_before] = '\0';
		printf("%s", space_txt);
	} else {
		memset(space_txt, ' ', space_between - dash_count * 2);
		if (dash_count > 0) {
			memset(space_txt + space_between - dash_count * 2, '_', dash_count);
		}
		space_txt[space_between - dash_count] = '\0';
		printf("%s", space_txt);
	}
	printf("%02d", node->val);

	if (dash_count > 0) {
		memset(space_txt, '_', dash_count);
		space_txt[dash_count] = '\0';
		printf("%s", space_txt);
	}

	if (m.lil) {
		printf("\n");
	} else {
		// skip the next code if not lil
		return;
	}

	if (node->left == NULL && node->right == NULL) {
		return;
	}

	int space_between_after = nodeCount(m.depth - m.layer - 1) * m.dataWidth;

	// first one is different
	// printf("node=%p, space_before=%d, dash_count=%d, space_between=%d\n", node, space_before_after, dash_count_after, space_between);
	memset(space_txt, ' ', space_before - dash_count - 1);
	space_txt[space_before - dash_count - 1] = '/'; 
	space_txt[space_before - dash_count] = '\0';
	printf("%s", space_txt);

	bool closed = true;
	for (int i = 0; i < nodeCount(m.layer+1); i++) {
		int margins = closed ? 0: 2;
		memset(space_txt, ' ', space_between_after + margins);
		space_txt[space_between_after + margins] = closed? '\\': '/';
		space_txt[space_between_after + margins + 1] = '\0';
		printf("%s", space_txt);

		closed = !closed;
	}
	printf("\n");
}

void prettyPrint(TNode *root) {
	int depth = treeDepth(root);
	int node_count = nodeCount(depth);
	TNode *node_queue[node_count] = {};
	PrintMetadata metadata_queue[node_count] = {};
	int queue_last = 0;
	int queue_first = 0;

	node_queue[queue_last] = root;
	metadata_queue[queue_last] = (PrintMetadata) {
		.fil = true,
		.lil = true,

		.layer = 0,
		.depth = depth,
		.dataWidth = 2,
	};
	++queue_last;
	while (queue_first < queue_last) {
		// dequeue
		root = node_queue[queue_first];
		PrintMetadata metadata = metadata_queue[queue_first];
		++queue_first;

		printNode(root, metadata);

		if (root->left != NULL) {
			node_queue[queue_last] = root->left;
			metadata_queue[queue_last] = (PrintMetadata) {
				.fil = metadata.fil,
				.lil = false,
				.layer = metadata.layer + 1,
				.depth = depth,
				.dataWidth = metadata.dataWidth,
			};
			++queue_last;
		}
		if (root->right != NULL) {
			node_queue[queue_last] = root->right;
			metadata_queue[queue_last] = (PrintMetadata) {
				.fil = false,
				.lil = metadata.lil,
				.layer = metadata.layer + 1,
				.depth = depth,
				.dataWidth = metadata.dataWidth,
			};
			++queue_last;
		}
	}
}

int main(void) {

	// initialize the randomization process
	srand(time(NULL));

	int depth = 5;

	// generate a tree with 5 levels of depth
	TNode *root = fillTree(depth);

	printf("The calculated depth of the tree is: %d\n", treeDepth(root));
	prettyPrint(root);
	printf("\n\nBrand by branch, Bottom to top, Left to Right (BBLR):\n");
	printf("Recursive:\n");
	traverseBBLR_recursive(root);
	printf("\n");
	printf("------------------------------\n");
	printf("Imperative:\n");
	traverseBBLR_imperative(root, depth);
	printf("\n");

	printf("\n\nLeaf by Leaf, Left to Right (LLLR):\n");
	printf("Recursive:\n");
	traverseLLLR_recursive(root);
	printf("\n");
	printf("------------------------------\n");
	printf("Imperative:\n");
	traverseLLLR_imperative(root, depth);
	printf("\n");

	printf("\n\nBranch by branch, Bottom to top, Right to Left (BBRL):\n");
	printf("Recursive:\n");
	traverseBBRL_recursive(root);
	printf("\n");
	printf("------------------------------\n");
	printf("Imperative:\n");
	traverseBBRL_imperative(root, depth);
	printf("\n");

	printf("\n\nLeaf by Leaf, Right to Left (LLRL):\n");
	printf("Recursive:\n");
	traverseLLRL_recursive(root);
	printf("\n");
	printf("------------------------------\n");
	printf("Imperative:\n");
	traverseLLRL_imperative(root, depth);
	printf("\n");

	free_node(&root);

	return 0;
}
