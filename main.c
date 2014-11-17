#include <argp.h>
#include <err.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "htstress.h"

/* Program documentation */
const char *argp_program_version = "htstress 0.1";
const char *argp_program_bug_address = "<burberger@gmail.com>";

static char doc[] =
  "htstress -- A stress tester for http/https services";

static char args_doc[] = "server:port";

/* Accepted arguments */
static struct argp_option options[] = {
  { "nthreads", 'n', "TC", 0, "Number of threads to run" },
  { "verbose",  'v', 0,    0, "Output additional request info" },
  { 0 }
};

/* Parser */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct args *args = state->input;

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

int main(int argc, char **argv) {
  /* Arguments parsing */
  struct args args;
  args.verbose = 0;
  args.nthreads = 0;
  args.url = 0;

  argp_parse(&argp, argc, argv, 0, 0, &args);

  /* CURL Setup */
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, args.url);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      fprintf(stderr, "Request failed: %s", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  return 0;
}
