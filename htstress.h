#ifndef HTSTRESS_H
#define HTSTRESS_H

#include <curl/curl.h>

/* Main arguments status struct */
typedef struct prog_args {
  int verbose;
  int nthreads;
  int freq;
  char* url;
} prog_args;

prog_args* prog_args_init();

/* callbacks */
typedef void (*pre_func)(CURL*);
typedef size_t (*resp_func)(char*, size_t, size_t, void*);

/* fetch_url thread arguments */
typedef struct thread_args {
  char* url; /* URL to request */
  int req_count; /* Number of times to run request (0 runs forever) */
  pre_func pre_callback; /* function to call before each request */
  resp_func resp_callback; /* Function to handle response data */
} thread_args;

thread_args* thread_args_init();
thread_args** thread_args_array_init(int n);

#endif

