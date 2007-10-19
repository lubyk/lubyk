#include "rubyk.h"

static print_error_message(char * message) {
  printf(message);
}

void
rk_error     (char * message) {
  print_error_message(message);
}

void
rk_sys_error (char * message) {
  // FIXME: get error, ...
  print_error_message(message);
}

void
rk_die       (char * message) {
  print_error_message(message);
  exit(EXIT_FAILURE);
}