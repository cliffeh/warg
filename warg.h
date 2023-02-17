#ifndef __WARG_H
#define __WARG_H 1

#include <stdio.h>

typedef struct warg_opt
{
  const char *longopt;
  char shortopt;
  const char *description;
} warg_opt;

typedef struct warg_context
{
  const char *progname;
  const warg_opt *opts;
} warg_context;

int warg_context_init (warg_context *ctx, const warg_opt *opts, int argc,
                       char *argv[]);
int warg_print_help (FILE *out, const warg_context *ctx);

#endif
