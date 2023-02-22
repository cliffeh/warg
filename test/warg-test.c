#include "../warg.h"
#include <stdlib.h>

int
main (int argc, const char *argv[])
{
  char infile[4096] = "-", outfile[4096] = "-", prefix[4096] = { 0 };
  int indent = 0, max_frobulate = 0, verbose = 0, rc;

  warg_context option_context;
  warg_opt option_table[] = {
    /* longopt, shortopt, argname, store, type, description */
    { "indent", 'I', "NUM", &indent, WARG_TYPE_INT,
      "indent NUM spaces; 0 implies compact formatting" },
    { "infile", 'i', "FILE", &infile, WARG_TYPE_STRING,
      "read input from FILE; '-' to read from stdin" },
    { "outfile", 'o', "FILE", &outfile, WARG_TYPE_STRING,
      "write output to FILE; '-' to write to stdout" },
    { "prefix", 'p', "STRING", &prefix, WARG_TYPE_STRING,
      "prefix widget names with STRING" },
    { "this-is-a-really-absurdly-unnecessarily-long-flag-name", 'T', 0,
      &max_frobulate, WARG_TYPE_INT,
      "set all of the doinklets to max frobulation" },
    { "verbose", 'v', 0, &verbose, WARG_TYPE_INT, "turn on verbose logging" },
    WARG_AUTOHELP,
    WARG_TABLE_END
  };

  warg_context_init (&option_context, option_table, argc, argv);

  while ((rc = warg_next_option (&option_context)) != WARG_OK)
    {
      switch (rc)
        {
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
        case WARG_HELP_CHAR:
          {
            warg_print_help (stdout, &option_context);
            exit (0);
          }
          break;
        default:
          {
            warg_print_error (stderr, &option_context, rc);
            exit (1);
          }
        }
    };

  const char **extra_args = warg_extra_args (&option_context);
  for (int i = 0; extra_args[i]; i++)
    {
      printf ("extra arg: %s\n", extra_args[i]);
    }

  return 0;
}
