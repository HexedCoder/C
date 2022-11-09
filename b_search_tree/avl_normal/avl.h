#ifndef AVL_TREE_H
#define AVL_TREE_H

struct tree {
    struct tree *parent;
    struct tree *left;
    struct tree *right;
    int data;
    int height;
};
typedef struct tree tree;

tree *create_node(int);

tree *insert(tree *, tree *);

void avl_insert(tree *, tree *);

tree *search(tree *, int);

tree *minimum(tree *);

tree *maximum(tree *);

tree *rotate_right(tree **, tree *);

tree *rotate_left(tree **, tree *);

int size(tree *);

void print(tree *);

tree *delete_node(tree **, int); // Regular BST deletion (not AVL)

void delete(tree **);

void preorder(tree *, void (*)(tree *));

void postorder(tree *, void (*)(tree *));

void inorder(tree *, void (*)(tree *));

void levelorder(tree *, void (*)(tree *));

#endif