// ------------------------------------------------------------------------------
//Lesson 8 Exercise:AVL tree
//
//Print debug statements:Compile with - DDEBUG
// ------------------------------------------------------------------------------
#include <err.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>

#include "avl.h"

#define HALT_DEDUCTION 1
#define TEST_TOTAL 14
#define POINT_TOTAL 19
static const int point[TEST_TOTAL] = {2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2};
static int testcount = 0;
static int pass = 0;
static int fail = 0;
static int *traversal_sequence = 0;
static int traversal_step = 0;
struct {
    int error_flag;
    int on_step;
    int on_key;
    tree *on_node;
} traversal_fail;

void printvisual(tree *root);

static const char *header =
        "\n-------------------------------------------------------------------------------";
static const char *section =
        "-----------------------------------section-------------------------------------";
static const char *footer =
        "-------------------------------------------------------------------------------\n\n";

int *random_array(int n) {
    //Return an array of n unique random numbers between 0 and n - 1.
    //
    //Initialize array from 0 to n - 1, then randomly permute to avoid duplicates.
    int *a = (int *) malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        a[i] = i;
    }
    srand(0);
    int j;
    for (int i = 0; i < n; ++i) {
        j = rand() % n;
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
    return a;
}

void test_size(tree *t, int sz) {
    printf("Is the tree the expected size?\n");
    if (size(t) == sz) {
        ++pass;
        printf("\tPASS!\n");
    } else {
        ++fail;
        printf("\tFAIL! Size of tree = %d but should %d\n", size(t), sz);

#ifdef DEBUG
        printvisual(t);
#endif
    }
}

void test_traversal(tree *t,
                    void (*fp)(tree *, void (*)(tree *)), void (*tp)(tree *),
                    int size) {
    traversal_step = 0;
    traversal_fail.error_flag = 0;
    traversal_fail.on_step = 0;
    traversal_fail.on_key = 0;
    traversal_fail.on_node = 0;
    fp(t, tp);
    if (traversal_fail.error_flag == 0) {
        if (traversal_step < size) {
            ++fail;
            printf("\tFAIL! Traversal halted early on step %d instead of %d\n",
                   traversal_step, size);
        } else {
            ++pass;
            printf("\tPASS!\n");
        }
    } else {
        ++fail;
        printf("\tFAIL! Traversal failed on step %d: Found node %d instead of %d\n",
               traversal_fail.on_step,
               traversal_fail.on_node->data,
               traversal_fail.on_key);
    }
}

double test_traversal_runtime(tree *t, void (*fp)(tree *, void (*)(tree *)),
                              void (*tp)(tree *), double f) {
    struct timeval t1;
    struct timeval t2;
    double time = 0.0;
    gettimeofday(&t1, 0);
    fp(t, tp);
    gettimeofday(&t2, 0);
    time = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
    if (time < f) {
        ++pass;
        printf("\tPASS!\n");
    } else {
        ++fail;
        printf("\tFAIL! Too slow\n");
    }
    return time;
}

double test_delete_runtime(tree *t, void (*fp)(tree **), double f) {
    struct timeval t1;
    struct timeval t2;
    double time = 0.0;
    gettimeofday(&t1, 0);
    fp(&t);
    gettimeofday(&t2, 0);
    time = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
    if (time < f) {
        ++pass;
        printf("\tPASS!\n");
    } else {
        ++fail;
        printf("\tFAIL! Too slow\n");
    }
    return time;
}

void verify_node(tree *t, int v) {
    if (t->data == v) {
        ++pass;
        printf("\tPASS!\n");
    } else {
        ++fail;
        printf("\tFAIL! Node = %d but should %d\n", t->data, v);
#ifdef DEBUG
        printvisual(t);
#endif
    }
}

void verify_traversal(tree *t) {
    if (t->data != traversal_sequence[traversal_step]) {
        traversal_fail.error_flag = 1;
        traversal_fail.on_step = traversal_step;
        traversal_fail.on_key = traversal_sequence[traversal_step];
        traversal_fail.on_node = t;
    }
    ++traversal_step;
}

void noop(tree *t) {
    (void) t;
}

void verify_balance(tree *t) {
    while (t->parent != 0) {
        int lh = (t->left) ? t->left->height : 0;
        int rh = (t->right) ? t->right->height : 0;
        if (abs(rh - lh) > 1) {
            ++fail;
            printf("\tFAIL! Tree is not balanced!\n");
#ifdef DEBUG
            printvisual(t);
#endif
            break;
        }
    }
    if (t->parent == 0) {
        ++pass;
        printf("\tPASS!\n");
    }
}

void report(void) {
    puts(header);
    printf("Final Report!\n");
    printf("Completed %d out of %d tests\n", testcount, TEST_TOTAL);
    printf("Total earned points = %d\n", pass);
    printf("Total possible points = %d\n", POINT_TOTAL);
    printf("Final score: %3.2f%%\n", (double) pass / POINT_TOTAL * 100);
    puts("");
    printf("Points distribution:\n");
    for (int i = 0; i < TEST_TOTAL; ++i) {
        printf("Problem #%d:\t%d points\n", i + 1, point[i]);
    }
    puts(footer);
}

void signal_handler(int signal, siginfo_t *info, void *context) {
    (void) context;
    puts(header);
    puts("");
    printf("EXECUTION PREMATURELY HALTED!\n");
    printf("Caught signal (%d): %s\n", info->si_signo, strsignal(signal));
    printf("Fail at memory address: %p\n", info->si_addr);
    puts("");

    //Count incomplete tests.
    --testcount;
    int remain = TEST_TOTAL - testcount;

    //Sum up remaining points.
    int skip = POINT_TOTAL - pass;

    //Point status.
    printf("Current points = %d\n", pass);
    printf("Missed points due to %d incomplete test(s) = %d\n", remain, skip);
    if (pass > 0)
        --pass;
    printf("Points after deducting %d point(s) for unexpected error = %d\n",
           HALT_DEDUCTION, pass);

    //Report
    report();
    exit(1);
}

struct trunk {
    struct trunk *prev;
    const char *str;
};

static void print_trunks(struct trunk *p) {
    if (!p) {
        return;
    }
    print_trunks(p->prev);
    printf("%s", p->str);
}

static void print_recursive(tree *root, struct trunk *prev, int is_left) {
    if (!root) {
        return;
    }

    struct trunk this_disp = {prev, "     "};
    const char *prev_str = this_disp.str;
    print_recursive(root->right, &this_disp, 1);

    if (!prev) {
        this_disp.str = "---";
    } else if (is_left) {
        this_disp.str = ".--";
        prev_str = "    |";
    } else {
        this_disp.str = "`--";
        prev->str = prev_str;
    }

    print_trunks(&this_disp);
    printf("%d\n", root->data); // whatever custom printvisual you need

    if (prev) {
        prev->str = prev_str;
    }
    this_disp.str = "    |";

    print_recursive(root->left, &this_disp, 0);
    if (!prev) {
        puts("");
    }
}

void printvisual(tree *root) {
    if (!root) {
        return;
    }
    print_recursive(root, NULL, 0);
}

int main(void) {
    struct timeval t1;
    struct timeval t2;
    double time = 0.0;
    struct sigaction sa;

    //Signal handling.
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = signal_handler;
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGBUS, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);

    //Declarations specific to this exercise.
    const int BIGN = 100000;
    int a[] = {10, 12, 3, 8, 11, 14, 2, 5, 7, 9, 13, 15, 1, 4, 16, 30, 25, 22,
               29, 38};
    int inorder_sequence[] =
            {
                    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 22,
                    25, 29, 30, 38
            };
    int sz = 0;
    int tree_size = 0;

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Create binary search tree with root node %d\n", 6);

    //Test.
    puts(section);
    tree *avl = create_node(6);
    sz = 1;

    //Verify size of tree.
    test_size(avl, sz);

    //Verify root node.
    printf("Is the root node correct?\n");
    verify_node(avl, 6);
    puts(footer);


    //New Test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Insert 10, 12, 3, 8, 11, 14, 2, 5, 7, 9, 13, 15, 1, 4, 16, ");
    printf("30, 25, 22, 29, 38\n");

    //Test.
    puts(section);
    for (unsigned int i = 0; i < sizeof(a) / sizeof(int); ++i) {
#ifdef DEBUG
        printf("Inserting node %d\n", i);
#endif
        insert(avl, create_node(a[i]));
    }

    printvisual(avl);

    sz = sz + sizeof(a) / sizeof(int);

    //Verify size of tree.
    test_size(avl, sz);

    //Verify BST property.
    printf("Is BST property preserved? (verifying with inorder traversal)\n");
    traversal_sequence = &inorder_sequence[0];
    tree_size = sizeof(inorder_sequence) / sizeof(int);
    test_traversal(avl, &inorder, &verify_traversal, tree_size);
    puts(footer);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Right rotation on node %d\n", 8);

    //Test.
    puts(section);
    rotate_right(&avl, search(avl, 8));

    //Verify BST property.
    printf("Is BST property preserved? (verifying with inorder traversal)\n");
    traversal_sequence = &inorder_sequence[0];
    tree_size = sizeof(inorder_sequence) / sizeof(int);
    test_traversal(avl, &inorder, &verify_traversal, tree_size);
    puts(footer);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Left rotation on node %d\n", 5);

    //Test.
    puts(section);
    rotate_left(&avl, search(avl, 5));

    //Verify BST property.
    printf("Is BST property preserved? (verifying with inorder traversal)\n");
    traversal_sequence = &inorder_sequence[0];
    tree_size = sizeof(inorder_sequence) / sizeof(int);
    test_traversal(avl, &inorder, &verify_traversal, tree_size);
    puts(footer);

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Right rotation on node %d\n", 5);

    //Test.
    puts(section);
    rotate_right(&avl, search(avl, 5));

    //Verify BST property.
    printf("Is BST property preserved? (verifying with inorder traversal)\n");
    traversal_sequence = &inorder_sequence[0];
    tree_size = sizeof(inorder_sequence) / sizeof(int);
    test_traversal(avl, &inorder, &verify_traversal, tree_size);
    puts(footer);

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Left rotation on node %d\n", 6);

    //Test.
    puts(section);
    rotate_left(&avl, search(avl, 6));
#ifdef DEBUG
    printvisual(avl);
#endif

    //Verify BST property.
    printf("Is BST property preserved? (verifying with inorder traversal)\n");
    traversal_sequence = &inorder_sequence[0];
    tree_size = sizeof(inorder_sequence) / sizeof(int);
    test_traversal(avl, &inorder, &verify_traversal, tree_size);
    puts(footer);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Right rotation on node %d\n", 3);

    //Test.
    puts(section);
    rotate_right(&avl, search(avl, 3));
#ifdef DEBUG
    printvisual(avl);
#endif

    //Verify BST property.
    printf("Is BST property preserved? (verifying with inorder traversal)\n");
    traversal_sequence = &inorder_sequence[0];
    tree_size = sizeof(inorder_sequence) / sizeof(int);
    test_traversal(avl, &inorder, &verify_traversal, tree_size);
    puts(footer);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Left rotation on node %d\n", 10);

    //Test.
    puts(section);
    rotate_left(&avl, search(avl, 10));
#ifdef DEBUG
    printvisual(avl);
#endif

    //Verify BST property.
    printf("Is BST property preserved? (verifying with inorder traversal)\n");
    traversal_sequence = &inorder_sequence[0];
    tree_size = sizeof(inorder_sequence) / sizeof(int);
    test_traversal(avl, &inorder, &verify_traversal, tree_size);
    puts(footer);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Right rotation on node %d\n", 13);

    //Test.
    puts(section);
    rotate_right(&avl, search(avl, 13));
#ifdef DEBUG
    printvisual(avl);
#endif

    //Verify BST property.
    printf("Is BST property preserved? (verifying with inorder traversal)\n");
    traversal_sequence = &inorder_sequence[0];
    tree_size = sizeof(inorder_sequence) / sizeof(int);
    test_traversal(avl, &inorder, &verify_traversal, tree_size);
    puts(footer);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Left rotation on node %d\n", 14);

    //Test.
    puts(section);
    rotate_left(&avl, search(avl, 14));
#ifdef DEBUG
    printvisual(avl);
#endif

    //Verify BST property.
    printf("Is BST property preserved? (verifying with inorder traversal)\n");
    traversal_sequence = &inorder_sequence[0];
    tree_size = sizeof(inorder_sequence) / sizeof(int);
    test_traversal(avl, &inorder, &verify_traversal, tree_size);
    puts(footer);

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Destroy tree\n");

    //Test.
    puts(section);
    delete(&avl);

    //Verify size of tree.
    test_size(avl, 0);
    puts(footer);

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Automatic balance on AVL insertion\n");

    //Test.
    puts(section);
    printf("Create an AVL tree with root node %d\n", 6);
    avl = create_node(6);

    //Insert new nodes.
    printf("Insert nodes 4, 18, 9, 26, 15, 13\n");
    avl_insert(avl, create_node(4));
    avl_insert(avl, create_node(18));
    avl_insert(avl, create_node(9));
    avl_insert(avl, create_node(26));
    avl_insert(avl, create_node(15));
#ifdef DEBUG
    printvisual(avl);
#endif

    //Verify tree is balanced.
    printf("Verify tree is balanced\n");
    verify_balance(avl);

    //Insert AVL violation.
    puts(section);
    printf("Insert node 13 creating AVL violation at node 9\n");
    avl_insert(avl, create_node(13));
#ifdef DEBUG
    printvisual(avl);
#endif

    //Verify tree is balanced.
    printf("Verify tree is balanced\n");
    verify_balance(avl);
    puts(footer);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Automatic balance on AVL insertion\n");

    //Test.
    puts(section);
    printf("Create an AVL tree with root node %d\n", 6);
    delete(&avl);
    avl = create_node(6);
    printf("Insert 10, 12, 3, 8, 11, 14, 2, 5, 7, 9, 13, 15, 1, 4, 16, ");
    printf("30, 25, 22, 29, 38\n");

    //Test.
    puts(section);
    for (unsigned int i = 0; i < sizeof(a) / sizeof(int); ++i) {
#ifdef DEBUG
        printf("Inserting node %d\n", i);
#endif
        avl_insert(avl, create_node(a[i]));
    }
#ifdef DEBUG
    printvisual(avl);
#endif

    printvisual(avl);

    //Verify tree is balanced.
    printf("Verify tree is balanced\n");
    verify_balance(avl);

    //Verify BST property.
    printf("Is BST property preserved? (verifying with inorder traversal)\n");
    traversal_sequence = &inorder_sequence[0];
    tree_size = sizeof(inorder_sequence) / sizeof(int);
    test_traversal(avl, &inorder, &verify_traversal, tree_size);
    delete(&avl);
    puts(footer);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Performance tests\n");

    //Test.
    puts(section);
    printf("Time to create a self-balancing tree of %d nodes\n", BIGN);
    int *rarray = random_array(BIGN);
    gettimeofday(&t1, 0);
    avl = create_node(rarray[0]);
    for (int i = 1; i < BIGN; ++i) {
        avl_insert(avl, create_node(rarray[i]));
    }
    gettimeofday(&t2, 0);
    time = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
    printf("Time: %f seconds\n", time);
    if (time < 0.2) {
        ++pass;
        printf("\tPASS!\n");
    } else {
        ++fail;
        printf("\tFAIL! Too slow\n");
    }
    free(rarray);


    //Test.
    puts(section);
    printf("Time to destroy tree\n");
    time = test_delete_runtime(avl, &delete, 0.05);
    printf("Time: %f seconds\n", time);
    puts(footer);

    //End of exercise.
    report();
}