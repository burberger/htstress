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

/* callbacks */
typedef void (*pre_func)(CURL*);
typedef size_t (*resp_func)(char*, size_t, size_t, void*);

/* fetch_url thread arguments */
typedef struct thread_args {
  char* url;
  pre_func pre_callback;
  resp_func resp_callback;
} thread_args;

#endif
