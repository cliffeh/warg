#include "warg.h"

#include <string.h>

#ifndef max
#define max(a, b) ((a > b) ? a : b)
#endif

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

static const warg_opt *
warg_find_longopt (const warg_context *ctx, const char *longoptname)
{
  for (int i = 0; ctx->opts[i].longopt; i++)
    {
      if (strcmp (longoptname, ctx->opts[i].longopt) == 0)
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
  ctx->curr = 0;
  // initialize ctx->ptr to the null character at the end of the program name
  ctx->ptr = argv[0];
  while (*ctx->ptr)
    ctx->ptr++;

  return 0;
}

const char *
warg_current_option (warg_context *ctx)
{
  return ctx->ptr;
}

int
warg_next_option (warg_context *ctx)
{
  if (ctx->stop)
    return -1;

  if (!*ctx->ptr)
    {
      ctx->curr++;
      if (ctx->curr >= ctx->argc)
        { // we're done!
          ctx->stop = 1;
          return -1;
        }
      else
        { // advance our pointer
          ctx->ptr = ctx->argv[ctx->curr];
        }
    }

  if (*ctx->ptr == '-')
    { // we have an option
      ctx->ptr++;
      if (*ctx->ptr == '-')
        { // either a long option or a stop directive
          ctx->ptr++;
          if (!*ctx->ptr)
            { // -- means stop processing options
              ctx->stop = 1;
              return -1;
            }
          else
            { // longopt
              // TODO standardize on length limit?
              char longoptname[1024];
              int i = 0;
              for (const char *p = ctx->ptr; *p && *p != '='; p++)
                {
                  longoptname[i++] = *p;
                }
              longoptname[i] = 0;

              const warg_opt *opt = warg_find_longopt (ctx, longoptname);
              if (!opt)
                {
                  ctx->ptr -= 2;
                  return WARG_UNKNOWN_OPTION;
                }

              ctx->ptr += strlen (longoptname);
              // now that we've located the option, we can figure out what
              // we're supposed to do with it

              // TODO set arg!
            }
        }
      else
        { // shortopt
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
      fprintf (out, "%s", ctx->opts[i].description);

      if (ctx->opts[i].argname)
        {
          switch (ctx->opts[i].type)
            {
            case WARG_TYPE_STRING:
              {
                if (*(char *)ctx->opts[i].store)
                  fprintf (out, " (default: %s)", (char *)ctx->opts[i].store);
              }
              break;
            case WARG_TYPE_INT:
              {
                fprintf (out, " (default: %i)", *((int *)ctx->opts[i].store));
              }
              break;
              // default: don't print a default
            }
        }

      fprintf (out, "\n");
    }
  return 0;
}
