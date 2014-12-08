#include <stdlib.h>
#include "htstress.h"

/* Struct initializer functions */

prog_args* prog_args_init() {
  prog_args *args = malloc(sizeof(prog_args));
  args->verbose = 0;
  args->nthreads = 1; /* program defaults to running with 1 thread */
  args->freq = 0;
  args->url = 0;
  return args;
}

thread_args* thread_args_init() {
  thread_args *args = malloc(sizeof(thread_args));
  args->url = 0;
  args->req_count = 0;
  args->pre_callback = 0;
  args->resp_callback = 0;
  return args;
}

/* Tests */

thread_args** make_test_table() {
  thread_args* test_urls[] = {
    { "new_ssl_stamps.php", 1, new_ssl_stamps_pre, new_ssl_stamps_post },
    { "find_ssl_user.php", 1, find_ssl_user_pre, find_ssl_user_post },
    { "find_ssl_stamps.php", 1, find_ssl_stamps_pre, find_ssl_stamps_post },
    { 0 }
  };
  return array;
}
