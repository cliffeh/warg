#include "warg.h"

#include <string.h>

#ifndef max
#define max(a, b) ((a > b) ? a : b)
#endif

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

static const warg_opt *
warg_find_longopt (const warg_context *ctx, const char *longopt, int len)
{
  for (int i = 0; ctx->opts[i].longopt; i++)
    {
      if (strncmp (longopt, ctx->opts[i].longopt, len) == 0)
        return &ctx->opts[i];
    }
  return 0;
}

static const warg_opt *
warg_find_shortopt (const warg_context *ctx, char shortopt)
{
  for (int i = 0; ctx->opts[i].longopt; i++)
    {
      if (shortopt == ctx->opts[i].shortopt)
        return &ctx->opts[i];
    }
  return 0;
}

static int
warg_flag_string (char *buf, const warg_opt *opt)
{
  int len = 0;
  char tmp[1024];
  char *p = buf ? buf : tmp;

  // there will always be a longopt, there may not always be a shortopt
  if (opt->shortopt)
    len += sprintf (p + len, "-%c, --%s", opt->shortopt, opt->longopt);
  else
    len += sprintf (p + len, "    --%s", opt->longopt);

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
  ctx->stop = 0;
  ctx->curr = 1;
  if (argc > 1)
    ctx->ptr = ctx->argv[1];
  else
    ctx->ptr = 0;

  return 0;
}

int
warg_next_arg (warg_context *ctx)
{
  if (!ctx->ptr)
    return -1;

  if (!ctx->stop && *ctx->ptr == '-')
    {
      ctx->ptr++;
      if (*ctx->ptr == '-')
        {
          ctx->ptr++;
          if (*ctx->ptr == 0)
            {
              // -- means stop processing options
              ctx->stop = 1;
              return -1;
            }
          int len = 0;
          for (const char *p = ctx->ptr; *p && *p != '='; p++)
            len++;
          const warg_opt *opt = warg_find_longopt (ctx, ctx->ptr, len);
          printf("found longopt: %s\n", opt->longopt);
        }
      else
        {
          const warg_opt *opt = warg_find_shortopt (ctx, *(ctx->ptr + 1));
        }
    }
  else
    {
      printf ("extra arg\n");
    }

  printf ("ptr: %s\n", ctx->ptr);
  return -1;
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
