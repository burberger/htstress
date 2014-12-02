#include <argp.h>
#include <curl/curl.h>
#include <err.h>
#include <pthread.h>
#include <stdlib.h>
#include "htstress.h"

/* Global interrupt indicator */
static int running = 1;

/* Program documentation */
const char *argp_program_version = "htstress 0.1";
const char *argp_program_bug_address = "<burberger@gmail.com>";

static char doc[] =
  "htstress -- A stress tester for http/https services";

static char args_doc[] = "address:port";

/* Accepted arguments */
static struct argp_option options[] = {
  { "nthreads",  'n', "TC",   0, "Number of threads to run" },
  { "frequency", 'f', "FREQ", 0, "Rate to run requests at in hertz (integers \
    only).  Default is 0 (maximum runnable)" },
  { "verbose",   'v', 0,      0, "Output additional request info" },
  { 0 }
};

/* Parser */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct prog_args *args = state->input;

  switch (key) {
  /* handlers for switches defined in the options array */
  case 'v':
    args->verbose = 1;
    break;
  case 'n':
    args->nthreads = atoi(arg);
    if (args->nthreads < 1 || args->nthreads > 256) {
      errx(1, "Invalid thread count");
    }
    break;
  case 'f':
    args->freq = atoi(arg);
    if (args->freq < 0) {
      errx(1, "Invalid frequency");
    }

  /* Argp special states */
  case ARGP_KEY_ARG:
    break;
  case ARGP_KEY_NO_ARGS:
    argp_usage(state);
    break;
  case ARGP_KEY_END:
    args->url = state->argv[state->argc-1];
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

/* drop_response:
 * do nothing function that discards the response from the server
 */
size_t drop_response(char *ptr, size_t size, size_t nmemb, void *userdata) {
  return size * nmemb;
}

/* fetch_url:
 * url: string of url to fetch
 * pre_callback: function to call before making request
 * resp_callback: function to call to process response data, see
 *                CURLOPT_WRITEFUNCTION
 * Fetches provided URL at specified frequency, checking response with
 * parser provided as argument
 */
void *fetch_url(void *t_args) {
  thread_args *args = (thread_args*) t_args;
  CURL *curl;
  curl = curl_easy_init();
  if (!curl) {
    errx(1, "curl could not initialize!");
  }
  curl_easy_setopt(curl, CURLOPT_URL, args->url);

  /* check for response callback, otherwise discard response */
  if (args->resp_callback) {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, args->resp_callback);
  } else {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &drop_response);
  }

  /* Request loop */
  CURLcode res;
  while (running) {
    if (args->pre_callback) {
      args->pre_callback(curl);
    }
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      fprintf(stderr, "Request failed to %s: %s\n", args->url, curl_easy_strerror(res));
    }
  }
  curl_easy_cleanup(curl);
  pthread_exit(0);
}

int main(int argc, char **argv) {
  /* Arg parsing and init */
  prog_args *args = prog_args_init();
  thread_args *t_args = thread_args_init();
  argp_parse(&argp, argc, argv, 0, 0, args);

  t_args->url = args->url;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  /* create threads and hand out work */
  int i, err;
  pthread_t tid[args->nthreads];
  for (i = 0; i < args->nthreads; i++) {
    err = pthread_create(&tid[i], NULL, &fetch_url, t_args);
    if (err) {
      errx(err, "Could not create threads!");
    }
  }

  /* wait for all threads to terminate */
  for (i = 0; i < args->nthreads; i++) {
    err = pthread_join(tid[i], NULL);
  }

  free(args);
  free(t_args);
  curl_global_cleanup();
  printf("Program exited successfully.\n");

  return 0;
}

