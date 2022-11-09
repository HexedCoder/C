// ------------------------------------------------------------------------------
//Lesson 6 Exercise:binary search tree
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

#include "binary_search_tree.h"

#define HALT_DEDUCTION 1
#define TEST_TOTAL 14
#define POINT_TOTAL 40
static const int point[TEST_TOTAL] = {2, 1, 1, 1, 1, 1, 2, 3, 3, 17, 2, 1, 1,
                                      4};
static int testcount = 0;
static int pass = 0;
static int fail = 0;
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
    if (tree_size(t) == sz) {
        ++pass;
        printf("\tPASS!\n");
    } else {
        ++fail;
        printf("\tFAIL! Size of tree = %d but should %d\n", tree_size(t), sz);

#ifdef DEBUG
        print(l);
#endif
    }
}

double test_runtime(tree *t, void (*fp)(void *), double f) {
    struct timeval t1;
    struct timeval t2;
    double time = 0.0;
    gettimeofday(&t1, 0);
    fp(t);
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
        print(l);
#endif
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
    printf("%d\n", root->data); // whatever custom print you need

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
    const int N = 16;
    const int BIGN = 100000;
    int a[] = {10, 12, 3, 8, 11, 14, 2, 5, 7, 9, 13, 15, 1, 4, 16};

    int sz = 0;
    tree *node = 0;
    int min = 1;
    int max = 16;

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Create binary search tree with root node %d\n", 6);

    //Test.
    puts(section);
    tree *bst = create_node(6);

    //Verify size of tree.
    test_size(bst, 1);

    //Verify root node.
    printf("Is the root node correct?\n");
    verify_node(bst, 6);
    puts(footer);

    //New Test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Insert 10, 12, 3, 8, 11, 14, 2, 5, 7, 9, 13, 15, 1, 4, 16\n");

    //Test.
    puts(section);
    for (unsigned int i = 0; i < sizeof(a) / sizeof(int); ++i) {
#ifdef DEBUG
        printf("Inserting node %d\n", i);
#endif
        bst_insert(bst, create_node(a[i]));
    }

    printvisual(bst);

#ifdef DEBUG
    print(bst);
#endif

    //Verify size of tree.
    test_size(bst, N);
    puts(footer);

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Get the minimum value\n");

    //Test.
    puts(section);
    node = bst_minimum_node(bst);

    //Verify node.
    verify_node(node, min);
    puts(footer);

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Get the maximum value\n");

    //Test.
    puts(section);
    node = bst_maximum_node(bst);

    //Verify node.
    verify_node(node, max);
    puts(footer);

    printf("Pre-order: ");
    preorder(bst);
    printf("\n");

    printf("Post-order: ");
    postorder(bst);
    printf("\n");

    printf("In-order: ");
    inorder(bst);
    printf("\n");

    printf("Level-order: ");
    levelorder(bst);
    printf("\n");

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Search for key 13\n");

    //Test.
    puts(section);
    node = search(bst, 13);

    //Verify node.
    verify_node(node, 13);
    puts(footer);

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Search for %d random keys between 0 and 100\n", 10);

    //Test.
    puts(section);
    srand(0);
    int j = 0;
    for (int i = 0; i < 10; ++i) {
        j = rand() % 100;

        printf("Search for %d key\n", j);
        node = search(bst, j);
        if (j >= 1 && j <= N) {
            if (node == 0) {
                --pass;
            } else {
                printf("Found %d key\n", j);
                verify_node(node, j);
            }
        } else {
            if (node != 0) {
                --pass;
            }
        }
    }
    puts(footer);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Inserting node 30\n");

    //Test.
    puts(section);
    bst_insert(bst, create_node(30));
#ifdef DEBUG
    print(bst);
#endif

    //Verify size of tree.
    sz = N + 1;
    test_size(bst, sz);

    //Verify position of new node.
    printf("Is the new node in the correct position?\n");
    node = bst;
    while (node->right != 0) {
        node = node->right;
    }
    verify_node(node, 30);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Inserting nodes %d and %d\n", 25, 29);

    //Test.
    puts(section);
    bst_insert(bst, create_node(25));
    bst_insert(bst, create_node(29));
#ifdef DEBUG
    print(bst);
#endif

    //Verify size of tree.
    sz = sz + 2;
    test_size(bst, sz);

    //Verify position of new node.
    printf("Is the new node %d in the correct position?\n", 25);
    verify_node(node->left, 25);

    //Verify position of new node.
    printf("Is the new node %d in the correct position?\n", 29);
    verify_node(node->left->right, 29);
    puts(footer);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Inserting nodes %d and %d\n", 22, 38);

    //Test.
    puts(section);
    bst_insert(bst, create_node(22));
    bst_insert(bst, create_node(38));
#ifdef DEBUG
    print(bst);
#endif

    //Verify size of tree.
    sz = sz + 2;
    test_size(bst, sz);

    //Verify position of new node.
    printf("Is the new node %d in the correct position?\n", 22);
    verify_node(node->left->left, 22);

    //Verify position of new node.
    printf("Is the new node %d in the correct position?\n", 38);
    verify_node(node->right, 38);
    puts(footer);


    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Delete nodes 1 through 16\n");

    //Test.
    puts(section);
    for (int i = 1; i <= 16; ++i) {
#ifdef DEBUG
        printf("Deleting node %d\n", i);
#endif
        delete_node(&bst, i);
        sz = sz - 1;

        //Verify deletion.
        printf("Has node %d been deleted?\n", i);
        node = search(bst, i);
        if (node == 0) {
            ++pass;
            printf("\tPASS!\n");
        } else {
            ++fail;
            printf("\tFAIL! Node = %d but should have been deleted\n",
                   node->data);
#ifdef DEBUG
            print(l);
#endif
        }
    }

    printvisual(bst);

#ifdef DEBUG
    print(bst);
#endif

    //Verify size of tree.
    test_size(bst, sz);
    puts(footer);

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Delete node %d\n", 30);

    //Test.
    puts(section);
    delete_node(&bst, 30);
#ifdef DEBUG
    print(bst);
#endif

    //Verify size of tree.
    sz = sz - 1;
    test_size(bst, sz);

    //Verify deletion.
    printf("Has node %d been deleted?\n", 30);

    printvisual(bst);

    node = search(bst, 30);
    if (node == 0) {
        ++pass;
        printf("\tPASS!\n");
    } else {
        ++fail;
        printf("\tFAIL! Node = %d but should have been deleted\n", node->data);
#ifdef DEBUG
        print(l);
#endif
    }
    puts(footer);

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Search for key %d\n", 22);

    //Test.
    puts(section);
    node = search(bst, 22);

    //Verify node.
    verify_node(node, 22);
    puts(footer);

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Destroy tree\n");

    //Test.
    puts(section);
    tree_delete(&bst);

    //Verify size of tree.
    test_size(bst, 0);
    puts(footer);

    //New test.
    puts(header);
    printf("Test #%d:\n", ++testcount);
    printf("Performance tests\n");

    //Test.
    puts(section);
    printf("Time to create tree of %d nodes\n", BIGN);
    int *rarray = random_array(BIGN);
    gettimeofday(&t1, 0);
    bst = create_node(rarray[0]);
    for (int i = 1; i < BIGN; ++i) {
        bst_insert(bst, create_node(rarray[i]));
    }
    gettimeofday(&t2, 0);
    time = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
    printf("Time: %f seconds\n", time);
    if (time < 0.14) {
        ++pass;
        printf("\tPASS!\n");
    } else {
        ++fail;
        printf("\tFAIL! Too slow\n");
    }
    free(rarray);

    //Test.
    puts(section);
    printf("Time to get the size of the tree\n");
    time = test_runtime(bst, (void (*)(void *)) &tree_size, 0.02);
    printf("Time: %f seconds\n", time);

    //Test.
    puts(section);
    printf("Time to get maximum\n");
    time = test_runtime(bst, (void (*)(void *)) &bst_maximum, 0.001);
    printf("Time: %f seconds\n", time);

    //Test.
    puts(section);
    printf("Time to destroy tree\n");
    time = test_delete_runtime(bst, &tree_delete, 0.05);
    printf("Time: %f seconds\n", time);
    puts(footer);
    //End of exercise.
    report();
}
