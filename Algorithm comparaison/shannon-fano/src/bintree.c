#include "main.h"

void
clear(tree *root)
{
	if (root->left != NULL)
		clear(root->left);
	if (root->right != NULL)
		clear(root->right);
	
	free(root);
}

int
gototree(char c, tree **t)
{
	if (c == '0')
		(*t) = (*t)->left;
	else
		(*t) = (*t)->right;
	return (*t)->data;
}

tree*
addelement(int v)
{
	tree *node = (tree*) malloc(sizeof(tree));
	node->right = node->left = NULL;
	node->data = v;
	return node;
} 
