#include <stdlib.h>
#include <check.h>
#include "../../src/types.h"
#include "../../src/mumble/session.h"
#include "../../src/mumble/loop.h"

// Ensure a new loop can be created in the expected state
// ✔ Loop should not be null
// ✔ Loop should not indicate that it's currently looping
// ✔ Loop should not have a null list of events
// ✔ Loop should have an empty list of events
// ✔ Loop should have the correct assocation of a session
// ✔ Loop should have have a zero-value for duration
START_TEST (test_new_loop) {
  mumble_session_t * session = new_session();
  mumble_loop_t * loop = new_loop(session);

  fail_if(loop == NULL, "Loop is null/");
  fail_if(loop->looping, "Loop should not indicate that it is looping upon initialization.");
  fail_if(loop->events == NULL, "Events list should not be NULL.");
  fail_if(!is_empty(loop->events), "Events is incorrectly populated upon initialization.");
  fail_if(loop->session == NULL, "Session relation is null.");
  fail_if(loop->session != session, "Incorrect session relation.");
  fail_if(loop->duration != 0, "Unexceptable value for duration.");

} END_TEST


// Ensure a loop can correctly add a midi event.
// ✔ Loop events should not be empty
START_TEST (test_add_event) {
  // TODO This test should also ensure that the event we add is the correct event.
  mumble_session_t * session = new_session();
  mumble_loop_t * loop = new_loop(session);

  // TODO dang, I don't have a new_* for this struct :(  #fail
  //      Lets revisit this later.
  mumble_midi_event_t * event = calloc(1, sizeof(mumble_midi_event_t));

  add_midi_event(loop, event);

  fail_if(is_empty(loop->events), "Loop is incorrectly empty");

} END_TEST

Suite * new_loop_suite() {
  Suite * s = suite_create("loop");

  TCase * tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_new_loop);
  tcase_add_test(tc_core, test_add_event);

  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  // Run loop_suite
  int num_failed;

  Suite * loop_suite = new_loop_suite();
  SRunner * loop_suite_runner = srunner_create(loop_suite);

  srunner_run_all(loop_suite_runner, CK_NORMAL);
  num_failed = srunner_ntests_failed(loop_suite_runner);
  
  srunner_free(loop_suite_runner);

  return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
