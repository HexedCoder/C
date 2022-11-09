#include <stdlib.h>
#include <stdio.h>

#include "binary_search_tree.h"
#include "llist.h"

typedef struct node_t {
    struct node_t *next;
    void *data;
} node_t;


tree *create_node(int value) {
    tree *tmp = NULL;

    tmp = calloc(1, sizeof(*tmp));
    if (tmp) {
        tmp->left = NULL;
        tmp->right = NULL;
        tmp->data = value;
    }

    return tmp;
} /* create_node() */

tree *bst_insert(tree *root, tree *node) {
    if (!root || !node) {
        return NULL;
    }

    while (root) {

        if (node->data < root->data) {
            if (!root->left) {
                root->left = node;
                node->parent = root;
                break;
            } else {
                root = root->left;
            }
        } else if (node->data > root->data) {
            if (!root->right) {
                root->right = node;
                node->parent = root;
                break;
            } else {
                root = root->right;
            }
        } else {
            break;
        }
    }

    return root;
}

tree *search(tree *root, int value) {

    if (!root) {
        return NULL;
    }

    if (value < root->data) {
        return search(root->left, value);
    } else if (root->data == value) {
        return root;
    } else {
        return search(root->right, value);
    }

}

void preorder(tree *root) {

    if (!root) {
        return;
    }

    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void postorder(tree *root) {

    if (!root) {
        return;
    }

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

void inorder(tree *root) {

    if (!root) {
        return;
    }

    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void levelorder(tree *root) {

    if (!root) {
        return;
    }

    llist_t *queue = llist_create();
    tree *curr = root;

    while (curr) {

        printf("%d ", curr->data);

        if (curr->left) {
            llist_enqueue(queue, curr->left);
        }
        if (curr->right) {
            llist_enqueue(queue, curr->right);
        }

        curr = llist_dequeue(queue);
    }
    llist_delete(queue, NULL);
}

int bst_minimum(tree *tree) {
    while (tree->left)
        tree = tree->left;

    return tree->data;
}

tree *bst_minimum_node(tree *tree) {
    while (tree->left)
        tree = tree->left;

    return tree;
}

tree *bst_maximum_node(tree *tree) {
    while (tree->right)
        tree = tree->right;

    return tree;
}

int bst_maximum(tree *tree) {
    while (tree->right) {
        tree = tree->right;
    }

    return tree->data;
}

int tree_size(tree *tree) {
    if (!tree) {
        return 0;
    }
    int count = tree_size(tree->left);
    count += tree_size(tree->right);

    return 1 + count;
}

void print(tree *root) {
    if (!root) {
        return;
    }

    print(root->left);
    printf("%d ", root->data);
    print(root->right);
}

tree *delete_node(tree **root, int val) {
// Return parent of deleted node except:
    //
    // 1. if deleted node does not exist then return null
    // 2. if deleted node is the root then return new root
    tree *node = search(*root, val);
    tree *tmp = 0;
    if (node == 0) return *root;

    // Root node.
    //
    // Treat special because root has no parent to relink.
    //
    // Only need special handling for:
    //
    // i. no children
    // ii. one child
    if (node->parent == 0) {
        if (node->left == 0 && node->right == 0) {
            free(node);
            *root = 0;
            return *root;
        }
        if (node->left == 0 || node->right == 0) {
            tmp = (node->left == 0) ? node->right : node->left;
            tmp->parent = 0;
            free(node);
            *root = tmp;
            return *root;
        }
    }

    // Replace deleted node with the minimum of its right branch.
    //
    // If missing the right branch then replace with the left branch, treating
    // leaf nodes in the same manner.
    //
    // The minimum node cannot have a left child.
    //
    // But the minimum node can be the right child of the deleted node so replace
    // deleted node's data with the minimum after testing if the minimum is its
    // right child.
    //
    // NOTE: If a leaf or missing right branch, then saving the node and copying
    // its data is extraneous but minimal work overall.
    tree *x = node;
    tmp = node->left;
    if (node->right != 0) {
        node = bst_minimum_node(node->right);
        tmp = node->right;
    }
    if (node->data > node->parent->data) {
        // Right child.
        node->parent->right = tmp;
    } else {
        // Left child.
        node->parent->left = tmp;
    }
    if (tmp != 0) {
        tmp->parent = node->parent;
    }
    x->data = node->data;
    tmp = node->parent;
    free(node);
    return tmp;
}

void tree_delete(tree **p_tree) {
    if (!p_tree || !*p_tree) {
        return;
    }

    tree_delete(&(*p_tree)->left);
    tree_delete(&(*p_tree)->right);
    free(*p_tree);
    *p_tree = NULL;
}

struct trunk {
    struct trunk *prev;
    const char *str;
};