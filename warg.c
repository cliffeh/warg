#include "warg.h"

#ifndef max
#define max(a, b) ((a > b) ? a : b)
#endif

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

static int
warg_flag_string (char *buf, const warg_opt *opt)
{
  int len = 0;
  char tmp[1024];
  char *p = buf ? buf : tmp;

  len += sprintf (p, "-%c, --%s", opt->shortopt, opt->longopt);
  if (opt->argname)
    {
      if (*opt->argname == '?')
        {
          len += sprintf (p + len, "[=%s]", opt->argname + 1);
        }
      else
        {
          len += sprintf (p + len, "=%s", opt->argname);
        }
    }
  return len;
}

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
  char buf[1024];
  int longest = 0; // longest flag string
  fprintf (out, "Usage: %s [OPTION...]\n", ctx->progname);
  fprintf (out, "Options:\n");

  for (int i = 0; ctx->opts[i].longopt; i++)
    {
      longest = max (longest, warg_flag_string (0, &ctx->opts[i]));
    }

  longest = min (longest, WARG_MAX_FLAG_STRING_LENGTH);

  for (int i = 0; ctx->opts[i].longopt; i++)
    {
      int len = warg_flag_string (buf, &ctx->opts[i]);

      fprintf (out, "  %-*s", longest, buf);
      if (len > longest)
        fprintf (out, "\n%-*s  ", longest, "  ");
      printf ("%s", ctx->opts[i].description);
      if (ctx->opts[i].defaultarg)
        {
          fprintf (out, " (default: %s)", ctx->opts[i].defaultarg);
        }

      fprintf (out, "\n");
    }

  return 0;
}
