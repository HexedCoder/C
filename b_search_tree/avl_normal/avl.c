#include "avl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tree* create_node(int d)
{
    tree* t = (tree*)malloc(sizeof(tree));
    t->parent = 0;
    t->left = 0;
    t->right = 0;
    t->data = d;
    t->height = 0;
    return t;
}

tree* insert(tree* t, tree* i)
{
    if (i->data < t->data)
    {
        if (t->left == 0)
        {
            t->left = i;
            i->parent = t;
            return i;
        }
    }
    else
    {
        if (t->right == 0)
        {
            t->right = i;
            i->parent = t;
            return i;
        }
    }
    tree* n = (i->data < t->data) ? t->left : t->right;
    return insert(n, i);
}

void avl_insert(tree* t, tree* i)
{
    // Regular binary search tree insertion.
    tree* n = insert(t, i);
    n->height = 1;
    int bf = 0;

    // Update heights and test for AVL violation.
    while (n->parent != 0 && bf <= 1)
    {
        n = n->parent;
        int lh = (n->left) ? n->left->height : 0;
        int rh = (n->right) ? n->right->height : 0;
        int max = (rh > lh) ? rh : lh;
        bf = abs(rh - lh);
        n->height = max+1;
    }
    if (n->parent == 0 || bf <= 1) return;

    // Automatic correction of AVL violation (self-balance).
    if (i->data < n->data)
    {
        // Determine if grandchild of first imbalanced node is LL or LR.
        if (i->data < n->left->data)
        {
            // LL
            n = rotate_right(&t, n->left);
            n->right->height = n->height - 1;
        }
        else
        {
            // LR
            n = rotate_right(&t, rotate_left(&t, n->left->right));
            if (n != 0)
            {
                n->left->height = n->height;
                n->right->height = n->height;
                ++(n->height);
            }
        }
    }
    else
    {
        // Determine if grandchild of first imbalanced node is RR or RL.
        if (i->data > n->right->data)
        {
            // RR
            n = rotate_left(&t, n->right);
            n->left->height = n->height - 1;
        }
        else
        {
            // RL
            n = rotate_left(&t, rotate_right(&t, n->right->left));
            if (n != 0)
            {
                n->left->height = n->height;
                n->right->height = n->height;
                ++(n->height);
            }
        }
    }
}

tree* search(tree* t, int d)
{
    if (t == 0 || t->data == d)
    {
        return t;
    }
    tree* n = (d < t->data) ? t->left : t->right;
    return search(n, d);
}

tree* minimum(tree* t)
{
    // Minimum must be in the leftmost path from root to leaf.
    if (t->left == 0)
    {
        return t;
    }
    return minimum(t->left);
}

tree* maximum(tree* t)
{
    // Maximum must be in the rightmost path from root to leaf.
    if (t->right == 0)
    {
        return t;
    }
    return maximum(t->right);
}

tree* rotate_right(tree** t, tree* n)
{
    // Return if node is null or the root, or is not a left child.
    if (n == 0 || n == *t || n->data > n->parent->data) return n;
    tree* p = n->parent;
    tree* pp = p->parent;
    tree* c = n->right;
    n->parent = pp;
    n->right = p;
    p->parent = n;
    p->left = c;
    if (c != 0)
    {
        c->parent = p;
    }
    if (pp != 0)
    {
        if (p->data < pp->data)
        {
            pp->left = n;
        }
        else
        {
            pp->right = n;
        }
    }
    else
    {
        // Parent of rotated node is the root; rotated node becomes the new root.
        *t = n;
    }
    return n;
}

tree* rotate_left(tree** t, tree* n)
{
    // Return if node is null or the root, or is not a right child.
    if (n == 0 || n == *t || n->data < n->parent->data) return n;
    tree* p = n->parent;
    tree* pp = p->parent;
    tree* c = n->left;
    n->parent = pp;
    n->left = p;
    p->parent = n;
    p->right = c;
    if (c != 0)
    {
        c->parent = p;
    }
    if (pp != 0)
    {
        if (p->data < pp->data)
        {
            pp->left = n;
        }
        else
        {
            pp->right = n;
        }
    }
    else
    {
        // Parent of rotated node is the root; rotated node becomes the new root.
        *t = n;
    }
    return n;
}

int size(tree* t)
{
    int s = 1;
    if (t == 0)
    {
        return 0;
    }
    s += size(t->left);
    s += size(t->right);
    return s;
}

void print(tree* t)
{
    // Pre-order traversal.
    if (t != 0)
    {
        int i = 0;
        int j = 0;
        if (t->left)
        {
            i = t->left->data;
        }
        if (t->right)
        {
            j = t->right->data;
        }
        printf("parent = %d, left = %d, right = %d\n", t->data, i, j);
        print(t->left);
        print(t->right);
    }
}

tree* delete_node(tree** t, int d)
{
    // Return parent of deleted node except:
    //
    // 1) if deleted node does not exist then return null
    // 2) if deleted node is the root then return new root
    tree* node = search(*t, d);
    tree* tmp = 0;
    if (node == 0) return *t;

    // Root node.
    //
    // Treat special because root has no parent to relink.
    //
    // Only need special handling for:
    //
    //  i) no children
    // ii) one child
    if (node->parent == 0)
    {
        if (node->left == 0 && node->right == 0)
        {
            free(node);
            *t = 0;
            return *t;
        }
        if (node->left == 0 || node->right == 0)
        {
            tmp = (node->left == 0) ? node->right : node->left;
            tmp->parent = 0;
            free(node);
            *t = tmp;
            return *t;
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
    tree* x = node;
    tmp = node->left;
    if (node->right != 0)
    {
        node = minimum(node->right);
        tmp = node->right;
    }
    if (node->data > node->parent->data)
    {
        // Right child.
        node->parent->right = tmp;
    }
    else
    {
        // Left child.
        node->parent->left = tmp;
    }
    if (tmp != 0)
    {
        tmp->parent = node->parent;
    }
    x->data = node->data;
    tmp = node->parent;
    free(node);
    return tmp;
}

void delete(tree** t)
{
    if (*t == 0)
    {
        return;
    }
    tree* l = (*t)->left;
    tree* r = (*t)->right;
    free(*t);
    *t = (0);
    delete(&l);
    delete(&r);
}

void preorder(tree* t, void (*fn)(tree*))
{
    if (t == 0) return;
    fn(t);
    preorder(t->left, fn);
    preorder(t->right, fn);
}

void postorder(tree* t, void (*fn)(tree*))
{
    if (t == 0) return;
    postorder(t->left, fn);
    postorder(t->right, fn);
    fn(t);
}

void inorder(tree* t, void (*fn)(tree*))
{
    if (t == 0) return;
    inorder(t->left, fn);
    fn(t);
    inorder(t->right, fn);
}

void levelorder(tree* t, void(*fn)(tree*))
{
    if (t == 0) return;

    // Treat a simple array as a queue by incrementing front and back indices.
    //
    // Increase size dynamically but discard items that have already been
    // processed.
    int buf = 1<<10;
    tree** a = (tree**)malloc(buf*sizeof(tree*));
    a[0] = t;
    int f = 0;
    int b = 0;
    do
    {
        tree* node = a[f];
        fn(node);
        if (node->left != 0)
        {
            a[++b] = node->left;
        }
        if (node->right != 0)
        {
            a[++b] = node->right;
        }
        ++f;
        if (buf <= b+2)
        {
            // Allocate new memory, copy, and then free, keeping only items that have
            // not been processed. Reset front and back indices accordingly.
            //
            // NOTE: Could use realloc but it retains all data and might allocate new
            // memory into which it copies the contents of the old memory before
            // releasing the old memory, returning a new memory address.
            buf += b-f+1;
            tree** tmp = (tree**)malloc(buf*sizeof(tree*));
            memcpy(&tmp[0], &a[f], (b-f+1)*sizeof(tree*));
            free(a);
            a = tmp;
            b = b-f;
            f = 0;
        }
    } while (f <= b);
    free(a);
}