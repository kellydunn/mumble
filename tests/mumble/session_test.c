#include <stdlib.h>
#include <check.h>
#include "../../src/types.h"
#include "../../src/mumble/session.h"

// Ensure a new session can be made with the expected state
// ✔ Session should be false
// ✔ Recording should be false
START_TEST (test_new_session) {
  mumble_session_t * session = new_session();

  fail_if(session == NULL, "Session is null");
  fail_if(session->recording == true, "Incorrect session state: it should not be recording upon initialization.");
} END_TEST

Suite * new_session_suite() {
  Suite * s = suite_create("session");

  TCase * tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_new_session);

  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  // Run session_suite
  int num_failed;

  Suite * session_suite = new_session_suite();
  SRunner * session_suite_runner = srunner_create(session_suite);

  srunner_run_all(session_suite_runner, CK_NORMAL);
  num_failed = srunner_ntests_failed(session_suite_runner);
  
  srunner_free(session_suite_runner);

  return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
