#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "avl.h"
#include "llist.h"

typedef struct node_t
{
    struct node_t *p_parent;
    struct node_t *p_left;
    struct node_t *p_right;
    int            height;
    void          *data;
} node_t; /* node_t */

struct avl_t
{
    struct node_t  *p_root;
    compare         compare_func;
    action          action_func;
    pthread_mutex_t p_tree_mutex;
}; /* avl_t */

struct trunk
{
    struct trunk *p_prev;
    const char   *p_str;
}; /* trunk() */

static node_t *create_node(void *p_data);
/**
 * Creates a new node with the given data.
 *
 * @param data The data to be stored in the node.
 * @return Pointer to the newly created node on success, NULL on failure.
 */

static void preorder_action(node_t *p_node, action action_func);
/**
 * Performs the preorder action on the given node and its descendants.
 *
 * @param p_node The current node.
 * @param action_func Pointer to the action function.
 */

static void postorder_action(node_t *p_node, action action_func);
/**
 * Performs the postorder action on the given node and its descendants.
 *
 * @param p_node The current node.
 * @param action_func Pointer to the action function.
 */

static void inorder_action(node_t *p_node, action action_func);
/**
 * Performs the inorder action on the given p_node and its descendants.
 *
 * @param p_node The current p_node.
 * @param action_func Pointer to the action function.
 */

static void *get_max(node_t *p_node);
/**
 * Returns the maximum data value stored in the given subtree.
 *
 * @param p_node The root node of the subtree.
 * @return Pointer to the maximum data value on success, NULL on failure.
 */

static void *get_min(node_t *p_node);
/**
 * Returns the minimum data value stored in the given subtree.
 *
 * @param p_node The root node of the subtree.
 * @return Pointer to the minimum data value on success, NULL on failure.
 */

static int get_t_size(node_t *p_node);
/**
 * Calculates and returns the number of elements in the given subtree.
 *
 * @param node The root node of the subtree.
 * @return The number of elements in the subtree.
 */

static node_t *rotate_left(avl_t **pp_root_tree, node_t *p_new_node);
/**
 * Performs a left rotation on the given node in the AVL tree.
 *
 * @param pp_root_tree Pointer to the pointer of the AVL tree.
 * @param p_new_node The node to be rotated.
 * @return The new root node after rotation.
 */

static node_t *rotate_right(avl_t **pp_root_tree, node_t *p_new_node);
/**
 * Performs a right rotation on the given node in the AVL tree.
 *
 * @param root_tree Pointer to the pointer of the AVL tree.
 * @param new_node The node to be rotated.
 * @return The new root node after rotation.
 */

static node_t *insert(node_t *p_tree, void *p_data, compare compare_func);
/**
 * Inserts a new node with the given data into the AVL tree.
 *
 * @param tree The current node in the AVL tree.
 * @param data The data to be inserted.
 * @param compare_func Pointer to the comparison function.
 * @return Pointer to the inserted node on success, NULL on failure.
 */

static void *find(node_t *p_node, void *p_value, compare compare_func);
/**
 * Searches for a pp_node with the given value in the AVL tree.
 *
 * @param pp_node The current pp_node in the AVL tree.
 * @param value The value to search for.
 * @param compare_func Pointer to the comparison function.
 * @return Pointer to the pp_node containing the value on success, NULL on
 * failure.
 */

static void delete(node_t **pp_node);
/**
 * Recursively deletes the nodes in the AVL tree and frees the associated
 * memory.
 *
 * @param node Pointer to the pointer of the current node.
 */

static void print_trunks(struct trunk *p_tree);
/**
 * Prints the indentation trunks for pretty printing the AVL tree.
 *
 * @param p_tree Pointer to the trunk structure.
 */

static void print_recursive(node_t *p_root, struct trunk *p_prev, int is_left);
/**
 * Recursively prints the AVL tree in a visually appealing format.
 *
 * @param root The current root node.
 * @param prev Pointer to the previous trunk.
 * @param is_left Indicator for whether the current node is the left child of
 * its parent.
 */

avl_t *
tree_create(compare compare_func, action action_func)
{
    avl_t *p_tree = calloc(1, sizeof(*p_tree));

    p_tree->compare_func = compare_func;
    p_tree->action_func  = action_func;

    return p_tree;
} /* tree_create() */

static node_t *
create_node(void *p_data)
{
    node_t *node = NULL;

    node = calloc(1, sizeof(*node));

    if (NULL != node)
    {
        node->data = p_data;
    }

    return node;
} /* create_node() */

static int
get_t_size(node_t *p_node)
{
    if (NULL == p_node)
    {
        return 0;
    }

    int count = get_t_size(p_node->p_left);
    count += get_t_size(p_node->p_right);

    return 1 + count;
} /* get_t_size() */

static node_t *
rotate_left(avl_t **pp_root_tree, node_t *p_new_node)
{
    avl_t   *p_root_tree  = *pp_root_tree;
    node_t **pp_root_node = &p_root_tree->p_root;

    if (NULL == p_new_node || p_new_node == *pp_root_node
        || 0 < p_root_tree->compare_func(p_new_node->p_parent->data,
                                         p_new_node->data))
    {
        return p_new_node;
    }

    node_t *p_parent  = p_new_node->p_parent;
    node_t *p_gparent = p_parent->p_parent;
    node_t *p_child   = p_new_node->p_left;

    p_new_node->p_parent = p_gparent;
    p_new_node->p_left   = p_parent;
    p_parent->p_parent   = p_new_node;
    p_parent->p_right    = p_child;

    if (NULL != p_child)
    {
        p_child->p_parent = p_parent;
    }

    if (NULL != p_gparent)
    {
        if (p_root_tree->compare_func(p_parent->data, p_gparent->data) < 0)
        {
            p_gparent->p_left = p_new_node;
        }

        else
        {
            p_gparent->p_right = p_new_node;
        }
    }

    else
    {
        // p_parent of rotated new_node is the root; rotated new_node
        // becomes root
        p_root_tree->p_root = p_new_node;
    }

    return p_new_node;
} /* rotate_left() */

static node_t *
rotate_right(avl_t **pp_root_tree, node_t *p_new_node)
{
    avl_t   *p_root_tree  = *pp_root_tree;
    node_t **pp_root_node = &p_root_tree->p_root;

    if (NULL == p_new_node || p_new_node == *pp_root_node
        || p_root_tree->compare_func(p_new_node->p_parent->data,
                                     p_new_node->data)
               < 0)
    {
        return p_new_node;
    }
    node_t *p_parent  = p_new_node->p_parent;
    node_t *p_gparent = p_parent->p_parent;
    node_t *p_child   = p_new_node->p_right;

    p_new_node->p_parent = p_gparent;
    p_new_node->p_right  = p_parent;
    p_parent->p_parent   = p_new_node;
    p_parent->p_left     = p_child;

    if (NULL != p_child)
    {
        p_child->p_parent = p_parent;
    }

    if (NULL != p_gparent)
    {
        if (p_root_tree->compare_func(p_parent->data, p_gparent->data) < 0)
        {
            p_gparent->p_left = p_new_node;
        }

        else
        {
            p_gparent->p_right = p_new_node;
        }
    }

    else
    {
        // p_parent of rotated new_node is the root; rotated new_node
        // becomes root
        p_root_tree->p_root = p_new_node;
    }

    return p_new_node;
} /* rotate_right() */

static node_t *
insert(node_t *p_tree, void *p_data, compare compare_func)
{
    int result = compare_func(p_data, p_tree->data);

    if (0 == result)
    {
        return NULL;
    }

    node_t *p_tmp_node = NULL;

    if (result < 0)
    {
        if (NULL != p_tree->p_left)
        {
            return insert(p_tree->p_left, p_data, compare_func);
        }

        else
        {
            p_tmp_node = create_node(p_data);
            if (NULL != p_tmp_node)
            {
                p_tmp_node->p_parent = p_tree;
                p_tree->p_left       = p_tmp_node;
            }
        }
    }

    else
    {
        if (NULL != p_tree->p_right)
        {
            return insert(p_tree->p_right, p_data, compare_func);
        }

        else
        {
            p_tmp_node = create_node(p_data);
            if (NULL != p_tmp_node)
            {
                p_tmp_node->p_parent = p_tree;
                p_tree->p_right      = p_tmp_node;
            }
        }
    }

    return p_tmp_node;
} /* insert() */

void *
avl_insert(avl_t *p_tree, void *p_data)
{
    if (NULL == p_tree || NULL == p_data)
    {
        return NULL;
    }

    pthread_mutex_lock(&p_tree->p_tree_mutex);
    node_t *p_tmp_node = NULL;
    if (NULL == p_tree->p_root)
    {
        p_tmp_node     = calloc(1, sizeof(*p_tmp_node));
        p_tree->p_root = p_tmp_node;

        if (p_tmp_node)
        {
            p_tmp_node->data = p_data;
        }
    }

    else
    {
        p_tmp_node = insert(p_tree->p_root, p_data, p_tree->compare_func);
    }

    if (NULL == p_tmp_node)
    {
        return NULL;
    }

    p_tmp_node->height = 1;
    int bal_factor     = 0;

    while (NULL != p_tmp_node->p_parent && bal_factor < 2)
    {
        p_tmp_node   = p_tmp_node->p_parent;
        int left_bf  = p_tmp_node->p_left ? p_tmp_node->p_left->height : 0;
        int right_bf = p_tmp_node->p_right ? p_tmp_node->p_right->height : 0;
        int max      = (left_bf < right_bf) ? right_bf : left_bf;
        bal_factor   = abs(right_bf - left_bf);
        p_tmp_node->height = max + 1;
    }

    // no balance needed
    if (NULL == p_tmp_node->p_parent && bal_factor < 2)
    {
        pthread_mutex_unlock(&p_tree->p_tree_mutex);
        return p_data;
    }

    if (p_tree->compare_func(p_data, p_tmp_node->data) < 0)
    {
        if (p_tree->compare_func(p_data, p_tmp_node->p_left->data) < 0)
        {
            p_tmp_node = rotate_right(&p_tree, p_tmp_node->p_left);
            p_tmp_node->p_right->height = p_tmp_node->height - 1;
        }

        else
        {
            p_tmp_node = rotate_right(
                &p_tree, rotate_left(&p_tree, p_tmp_node->p_left->p_right));

            if (NULL != p_tmp_node)
            {
                p_tmp_node->p_left->height  = p_tmp_node->height;
                p_tmp_node->p_right->height = p_tmp_node->height;
                ++p_tmp_node->height;
            }
        }
    }

    else
    {
        if (0 < p_tree->compare_func(p_data, p_tmp_node->p_right->data))
        {
            p_tmp_node = rotate_left(&p_tree, p_tmp_node->p_right);
            p_tmp_node->p_left->height = p_tmp_node->height - 1;
        }
        else
        {
            p_tmp_node = rotate_left(
                &p_tree, rotate_right(&p_tree, p_tmp_node->p_right->p_left));
            if (NULL != p_tmp_node)
            {
                p_tmp_node->p_left->height  = p_tmp_node->height;
                p_tmp_node->p_right->height = p_tmp_node->height;
                ++p_tmp_node->height;
            }
        }
    }
    pthread_mutex_unlock(&p_tree->p_tree_mutex);

    return p_data;
} /* avl_insert() */

static void *
find(node_t *p_node, void *p_value, compare compare_func)
{
    if (NULL == p_node)
    {
        return NULL;
    }

    int cmp_ret = compare_func(p_value, p_node->data);

    if (cmp_ret < 0)
    {
        return find(p_node->p_left, p_value, compare_func);
    }
    else if (cmp_ret > 0)
    {
        return find(p_node->p_right, p_value, compare_func);
    }
    else
    {
        return p_node;
    }
} /* find() */

void *
tree_search(avl_t *p_tree, void *p_value)
{
    if (NULL == p_tree || NULL == p_value)
    {
        return NULL;
    }

    return find(p_tree->p_root, p_value, p_tree->compare_func);
} /* tree_search() */

static void
preorder_action(node_t *p_node, action action_func)
{
    if (NULL == p_node)
    {
        return;
    }

    action_func(p_node->data);
    preorder_action(p_node->p_left, action_func);
    preorder_action(p_node->p_right, action_func);
} /* preorder_action() */

static void
postorder_action(node_t *p_node, action action_func)
{
    if (NULL == p_node)
    {
        return;
    }

    postorder_action(p_node->p_left, action_func);
    postorder_action(p_node->p_right, action_func);
    action_func(p_node->data);
} /* postorder_action() */

static void
inorder_action(node_t *p_node, action action_func)
{
    if (NULL == p_node)
    {
        return;
    }

    inorder_action(p_node->p_left, action_func);
    action_func(p_node->data);
    inorder_action(p_node->p_right, action_func);
} /* inorder_action() */

void
preorder(avl_t *p_tree)
{
    if (NULL == p_tree)
    {
        return;
    }

    if (NULL == p_tree->action_func)
    {
        printf("Invalid action function provided\n");
        return;
    }

    preorder_action(p_tree->p_root, p_tree->action_func);
    printf("\n");
} /* preorder() */

void
postorder(avl_t *p_tree)
{
    if (NULL == p_tree)
    {
        return;
    }

    if (NULL == p_tree->action_func)
    {
        printf("Invalid action function provided\n");
        return;
    }

    postorder_action(p_tree->p_root, p_tree->action_func);
    printf("\n");
} /* postorder() */

void
inorder(avl_t *p_tree)
{
    if (NULL == p_tree)
    {
        return;
    }

    if (NULL == p_tree->action_func)
    {
        printf("Invalid action function provided\n");
        return;
    }

    inorder_action(p_tree->p_root, p_tree->action_func);
    printf("\n");
} /* inorder() */

void
level_order(avl_t *p_tree)
{
    if (NULL == p_tree)
    {
        return;
    }

    if (NULL == p_tree->action_func)
    {
        printf("Invalid action function provided\n");
        return;
    }

    llist_t *p_queue     = llist_create();
    node_t  *p_curr_node = p_tree->p_root;

    while (NULL != p_curr_node)
    {
        p_tree->action_func(p_curr_node->data);

        if (NULL != p_curr_node->p_left)
        {
            llist_enqueue(p_queue, p_curr_node->p_left);
        }
        if (NULL != p_curr_node->p_right)
        {
            llist_enqueue(p_queue, p_curr_node->p_right);
        }

        p_curr_node = llist_dequeue(p_queue);
    }
    llist_delete(p_queue, NULL);
    printf("\n");
} /* level_order() */

static void *
get_max(node_t *p_node)
{
    while (NULL != p_node->p_right)
    {
        p_node = p_node->p_right;
    }
    return p_node->data;
} /* get_max() */

void *
tree_maximum(avl_t *p_tree)
{
    if (NULL == p_tree->p_root)
    {
        return NULL;
    }

    return get_max(p_tree->p_root);
} /* tree_maximum() */

static void *
get_min(node_t *p_node)
{
    while (NULL != p_node->p_left)
    {
        p_node = p_node->p_left;
    }
    return p_node->data;
} /* get_min() */

void *
tree_minimum(avl_t *p_tree)
{
    if (NULL == p_tree->p_root)
    {
        return 0;
    }

    return get_min(p_tree->p_root);
} /* tree_minimum() */

int
tree_size(avl_t *p_tree)
{
    if (NULL == p_tree)
    {
        return 0;
    }

    return get_t_size(p_tree->p_root);
} /* tree_size() */

static void *
min_node(node_t *p_node)
{
    while (NULL != p_node->p_left)
    {
        p_node = p_node->p_left;
    }
    return p_node;
} /* min_node() */

static int
delete_node(node_t **pp_root, node_t **pp_target, compare compare_func)
{
    if (NULL == pp_target || NULL == pp_root)
    {
        return 0;
    }

    node_t *p_target = *pp_target;

    //	printf("Removing %d\n", *(int *)p_target->data);

    // Handle pp_root p_target

    if (*pp_root == *pp_target)
    {
        if (NULL == p_target->p_left && NULL == p_target->p_right)
        {
            free(p_target->data);
            free(*pp_root);
            *pp_root = NULL;
            return 1;
        }

        if (NULL == p_target->p_left || NULL == p_target->p_right)
        {
            node_t *p_node
                = p_target->p_left ? p_target->p_left : p_target->p_right;
            //			printf("Temp Node: %s\n", (char *)p_node->data);
            //// How to print data
            p_node->p_parent = NULL;
            free(p_target->data);
            free(*pp_target);
            *pp_root = p_node;
            return 1;
        }
    }
    // Handle if inner p_target has no right for minimum
    if (NULL == p_target->p_right)
    {
        printf("%d Has no right\n", *(int *)p_target->data);
        int cmp_ret = compare_func(p_target->p_parent->data, p_target->data);

        if (0 < cmp_ret)
        {
            printf("%d is less than %d\n",
                   *(int *)p_target->data,
                   *(int *)p_target->p_parent->data);
            p_target->p_parent->p_left = p_target->p_left;
        }

        else
        {
            p_target->p_parent->p_right = p_target->p_left;
        }

        if (NULL != p_target->p_left)
        {
            p_target->p_left->p_parent = p_target->p_parent;
        }

        free(p_target->data);
        free(p_target);
        return 1;
    }
    // Standard handling
    node_t *p_min = min_node(p_target->p_right);

    void *p_temp   = p_min->data;
    p_min->data    = p_target->data;
    p_target->data = p_temp;

    if (p_min == p_target->p_right)
    {
        p_target->p_right = p_min->p_right;
        if (NULL != p_min->p_right)
        {
            p_min->p_right->p_parent = p_target;
        }
    }

    else
    {
        p_min->p_parent->p_left = p_min->p_right;
        if (NULL != p_min->p_right)
        {
            p_min->p_right->p_parent = p_min->p_parent;
        }
    }
    free(p_min->data);
    free(p_min);
    return 1;
} /* delete_node() */

int
tree_delete(avl_t **pp_root, void *p_data)
{
    avl_t *p_root = *pp_root;

    if (NULL == p_root || NULL == p_root->compare_func)
    {
        return 0;
    }

    node_t *p_search_node = tree_search(*pp_root, p_data);

    if (NULL == p_search_node)
    {
        printf("Search Value Not Found\n");
        return 0;
    }

    pthread_mutex_lock(&p_root->p_tree_mutex);
    int delete_ret
        = delete_node(&p_root->p_root, &p_search_node, p_root->compare_func);
    pthread_mutex_unlock(&p_root->p_tree_mutex);

    return delete_ret;
} /* tree_delete() */

static void delete(node_t **pp_node)
{
    if (NULL == pp_node || NULL == *pp_node)
    {
        return;
    }

    node_t *p_node = *pp_node;
    delete (&p_node->p_left);
    delete (&p_node->p_right);

    free(p_node->data);
    free(p_node);
    *pp_node = NULL;
} /* delete() */

void
tree_destroy(avl_t **pp_tree)
{
    if (NULL == pp_tree || NULL == *pp_tree)
    {
        return;
    }

    avl_t *p_treetmp = *pp_tree;

    delete (&p_treetmp->p_root);
    free(*pp_tree);
} /* tree_destroy() */

static void
print_trunks(struct trunk *p_tree)
{
    if (NULL == p_tree)
    {
        return;
    }

    print_trunks(p_tree->p_prev);
    printf("%s", p_tree->p_str);
} /* print_trunks() */

static void
print_recursive(node_t *p_root, struct trunk *p_prev, int is_left)
{
    if (NULL == p_root)
    {
        return;
    }

    struct trunk this_disp  = { p_prev, "     " };
    const char  *p_prev_str = this_disp.p_str;
    print_recursive(p_root->p_right, &this_disp, 1);

    if (NULL == p_prev)
    {
        this_disp.p_str = "---";
    }

    else if (1 == is_left)
    {
        this_disp.p_str = ".--";
        p_prev_str      = "    |";
    }

    else
    {
        this_disp.p_str = "`--";
        p_prev->p_str   = p_prev_str;
    }

    print_trunks(&this_disp);
    printf("%d\n", *(int *)p_root->data); // whatever custom print you need

    if (NULL != p_prev)
    {
        p_prev->p_str = p_prev_str;
    }
    this_disp.p_str = "    |";

    print_recursive(p_root->p_left, &this_disp, 0);
    if (NULL == p_prev)
    {
        puts("");
    }
} /* print_recursive() */

void
print_visual(avl_t *p_root)
{
    if (NULL == p_root)
    {
        return;
    }
    print_recursive(p_root->p_root, NULL, 0);
} /* print_visual() */
