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

  fail_if(list->head->next != list->tail, "Incorrect next node for Head");
  fail_if(list->tail->prev != list->head, "Incorrect prev node for Tail");

  fail_if(list->size != 2, "Unexpected list size");
} END_TEST

// Ensure a single value can be appended then popped off of a list
// - Data should be as expected
// - Size should be 0 
START_TEST (test_append_1_pop_1_list) {
  mumble_list_t * list = (mumble_list_t *) new_list();
  char * buf = "test data";

  list_append(list, buf);

  mumble_list_node_t * tmp = (mumble_list_node_t *) list_pop(list);

  fail_if(tmp == NULL, "Returned value is null");
  fail_if(tmp->data != "test data", "Unexpected data value for returned list node");

  fail_if(list->size != 0, "Unexpected list size");
  fail_if(!is_empty(list), "List doesn't report as empty");
} END_TEST

// Ensure a single value can be appended then popped off of a list
// - Data should be as expected
// - Size should be 1
// - Head should be the same as tail 
START_TEST (test_append_2_pop_1_list) {
  mumble_list_t * list = (mumble_list_t *) new_list();
  char * buf = "test data";
  char * buf2 = "test data2";

  list_append(list, buf);
  list_append(list, buf2);

  mumble_list_node_t * tmp = (mumble_list_node_t *) list_pop(list);

  fail_if(tmp == NULL, "Returned value is null");
  fail_if(tmp->data != "test data2", "Unexpected data value for returned list node");

  fail_if(list->head->data != list->tail->data, "Head and Tail have diverged");
  
  fail_if(list->size != 1, "Unexpected list size");
  fail_if(is_empty(list), "List reports itself as empty");
} END_TEST

// Ensure a single value can be appended then popped off of a list
// - Data should be as expected
// - Size should be 0
START_TEST (test_append_2_pop_2_list) {
  mumble_list_t * list = (mumble_list_t *) new_list();
  char * buf = "test data";
  char * buf2 = "test data2";

  list_append(list, buf);
  list_append(list, buf2);

  mumble_list_node_t * tmp = (mumble_list_node_t *) list_pop(list);
  mumble_list_node_t * tmp2 = (mumble_list_node_t *) list_pop(list);

  fail_if(tmp == NULL, "Returned value is null");
  fail_if(tmp->data != "test data2", "Unexpected data value for returned list node");
  fail_if(tmp2->data != "test data", "Unexpected data value for returned list node");

  fail_if(list->size != 0, "Unexpected list size");
  fail_if(!is_empty(list), "List does not report itself as empty");
} END_TEST

Suite * new_list_suite() {
  Suite * s = suite_create("list");

  TCase * tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_new_list);

  tcase_add_test(tc_core, test_append_list);
  tcase_add_test(tc_core, test_append_2_list);

  tcase_add_test(tc_core, test_append_1_pop_1_list);
  tcase_add_test(tc_core, test_append_2_pop_1_list);
  tcase_add_test(tc_core, test_append_2_pop_2_list);

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
