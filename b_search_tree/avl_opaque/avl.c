#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

typedef struct node_t
{
    struct node_t *parent;
    struct node_t *left;
    struct node_t *right;
    int            height;
    void          *data;
} node_t;

struct avl_t
{
    struct node_t  *root;
    compare         compare_func;
    action          action_func;
    pthread_mutex_t mutex;
};

/**
 * Allocates space for node and assigned data to it
 *
 * @param void *: Data to store
 *
 * @return The new root node after rotation, or NULL on error
 */
static node_t *create_node(void *);

/**
 * Performs a left rotation on the given node in the AVL tree.
 *
 * @param avl_t **: Pointer to the pointer of the AVL tree.
 * @param node_t *: The node to be rotated.
 *
 * @return The new root node after rotation.
 */
static node_t *rotate_left(avl_t **, node_t *);

/**
 * Performs a right rotation on the given node in the AVL tree.
 *
 * @param avl_t **: Pointer to the pointer of the AVL tree.
 * @param node_t *: The node to be rotated.
 *
 * @return The new root node after rotation.
 */
static node_t *rotate_right(avl_t **, node_t *);

/**
 * Inserts a new node with the given data into the AVL tree.
 *
 * @param node_t *: The current node in the AVL tree.
 * @param void *: The data to be inserted.
 * @param compare: Pointer to the comparison function.
 *
 * @return Pointer to the inserted node on success, NULL on failure.
 */
static node_t *insert(node_t *, void *, compare);

/**
 * Searches for a node with the given value in the AVL tree.
 *
 * @param node_t *: The current node in the AVL tree.
 * @param void *: The data to search for.
 * @param compare: Pointer to the comparison function.
 *
 * @return Pointer to the node containing the value on success, NULL on failure.
 */
static void *find(node_t *, void *, compare);

/**
 * Recursively deletes the nodes in the AVL tree and frees the associated
 * memory.
 *
 * @param node_t **: Pointer to the pointer of the current node.
 * @param destroy: Pointer to the user's destroy.
 */
static void delete(node_t **, destroy);

/**
 * Returns the minimum data value stored in the given subtree.
 *
 * @param node_t *: The root node of the subtree.
 * @return Pointer to the minimum data value on success, NULL on failure.
 */

static int get_t_size(node_t *);

avl_t *
avl_tree_create(compare compare_func, action action_func)
{
    avl_t *tree = calloc(1, sizeof(*tree));

    if (NULL == tree)
    {
        goto EXIT_AVL_CREATE;
    }

    tree->compare_func = compare_func;
    tree->action_func  = action_func;

    // Initialize the mutex for thread safety
    if (pthread_mutex_init(&tree->mutex, NULL) != 0)
    {
        // Handle mutex initialization failure
        free(tree);
        tree = NULL;
    }

EXIT_AVL_CREATE:
    return tree;

} /* avl_tree_create() */

static node_t *
create_node(void *data)
{
    node_t *node = NULL;

    if (NULL != data)
    {

        node = calloc(1, sizeof(*node));
        if (node != NULL)
        {
            node->data = data;
        }
    }

    return node;
} /* create_node() */

static node_t *
rotate_left(avl_t **pp_root_tree, node_t *p_new_node)
{

    if (!p_new_node || !pp_root_tree || !*pp_root_tree)
    {
        return p_new_node;
    }

    avl_t   *p_root_tree  = *pp_root_tree;
    node_t **pp_root_node = &p_root_tree->root;
    node_t  *p_parent     = p_new_node->parent;
    int      cmp_result
        = p_root_tree->compare_func(p_new_node->parent->data, p_new_node->data);

    if (p_new_node == *pp_root_node || 0 < cmp_result)
    {
        return p_new_node;
    }

    node_t *p_grand_parent = p_parent->parent;
    node_t *p_child        = p_new_node->left;

    p_new_node->parent = p_grand_parent;
    p_new_node->left   = p_parent;
    p_parent->parent   = p_new_node;
    p_parent->right    = p_child;

    if (p_child)
    {
        p_child->parent = p_parent;
    }

    if (p_grand_parent)
    {
        if (0 > p_root_tree->compare_func(p_parent->data, p_grand_parent->data))
        {
            p_grand_parent->left = p_new_node;
        }
        else
        {
            p_grand_parent->right = p_new_node;
        }
    }
    else
    {
        // rotated new_node becomes root
        p_root_tree->root = p_new_node;
    }

    return p_new_node;
} /* rotate_left() */

static node_t *
rotate_right(avl_t **pp_root_tree, node_t *p_new_node)
{

    if (!p_new_node || !pp_root_tree || !*pp_root_tree)
    {
        return p_new_node;
    }

    avl_t   *p_root_tree  = *pp_root_tree;
    node_t **pp_root_node = &p_root_tree->root;
    node_t  *p_parent     = p_new_node->parent;
    int      cmp_result
        = p_root_tree->compare_func(p_new_node->parent->data, p_new_node->data);

    if (p_new_node == *pp_root_node || 0 > cmp_result)
    {
        return p_new_node;
    }

    node_t *p_grand_parent = p_parent->parent;
    node_t *p_child        = p_new_node->right;

    p_new_node->parent = p_grand_parent;
    p_new_node->right  = p_parent;
    p_parent->parent   = p_new_node;
    p_parent->left     = p_child;

    if (p_child)
    {
        p_child->parent = p_parent;
    }
    if (p_grand_parent)
    {
        if (p_root_tree->compare_func(p_parent->data, p_grand_parent->data) < 0)
        {
            p_grand_parent->left = p_new_node;
        }
        else
        {
            p_grand_parent->right = p_new_node;
        }
    }
    else
    {
        // rotated new_node becomes root
        p_root_tree->root = p_new_node;
    }

    return p_new_node;
} /* rotate_right() */

static node_t *
insert(node_t *p_node, void *p_data, compare p_compare_func)
{
    int result = p_compare_func(p_data, p_node->data);

    if (0 == result)
    {
        fprintf(stderr, "Data exists\n");
        return NULL;
    }

    node_t *p_tmp_node = NULL;

    if (0 > result)
    {
        if (p_node->left)
        {
            return insert(p_node->left, p_data, p_compare_func);
        }
        else
        {
            p_tmp_node = create_node(p_data);
            if (p_tmp_node)
            {
                p_tmp_node->parent = p_node;
                p_node->left       = p_tmp_node;
            }
        }
    }
    else
    {
        if (p_node->right)
        {
            return insert(p_node->right, p_data, p_compare_func);
        }
        else
        {
            p_tmp_node = create_node(p_data);
            if (p_tmp_node)
            {
                p_tmp_node->parent = p_node;
                p_node->right      = p_tmp_node;
            }
        }
    }

    return p_tmp_node;
} /* insert() */

void *
avl_insert(avl_t *p_tree, void *p_data)
{
    if (!p_tree || !p_data)
    {
        return NULL;
    }

    pthread_mutex_lock(&p_tree->mutex);
    node_t *p_new_node = NULL;
    if (NULL == p_tree->root)
    {
        p_new_node   = calloc(1, sizeof(*p_new_node));
        p_tree->root = p_new_node;
        if (p_new_node)
        {
            p_new_node->data = p_data;
        }
    }
    else
    {
        p_new_node = insert(p_tree->root, p_data, p_tree->compare_func);
    }

    if (NULL == p_new_node)
    {
        pthread_mutex_unlock(&p_tree->mutex);
        return NULL;
    }

    p_new_node->height = 1;
    int bf             = 0;

    while (p_new_node->parent && 2 > bf)
    {
        p_new_node         = p_new_node->parent;
        int l_bf           = p_new_node->left ? p_new_node->left->height : 0;
        int r_bf           = p_new_node->right ? p_new_node->right->height : 0;
        int max            = (l_bf < r_bf) ? r_bf : l_bf;
        bf                 = abs(r_bf - l_bf);
        p_new_node->height = max + 1;
    }

    // no balance needed
    if (!p_new_node->parent && 2 > bf)
    {
        pthread_mutex_unlock(&p_tree->mutex);
        return p_data;
    }

    if (0 > p_tree->compare_func(p_data, p_new_node->data))
    {
        if (0 > p_tree->compare_func(p_data, p_new_node->left->data))
        {
            p_new_node                = rotate_right(&p_tree, p_new_node->left);
            p_new_node->right->height = p_new_node->height - 1;
        }
        else
        {
            p_new_node = rotate_right(
                &p_tree, rotate_left(&p_tree, p_new_node->left->right));

            if (p_new_node)
            {
                p_new_node->left->height  = p_new_node->height;
                p_new_node->right->height = p_new_node->height;
                ++p_new_node->height;
            }
        }
    }
    else
    {
        if (0 < p_tree->compare_func(p_data, p_new_node->right->data))
        {
            p_new_node               = rotate_left(&p_tree, p_new_node->right);
            p_new_node->left->height = p_new_node->height - 1;
        }
        else
        {
            p_new_node = rotate_left(
                &p_tree, rotate_right(&p_tree, p_new_node->right->left));
            if (p_new_node)
            {
                p_new_node->left->height  = p_new_node->height;
                p_new_node->right->height = p_new_node->height;
                ++p_new_node->height;
            }
        }
    }
    pthread_mutex_unlock(&p_tree->mutex);

    return p_data;
} /* avl_insert() */

static void *
find(node_t *p_node, void *p_value, compare p_compare_func)
{
    if (!p_node)
    {
        return NULL;
    }

    int ret = p_compare_func(p_value, p_node->data);

    if (ret < 0)
    {
        return find(p_node->left, p_value, p_compare_func);
    }
    else if (ret > 0)
    {
        return find(p_node->right, p_value, p_compare_func);
    }
    else
    {
        return p_node;
    }

} /* find() */

void *
avl_tree_search(avl_t *p_tree, void *p_value)
{
    void *p_result = NULL;

    if (!p_tree || !p_value)
    {
        goto EXIT_AVL_TREE_SEARCH;
    }

    pthread_mutex_lock(&p_tree->mutex);
    p_result = find(p_tree->root, p_value, p_tree->compare_func);
    pthread_mutex_unlock(&p_tree->mutex);

EXIT_AVL_TREE_SEARCH:

    if (p_result)
    {
        node_t *tmp_node = (node_t *)p_result;
        p_result         = tmp_node->data;
    }
    return p_result;

} /* avl_tree_search() */

static void *
min_node(node_t *node)
{
    while (node->left)
    {
        node = node->left;
    }
    return node;
}

static int
avl_delete_node(node_t **root,
                node_t **target,
                compare  compare_func,
                destroy  destroy_func)
{
    if (!target || !root)
    {
        return 0;
    }

    node_t *node = *target;

    // Handle root node

    if (*root == *target)
    {
        if (!node->left && !node->right)
        {
            destroy_func((*root)->data);
            free(*root);
            *root = NULL;

            return 1;
        }

        if (!node->left || !node->right)
        {
            node_t *temp = node->left ? node->left : node->right;
            temp->parent = NULL;
            destroy_func((*target)->data);
            free(*target);
            *root = temp;
            return 1;
        }
    }

    // Handle if inner node has no right for minimum
    if (!node->right)
    {
        int ret = compare_func(node->parent->data, node->data);

        if (0 < ret)
        {
            node->parent->left = node->left;
        }
        else
        {
            node->parent->right = node->left;
        }
        if (node->left)
        {
            node->left->parent = node->parent;
        }
        destroy_func(node->data);
        free(node);
        return 1;
    }

    // Standard handling
    node_t *min = min_node(node->right);

    void *temp = min->data;
    min->data  = node->data;
    node->data = temp;

    if (min == node->right)
    {
        node->right = min->right;
        if (min->right)
        {
            min->right->parent = node;
        }
    }
    else
    {
        min->parent->left = min->right;
        if (min->right)
        {
            min->right->parent = min->parent;
        }
    }

    destroy_func(min->data);
    free(min);
    return 1;
}

int
avl_node_delete(avl_t **pp_tree, void *p_value, destroy destroy_func)
{
    if (!pp_tree || !*pp_tree)
    {
        return 0;
    }
    avl_t *p_tree = *pp_tree;

    if (p_tree->compare_func)
    {
        return 0;
    }

    pthread_mutex_lock(&p_tree->mutex);
    node_t *p_result = find(p_tree->root, p_value, p_tree->compare_func);
    pthread_mutex_unlock(&p_tree->mutex);

    if (!p_result)
    {
        printf("Search Value Not Found\n");
        return 0;
    }

    pthread_mutex_lock(&p_tree->mutex);
    int result = avl_delete_node(
        &p_tree->root, &p_result, p_tree->compare_func, destroy_func);
    pthread_mutex_unlock(&p_tree->mutex);

    return result;
}

int
avl_tree_size(avl_t *p_tree)
{
    if (NULL == p_tree)
    {
        return 0;
    }

    return get_t_size(p_tree->root);
} /* avl_tree_size() */

static int
get_t_size(node_t *p_node)
{
    if (NULL == p_node)
    {
        return 0;
    }

    int count = get_t_size(p_node->left);
    count += get_t_size(p_node->right);

    return 1 + count;
} /* get_t_size() */

static void delete(node_t **pp_node, destroy p_destroy_func)
{
    if (!pp_node || !*pp_node)
    {
        return;
    }

    node_t *p_node = *pp_node;
    delete (&p_node->left, p_destroy_func);
    delete (&p_node->right, p_destroy_func);

    if (NULL != p_destroy_func)
    {
        p_destroy_func(p_node->data);
    }

    free(p_node);
    *pp_node = NULL;

} /* delete() */

void
avl_tree_destroy(avl_t **pp_tree, destroy p_destroy_func)
{
    if (!pp_tree || !*pp_tree)
    {
        return;
    }

    avl_t *p_tree = *pp_tree;

    delete (&p_tree->root, p_destroy_func);
    free(*pp_tree);

} /* tree_destroy() */

struct trunk
{
    struct trunk *prev;
    const char   *str;
};

static void
print_trunks(struct trunk *p)
{
    if (!p)
    {
        return;
    }
    print_trunks(p->prev);
    printf("%s", p->str);
}

static void
print_recursive(node_t *root, struct trunk *prev, int is_left)
{
    if (!root)
    {
        return;
    }

    struct trunk this_disp = { prev, "     " };
    const char  *prev_str  = this_disp.str;
    print_recursive(root->right, &this_disp, 1);

    if (!prev)
    {
        this_disp.str = "---";
    }
    else if (is_left)
    {
        this_disp.str = ".--";
        prev_str      = "    |";
    }
    else
    {
        this_disp.str = "`--";
        prev->str     = prev_str;
    }

    print_trunks(&this_disp);
    printf("%d\n", *(int *)root->data); // whatever custom print you need

    if (prev)
    {
        prev->str = prev_str;
    }
    this_disp.str = "    |";

    print_recursive(root->left, &this_disp, 0);
    if (!prev)
    {
        puts("");
    }
}

void
print_visual(avl_t *root)
{
    if (!root)
    {
        return;
    }
    print_recursive(root->root, NULL, 0);
}

/* end of file */
