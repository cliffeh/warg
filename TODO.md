* moar test cases
  * in particular, need some error case tests!
* more options
  * variant with/without:
    * longopt
    * printable shortopt
    * description?
* store default values *before* parsing (so --help output doesn't get modified)
  * instead of just "blindly" taking opts might want to create my own table and copy it in?
* some things to shore up:
  * printing help with null longopt
  * printing help with !isgraph() shortopt
