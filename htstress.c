#include <stdlib.h>
#include "htstress.h"

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
  args->pre_callback = 0;
  args->resp_callback = 0;
  return args;
}
