#include <argp.h>
#include <stdlib.h>
#include <curl/curl.h>

const char *argp_program_version = "htstress 0.1";
const char *argp_program_bug_address = "<burberger@gmail.com>";
static char doc[] =
  "htstress -- A stress tester for http/https services";

static struct argp argp = { 0, 0, 0, doc };

int main(int argc, char **argv) {
  // Arguments parsing
  argp_parse(&argp, argc, argv, 0, 0, 0);

  // CURL Setup
  CURL *curl;
  CURLcode resp;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  return 0;
}
