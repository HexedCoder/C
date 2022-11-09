// ------------------------------------------------------------------------------
//Lesson 6 Exercise:binary search tree
// ------------------------------------------------------------------------------
#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

struct tree {
    struct tree *parent;
    struct tree *left;
    struct tree *right;
    int data;
};
typedef struct tree tree;

tree *create_node(int);

void preorder(tree *root);

void postorder(tree *root);

void inorder(tree *root);

void levelorder(tree *root);

tree *bst_insert(tree *root, tree *node);

tree *search(tree *, int);

tree *bst_minimum(tree *tree);

tree *bst_maximum(tree *tree);

int tree_size(tree *tree);

void print(tree *);

tree *delete_node(tree **, int);

void tree_delete(tree **p_tree);

#endif
