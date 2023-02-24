* more error test cases
* store default values *before* parsing (so --help output doesn't get modified)
  * instead of just "blindly" taking opts might want to create my own table and copy it in?
* allow for optional arguments
* doc comments & a doxygen `make doc` target
* make warg_context opaque
  * downside: this will require dynamic memory allocation (sucks)
  * alternative: put big bold red warning flags around warg_context "HERE BE TRANSPARENT DARGONS!"
* ferret out and address TODOs in code comments...
* github CI workflow (at least a basic `make check` before merge)
