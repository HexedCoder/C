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

tree *bst_minimum(tree *tree) {
    while (tree->left)
        tree = tree->left;

    return tree;
}

tree *bst_maximum(tree *tree) {
    while (tree->right) {
        tree = tree->right;
    }

    return tree;
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
    tree *node = search(*root, val);
    if (!node) {
        return NULL;
    }

    // Handle root node
    if (*root == node) {
        if (!node->left && !node->right) {
            free(*root);
            *root = NULL;
            return *root;
        }
        if (!node->left || !node->right) {
            tree *temp = node->left ? node->left : node->right;
            temp->parent = NULL;
            free(*root);
            *root = temp;
            return *root;
        }
    }

    // Handle if inner node has no right for minimum
    if (!node->right) {
        if (node->parent->data < node->data) {
            node->parent->right = node->left;
        } else {
            node->parent->left = node->left;
        }
        if (node->left) {
            node->left->parent = node->parent;
        }
        free(node);
        return *root;
    }

    // Standard handling
    tree *min = bst_minimum(node->right);
    node->data ^= min->data;
    min->data ^= node->data;
    node->data ^= min->data;
    if (min == node->right) {
        node->right = min->right;
        if (min->right) {
            min->right->parent = node;
        }
    } else {
        min->parent->left = min->right;
        if (min->right) {
            min->right->parent = min->parent;
        }
    }
    free(min);
    return *root;
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