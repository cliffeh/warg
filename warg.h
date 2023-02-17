#ifndef __WARG_H
#define __WARG_H 1

#include <stdio.h>

// TODO for future use (line wrapping)
#ifndef WARG_MAX_LINE_LENGTH
#define WARG_MAX_LINE_LENGTH 120
#endif

#ifndef WARG_MAX_FLAG_STRING_LENGTH
#define WARG_MAX_FLAG_STRING_LENGTH 30
#endif

typedef struct warg_opt
{
  const char *longopt;
  char shortopt;
  const char *argname;
  const char *defaultarg;
  const char *description;
} warg_opt;

typedef struct warg_context
{
  const char *progname;
  const warg_opt *opts;
  int argc, curr;
  const char **argv;
} warg_context;

int warg_context_init (warg_context *ctx, const warg_opt *opts, int argc,
                       const char *argv[]);
int warg_next_arg (warg_context *ctx);
int warg_print_help (FILE *out, const warg_context *ctx);

#endif
