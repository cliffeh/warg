#include "warg.h"

#include <string.h>

#ifndef max
#define max(a, b) ((a > b) ? a : b)
#endif

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

#define IS_STOPOPT(p) (((*((p) + 0) == '-') && ((*((p) + 1) == '-')) && ((*((p) + 2) == 0))))
#define IS_LONGOPT(p) (((*((p) + 0) == '-') && ((*((p) + 1) == '-')) && ((*((p) + 2) != 0))))
#define IS_SHORTOPT(p) (((*((p) + 0) == '-') && ((*((p) + 1) != '-'))))

static const warg_opt *
warg_find_longopt (const warg_opt *opts, const char *longoptname)
{ // note: longoptname may contain trailing =argument
  char *p = 0;
  for (int i = 0; !AT_WARG_TABLE_END (opts[i]); i++)
    {
      // some opts might not have a long version
      if (opts[i].longopt)
        {
          int len = strlen (opts[i].longopt);
          if (strncmp (opts[i].longopt, longoptname, len) == 0
              && (!*(longoptname + len) || *(longoptname + len) == '='))
            return &opts[i];
        }
    }
  return 0;
}

static const warg_opt *
warg_find_shortopt (const warg_opt *opts, char shortopt)
{
  for (int i = 0; !AT_WARG_TABLE_END (opts[i]); i++)
    {
      if (shortopt == opts[i].shortopt)
        return &opts[i];
    }
  return 0;
}

static int
warg_opt_string (char *buf, const warg_opt *opt)
{
  int len = 0;
  char tmp[1024];
  char *p = buf ? buf : tmp;

  // TODO the below is probably not a great assumption; modify this so the
  // option can have either or both there will always be a longopt, there may
  // not always be a shortopt
  if (opt->shortopt)
    len += sprintf (p + len, "-%c, --%s", opt->shortopt, opt->longopt);
  else
    len += sprintf (p + len, "    --%s", opt->longopt);

  if (opt->argname)
    {
      len += sprintf (p + len, "=%s", opt->argname);
    }

  return len;
}

static int
warg_set_argument (const warg_opt *opt, const char *arg)
{
  switch (opt->type)
    {
    case WARG_TYPE_STRING:
      {
        // TODO make sure the whole thing printed?
        return sprintf (opt->store, "%s", arg);
      }
      break;
    case WARG_TYPE_INT:
      {
        // we need a temporary pointer in case of error
        int len = 0, value = 0;

        int is_negative = 0;
        if (*arg == '-')
          {
            is_negative = 1;
            len++;
          }

        while (*(arg + len))
          {
            switch (*(arg + len))
              {
              // clang-format off
              case '0': case '1': case '2': case '3': case '4':
              case '5': case '6': case '7': case '8': case '9':
                // clang-format on
                {
                  value *= 10;
                  value += (*(arg + len) - '0');
                  len++;
                }
                break;
              default:
                {
                  return WARG_ERROR_EXPECTED_INT;
                }
              }
          }
        if (is_negative)
          {
            *((int *)opt->store) *= -1;
          }
        *((int *)opt->store) = value;

        return len;
      }
    default:
      return WARG_ERROR_UNKNOWN_ARGUMENT_TYPE;
    }
}

int
warg_context_init (warg_context *ctx, const warg_opt *opts, int argc, const char *argv[])
{
  // TODO allow setting preamble/postable (for help string)
  // TODO use basename?
  // TODO allow setting progname explicitly?

  // find the last unescaped /
  int lastslash = 0, len = strlen (argv[0]);
  for (int i = 0; i < len; i++)
    {
      if (argv[0][i] != '\\' && i + 1 < len && argv[0][i + 1] == '/')
        lastslash = i + 2;
    }

  ctx->progname = argv[0] + lastslash;
  ctx->opts = opts;
  ctx->argc = argc;
  ctx->argv = argv;

  for (int i = 0; i < ctx->ea; i++)
    {
      ctx->extra_args[i] = 0;
    }
  ctx->ea = 0;
  ctx->stop = 0;
  // we want to start processing args after the program name
  ctx->curr = 1;
  ctx->ptr = 0;

  // TODO sanity checks? e.g., argc > max args?
  return 0;
}

int
warg_next_option (warg_context *ctx)
{
  while (ctx->curr < ctx->argc) // while we still have args to process
    {
      if (IS_STOPOPT (ctx->argv[ctx->curr]))
        {
          // we're done; let's pack everything else into extra_args
          while (++ctx->curr < ctx->argc)
            {
              ctx->extra_args[ctx->ea++] = ctx->argv[ctx->curr];
            }
          return -1;
        }

      if (IS_LONGOPT (ctx->argv[ctx->curr]))
        {
          ctx->ptr = ctx->argv[ctx->curr] + 2; // skip past the --
          const warg_opt *opt = warg_find_longopt (ctx->opts, ctx->ptr);
          if (!opt)
            return WARG_ERROR_UNKNOWN_OPTION;

          // now that we've located the option, we can figure out what
          // we're supposed to do with it
          ctx->ptr += strlen (opt->longopt);

          if (opt->argname)
            { // we're expecting an argument
              int len = 0;
              if (*ctx->ptr == '=')
                { // our argument will be the bit after the =
                  ctx->ptr++;
                  len = warg_set_argument (opt, ctx->ptr);
                }
              else if (!*ctx->ptr)
                { // our argument will be the next arg
                  if (++ctx->curr == ctx->argc)
                    return WARG_ERROR_ARGUMENT_NOT_FOUND;
                  len = warg_set_argument (opt, ctx->argv[ctx->curr]);
                }

              if (len < 0) // error!
                return len;

              // advance curr, clear our pointer, and return
              ctx->curr++;
              ctx->ptr = 0;
              return opt->shortopt;
            }
          else
            { // we're not expecting an argument
              if (*ctx->ptr == '=')
                return WARG_ERROR_UNEXPECTED_ARGUMENT;

              // TODO collapse this logic into warg_set_argument?
              if (opt->store)
                { // if storage has been provided but no argument is
                  // required...
                  switch (opt->type)
                    {
                    case WARG_TYPE_INT:
                      { // default behavior: increment
                        (*(int *)(opt->store))++;
                      }
                      break;
                    case WARG_TYPE_STRING:
                      { // default behavior: append shortopt (and
                        // null-terminate)
                        int len = strlen (((char *)(opt->store)));
                        // TODO if shortopt hasn't been set, is this actually
                        // the behavior we want?
                        ((char *)(opt->store))[len] = opt->shortopt;
                        ((char *)(opt->store))[len + 1] = 0;
                      }
                      // TODO default: toss an error for unknown types?
                    }
                }
              // advance curr, clear our pointer, and return
              ctx->curr++;
              ctx->ptr = 0;
              return opt->shortopt;
            }
        }

      if (IS_SHORTOPT (ctx->argv[ctx->curr]))
        {
          if (!ctx->ptr)
            ctx->ptr = ctx->argv[ctx->curr] + 1;
          // either ctx->ptr has been reset and is pointing at the character
          // after the '-', or we've already consumed argument-less shortopts
          // prior to it
          const warg_opt *opt = warg_find_shortopt (ctx->opts, *ctx->ptr);
          if (!opt)
            { // oops, we got an unknown option!
              return WARG_ERROR_UNKNOWN_OPTION;
            }

          if (opt->argname)
            { // we're expecting an argument
              if (!*(++ctx->ptr))
                { // if it's not elided into this option, we'll look at the
                  // next position
                  if (ctx->curr + 1 == ctx->argc)
                    { // if we're out of args, no argument has been provided!
                      return WARG_ERROR_ARGUMENT_NOT_FOUND;
                    }
                  // otherwise we'll advance to consume the next arg
                  ctx->ptr = ctx->argv[++ctx->curr];
                }
              int len = warg_set_argument (opt, ctx->ptr);
              if (len < 0) // error!
                return len;

              // advance curr, clear our pointer, and return
              ctx->curr++;
              ctx->ptr = 0;
              return opt->shortopt;
            }
          else
            {
              if (opt->store)
                { // if storage has been provided but no argument is
                  // required...
                  // TODO collapse this logic into warg_set_argument?
                  switch (opt->type)
                    {
                    case WARG_TYPE_INT:
                      { // default behavior: increment
                        (*(int *)(opt->store))++;
                      }
                      break;
                    case WARG_TYPE_STRING:
                      { // default behavior: append shortopt (and
                        // null-terminate)
                        int len = strlen (((char *)(opt->store)));
                        ((char *)(opt->store))[len] = opt->shortopt;
                        ((char *)(opt->store))[len + 1] = 0;
                      }
                      // TODO default: toss an error for unknown types?
                    }
                }
              // advance our pointer, possibly meaning moving to the next arg
              if (!*(++ctx->ptr))
                {
                  ctx->curr++;
                  ctx->ptr = 0;
                }
              return opt->shortopt;
            }
        }
      else
        { // this isn't an option, it's an extra argument
          ctx->extra_args[ctx->ea++] = ctx->argv[ctx->curr++];
        }
    }

  // if we're here then we've run out of arguments to process
  return WARG_OK;
}

const char *
warg_current_option (warg_context *ctx)
{
  return ctx->argv[ctx->curr];
}

const char **
warg_extra_args (warg_context *ctx)
{
  return ctx->extra_args;
}

int
warg_print_help (FILE *out, warg_context *ctx)
{
  char buf[1024];
  int longest = 0; // longest option string
  fprintf (out, "Usage: %s [OPTION...]\n", ctx->progname);
  fprintf (out, "Options:\n");

  for (int i = 0; !AT_WARG_TABLE_END (ctx->opts[i]); i++)
    {
      longest = max (longest, warg_opt_string (0, &ctx->opts[i]));
    }

  longest = min (longest, WARG_MAX_OPT_STRING_LENGTH);

  for (int i = 0; !AT_WARG_TABLE_END (ctx->opts[i]); i++)
    {
      int len = warg_opt_string (buf, &ctx->opts[i]);

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
                if (*((char *)ctx->opts[i].store))
                  fprintf (out, " (default: %s)", ((char *)ctx->opts[i].store));
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

void
warg_print_error (FILE *out, warg_context *ctx, int rc)
{
  switch (rc)
    {
    case WARG_ERROR_UNKNOWN_OPTION:
      {
        fprintf (out, "error: unknown option: %s\n", warg_current_option (ctx));
        warg_print_help (out, ctx);
      }
      break;
    case WARG_ERROR_ARGUMENT_NOT_FOUND:
      {
        fprintf (out, "error: option %s requires an argument\n", warg_current_option (ctx));
        warg_print_help (out, ctx);
      }
      break;
    case WARG_ERROR_UNEXPECTED_ARGUMENT:
      {
        fprintf (out, "error: option %s does not take an argument\n", warg_current_option (ctx));
        warg_print_help (out, ctx);
      }
      break;
    case WARG_ERROR_EXPECTED_INT:
      {
        fprintf (out, "error: option %s expects an integer argument\n", warg_current_option (ctx));
        warg_print_help (out, ctx);
      }
      break;
    case WARG_ERROR_UNKNOWN_ARGUMENT_TYPE:
      {
        fprintf (out, "error: option %s expects an unknown argument type\n", warg_current_option (ctx));
        warg_print_help (out, ctx);
      }
    default:
      {
        fprintf (out, "error: unknown error while parsing command line options\n");
      }
    }
}
