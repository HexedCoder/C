/** @file llist_check.c
 *
 * @brief llist_check.c tests public functions from llist.h
 *
 * COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
 *
 */

#ifndef LLIST_CHECK_H
#define LLIST_CHECK_H

#include <check.h>
#include "../src/llist.h"

void word_print (void *value) {
    if (!value) {
        return;
    }

    char *word = (char *) value;

    printf ("%s\n", word);
}

START_TEST(test_linked_list_ops) {
        printf ("-----LLIST----\n");

        printf ("Start: test_linked_list_ops\n");
        llist_t *list = llist_create();

        char *word_list[] = { (char *)"Word", (char *)"Word2", (char *)"Word3", (char *)"Word4", (char *)"Word5" };

        printf ("\tTest: llist_enqueue/get_size\n");
        for (size_t i = 0; i < 5; ++i) {
            llist_enqueue(list, word_list[i]);

            // null inputs should not append
            llist_enqueue(list, NULL);
            ck_assert(get_size(list) == i + 1);
        }

        printf ("\tTest: llist_pop/get_size\n");
        for (size_t i = 5; i > 0; --i) {
            llist_pop(list);
            ck_assert(get_size(list) == i - 1);
        }

        printf ("\tTest: llist_print (empty list)\n");
        // Nothing should print
        llist_print(list, word_print);

        for (size_t i = 0; i < 5; ++i) {
            // null inputs should not append
            llist_enqueue(list, NULL);
            llist_enqueue(list, word_list[i]);
        }

        printf ("\tTest: llist_print/llist_clear/get_size\n");
        // each element should print on its own line
        llist_print(list, word_print);
        llist_clear(list);
        ck_assert(get_size(list) == 0);

        printf ("End: test_linked_list_ops\n");

        printf ("-----END LLIST----\n\n");
}

END_TEST

#endif //LLIST_CHECK_H
