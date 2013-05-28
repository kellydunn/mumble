#include <stdlib.h>
#include <check.h>
#include "../../src/types.h"
#include "../../src/mumble/config_node.h"

// Ensure a new config_node can be made
// ✔ Node should not be NULL
// ✔ Key should be NULL
// ✔ Value should be NULL
START_TEST (test_new_config_node) {
  mumble_config_node_t * node = new_config_node();

  fail_if(node == NULL, "Node is null");
  fail_if(node->key != NULL, "Unexpected `key` value.");
  fail_if(node->value != NULL, "Unexpected `value` value");
} END_TEST

// Ensure a single value is appended to a list as expected
// ✔ Appending once should result in a non-NULL key value
START_TEST (test_config_node_append_data) {
  mumble_config_node_t * node = new_config_node();

  config_node_append_data(node, "test-key");
  
  fail_if(node->key != "test-key", "Unexpected `key` value.");
  fail_if(node->value != NULL, "Unexpected `value` value.");
} END_TEST

// Ensure a single value is appended to a list as expected
// ✔ Appending once should result in a non-NULL key value
START_TEST (test_config_node_append_data2) {
  mumble_config_node_t * node = new_config_node();

  config_node_append_data(node, "test-key");
  config_node_append_data(node, "test-value");
  
  fail_if(node->key != "test-key", "Unexpected `key` value.");
  fail_if(node->value != "test-value", "Unexpected `value` value.");
} END_TEST

// Ensure a single value is appended to a list as expected
// ✔ Head should be updated
// ✔ Tail should be updated
// ✔ Data should be the same
START_TEST (test_config_node_is_complete) {
} END_TEST

Suite * new_config_node_suite() {
  Suite * s = suite_create("config_node");

  TCase * tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_new_config_node);
  tcase_add_test(tc_core, test_config_node_append_data);
  tcase_add_test(tc_core, test_config_node_append_data2);
  //tcase_add_test(tc_core, test_config_node_is_complete);

  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  // Run config_node_suite
  int num_failed;

  Suite * config_node_suite = new_config_node_suite();
  SRunner * config_node_suite_runner = srunner_create(config_node_suite);

  srunner_run_all(config_node_suite_runner, CK_NORMAL);
  num_failed = srunner_ntests_failed(config_node_suite_runner);
  
  srunner_free(config_node_suite_runner);

  return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
