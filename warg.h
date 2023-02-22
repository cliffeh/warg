#ifndef __WARG_H
#define __WARG_H 1

#include <stdio.h>

// return values
#define WARG_OK -1

// types
#define WARG_TYPE_STRING 0
#define WARG_TYPE_INT 1

// TODO for future use (line wrapping)
#ifndef WARG_MAX_LINE_LENGTH
#define WARG_MAX_LINE_LENGTH 120
#endif

#ifndef WARG_MAX_OPT_STRING_LENGTH
#define WARG_MAX_OPT_STRING_LENGTH 30
#endif

#ifndef WARG_MAX_EXTRA_ARGS
#define WARG_MAX_EXTRA_ARGS 1024
#endif

// can be set to 'h' if you like
#ifndef WARG_HELP_CHAR
#define WARG_HELP_CHAR '?'
#endif

#define WARG_AUTOHELP_HELP                                                    \
  {                                                                           \
    "help", WARG_HELP_CHAR, 0, 0, 0, "show this help message and exit"        \
  }

#define WARG_AUTOHELP_VERSION                                                 \
  {                                                                           \
    "version", 0, 0, 0, 0, "show version information and exit"                \
  }

#ifdef PACKAGE_STRING
#define WARG_AUTOHELP WARG_AUTOHELP_HELP, WARG_AUTOHELP_VERSION
#else
#define WARG_AUTOHELP WARG_AUTOHELP_HELP
#endif

typedef struct warg_opt
{
  const char *longopt;
  char shortopt;
  const char *argname;
  void *store;
  int type;
  const char *description;
} warg_opt;

typedef struct warg_context
{
  const char *progname;
  const warg_opt *opts;
  int argc, curr, stop, ea;
  const char **argv;
  const char *ptr;
  const char *extra_args[WARG_MAX_EXTRA_ARGS];
} warg_context;

int warg_context_init (warg_context *ctx, const warg_opt *opts, int argc,
                       const char *argv[]);
int warg_next_option (warg_context *ctx);
const char **warg_extra_args(warg_context *ctx);
int warg_print_help (FILE *out, const warg_context *ctx);

#endif
