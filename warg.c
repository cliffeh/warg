#include "warg.h"

int
warg_context_init (warg_context *ctx, const warg_opt *opts, int argc,
                   const char *argv[])
{
  // TODO strip path (basename?)
  ctx->progname = argv[0];
  ctx->opts = opts;
  ctx->argc = argc;
  ctx->argv = argv;

  // TODO can we always assume that argv[0] is progname?
  ctx->curr = 1;

  return 0;
}

int
warg_print_help (FILE *out, const warg_context *ctx)
{
  fprintf (out, "usage: %s [OPTION...]\n", ctx->progname);
  for (int i = 0; ctx->opts[i].description; i++)
    {
      fprintf (out, "  -%c, --%s    %s\n", ctx->opts[i].shortopt,
               ctx->opts[i].longopt, ctx->opts[i].description);
    }

  return 0;
}
