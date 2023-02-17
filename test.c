#include "warg.h"

int
main (int argc, const char *argv[])
{
  char infile[4096] = "-", outfile[4096] = "-";
  int indent = 2, max_frobulate = 0;

  warg_context option_context;
  warg_opt option_table[]
      = { { "indent", 'I', "?NUM", "2",
            "indent NUM spaces; implies pretty-printing" },
          { "infile", 'i', "FILE", "-", "read input from FILE" },
          { "outfile", 'o', "FILE", "-", "write output to FILE" },
          { "this-is-a-really-absurdly-unnecessarily-long-flag-name", 'T', 0,
            0, "set all of the doinklets to max frobulation" },
          0 };

  warg_context_init (&option_context, option_table, argc, argv);
  warg_print_help (stdout, &option_context);

  return 0;
}
