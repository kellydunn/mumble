#include <stdlib.h>
#include <check.h>
#include "../../src/types.h"
#include "../../src/mumble/list.h"

// Ensure a new list can be made
// - Head should be NULL
// - Tail should be NULL
START_TEST (test_new_list) {
  mumble_list_t * list = (mumble_list_t *) new_list();
  fail_if(list->head != NULL, "Unexpected list head value");
  fail_if(list->tail != NULL, "Unexpected list tail value");
  fail_if(list->size != 0, "Unexpected list size");
} END_TEST

// Ensure a single value is appended to a list as expected
// - Head should be updated
// - Tail should be updated
// - Data should be the same
START_TEST (test_append_list) {
  mumble_list_t * list = (mumble_list_t *) new_list();
  char * buf = "test data";
  list_append(list, buf);
  
  fail_if(list->head == NULL, "Head is NULL");
  fail_if(list->tail == NULL, "Tail is NULL");

  fail_if(list->head->data != list->tail->data, "Head and Tail data have diverged");
  fail_if(list->head->data != "test data", "Unexecpted list node value");

  fail_if(list->size != 1, "Unexpected list size");
} END_TEST

// Ensure a single value is appended to a list as expected
// - Head should be updated
// - Tail should be updated
// - Data should be the same
START_TEST (test_append_2_list) {
  mumble_list_t * list = (mumble_list_t *) new_list();
  char * buf = "test data";
  list_append(list, buf);

  char * buf2 = "test data2";
  list_append(list, buf2);
  
  fail_if(list->head == NULL, "Head is NULL");
  fail_if(list->tail == NULL, "Tail is NULL");

  fail_if(list->head->data == list->tail->data, "Head and Tail data have not diverged");
  fail_if(list->head->data != "test data", "Unexecpted head node value");
  fail_if(list->tail->data != "test data2", "Unexecpted tail node value");

  fail_if(list->size != 2, "Unexpected list size");
} END_TEST

Suite * new_list_suite() {
  Suite * s = suite_create("list");

  TCase * tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_new_list);
  tcase_add_test(tc_core, test_append_list);
  tcase_add_test(tc_core, test_append_2_list);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int num_failed;
  
  Suite * list_suite = new_list_suite();
  SRunner * list_suite_runner = srunner_create(list_suite);
  
  srunner_run_all(list_suite_runner, CK_NORMAL);

  num_failed = srunner_ntests_failed(list_suite_runner);

  srunner_free(list_suite_runner);

  return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
