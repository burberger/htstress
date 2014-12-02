#include <stdlib.h>
#include "htstress.h"

/* Struct initializer functions */

prog_args* prog_args_init() {
  prog_args *args = malloc(sizeof(prog_args));
  args->verbose = 0;
  args->nthreads = 1;
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

thread_args** thread_args_array_init(int n) {
  thread_args **array = malloc(sizeof(thread_args*) * n);
  for (int i = 0; i < n; i++) {
    array[i] = thread_args_init();
  }
  return array;
}

/* Tests */

