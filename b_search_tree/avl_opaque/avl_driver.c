#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdint-gcc.h>
#include "avl.h"

//void print(void *data) {
//
//    printf("%lu ", (uint64_t) data);
//}

void print(void *data) {

    printf("%s ", (char *) data);
}

int
person_compare(char *name_1, char *name_2) {

    int index = 0;
    while (name_1[index] == name_2[index] && name_2[index]) {
        ++index;
    }

    return name_2[index] - name_1[index];
}

int int_cmp(const void *input_1, const void *input_2) {
    return (uint64_t) input_2 - (uint64_t) input_1;
}

int main(void) {
//    uint64_t arr[] = {90, 1, 8, 20, 20, 89, 13, 89, 81, 61, 62, 39, 96, 29, 93};
//    uint64_t arr2[] = {90, 1, 8, 20, 20, 89, 13, 89, 81, 61, 62, 39, 96, 29, 93};
    char *arr[] = {"Kyle", "Jacob", "Chante", "Tim", "Craig", "Sheriff",
                   "Jill", "Jillian"};

    char *arr2[] = {"Kyle", "Jacob", "Chante", "Tim", "Craig", "Sheriff",
                    "Jill", "Jillian"};

    size_t arr_size = sizeof(arr) / sizeof(arr[0]);

    tree *tree = tree_create((int (*)(void *, void *)) person_compare,
                             (void (*)(void *)) print);
    int ret = 0;
    for (size_t i = 0; i < arr_size; ++i) {
        ret = tree_insert(tree, (void *) arr[i]);
        if (!ret) {
            return 1;
        }
    }
    printf("Arr Size: %zu\n", arr_size);

    printf("Preorder: ");
    preorder(tree);
    printf("Postorder: ");
    postorder(tree);
    printf("Inorder: ");
    inorder(tree);
    printf("Levelorder: ");
    levelorder(tree);

    printf("Size: %d\n", tree_size(tree));

    void *sizes = tree_maximum(tree);
    if (sizes) {
        printf("Tree Max: %s\n", (char *) sizes);
    } else {
        printf("Tree Max: 0\n");
    }
    sizes = tree_minimum(tree);
    if (sizes) {
        printf("Tree Min: %s\n", (char *) sizes);
    } else {
        printf("Tree Min: 0\n");
    }

    print_visual(tree);

    ret = 0;
    for (int i = 0; i < arr_size; ++i) {

        ret = tree_delete(&tree, (void *) arr2[i]);

        if (ret) {
            printf("Deleted: %s\n", arr2[i]);
            print_visual(tree);
        }
    }

    print_visual(tree);
    printf("Size: %d\n", tree_size(tree));

    tree_destroy(&tree);
}