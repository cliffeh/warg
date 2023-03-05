#include "warg.h"
#include <stdlib.h>

int
main (int argc, const char *argv[])
{
  char infile[4096] = "-", outfile[4096] = "-", zazzle[4096] = "", prefix[4096] = { 0 };
  int indent = 0, max_frobulate = 0, verbose = 0, jumping_jacks = 0, kangaroos = 0, rc;

  warg_opt option_table[]
      = { /* longopt, shortopt, argname, store, type, description */
          { "brady-is-a-wanker", 7, 0, 0, WARG_TYPE_STRING, "just sayin'" },
          { "indent", 'I', "NUM", &indent, WARG_TYPE_INT, "indent NUM spaces; 0 implies compact formatting" },
          { "infile", 'i', "FILE", &infile, WARG_TYPE_STRING, "read input from FILE; '-' to read from stdin" },
          { "jumping-jacks", 8, "NUM", &jumping_jacks, WARG_TYPE_INT, "number of jumping jacks to do" },
          { 0, 'K', "KANGAROOS", &kangaroos, WARG_TYPE_INT, "how many kangaroos are there?" },
          { "outfile", 'o', "FILE", &outfile, WARG_TYPE_STRING, "write output to FILE; '-' to write to stdout" },
          { "prefix", 'p', "STRING", &prefix, WARG_TYPE_STRING, "prefix widget names with STRING" },
          { "this-is-a-really-absurdly-unnecessarily-long-flag-name", 'T', 0, &max_frobulate, WARG_TYPE_INT,
            "set all of the doinklets to max frobulation" },
          { "verbose", 'v', 0, &verbose, WARG_TYPE_INT, "turn on verbose logging" },
          { 0, 'Z', 0, &zazzle, WARG_TYPE_STRING, "add some zazzle to the mix" },
          WARG_AUTOHELP,
          WARG_TABLE_END
        };

  warg_context *option_context = warg_context_init (option_table, 0, argc, argv);
  if (!option_context)
    {
      fprintf (stderr, "error: could not initialize option parsing context\n");
      exit (1);
    }

  while ((rc = warg_next_option (option_context)) != WARG_OK)
    {
      switch (rc)
        {
        case 7:
          {
            printf ("brady is a wanker\n");
          }
          break;
        case 'I':
          {
            printf ("indent: %i\n", indent);
          }
          break;
        case 'i':
          {
            printf ("infile: %s\n", infile);
          }
          break;
        case 8:
          {
            printf ("do %i jumping jacks\n", jumping_jacks);
          }
          break;
        case 'K':
          {
            printf ("kangaroos: %i\n", kangaroos);
          }
        case 'o':
          {
            printf ("outfile: %s\n", outfile);
          }
          break;
        case 'p':
          {
            printf ("prefix: %s\n", prefix);
          }
          break;
        case 'T':
          {
            printf ("max_frobulate: %i\n", max_frobulate);
          }
          break;
        case 'v':
          {
            printf ("verbose: %i\n", verbose);
          }
          break;
        case 'Z':
          {
            printf ("zazzle: %s\n", zazzle);
          }
          break;
        case WARG_HELP_CHAR:
          {
            warg_print_help (stdout, option_context);
            rc = 0;
            goto cleanup;
          }
          break;
        default:
          {
            warg_print_error (stderr, option_context, rc);
            rc = 1;
            goto cleanup;
          }
        }
    };

  const char **extra_args = warg_extra_args (option_context);
  for (int i = 0; extra_args[i]; i++)
    {
      printf ("extra arg: %s\n", extra_args[i]);
    }

  rc = 0;

cleanup:
  warg_context_destroy (option_context);

  return rc;
}
