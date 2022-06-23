#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "node/inc/node.h"

#define TCNAME "node"

START_TEST(create_node_tc)
{
    Node *node = NULL;
    node = create_node();
    ck_assert_ptr_ne(node);
    destroy_node(node);
}
END_TEST

Suite *create_suite(void)
{
    Suite *s = suite_create(TCNAME);
    TCase *tc = tcase_create(TCNAME);

    tcase_add_test(tc, create_node_tc);
    suite_add_tcase(s, tc);
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s = create_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
