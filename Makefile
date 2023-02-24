BINARY=warg-test
LIBRARY=warg.a
OBJECTS=warg.o
SOURCES=warg.c warg.h
TESTDIR=tests
CFLAGS=-Wall
ACCEPT_TESTS=$(wildcard $(TESTDIR)/accept_*.opts)
REJECT_TESTS=$(wildcard $(TESTDIR)/reject_*.opts)

all: lib check ## build everything and run the test suite (default)

lib: $(LIBRARY) # build the warg library

check: warg-test ## run unit tests (work in progress)
	@tests/accept.test $(ACCEPT_TESTS)
	@tests/reject.test $(REJECT_TESTS)
.PHONY: check

format: ## format all source files (requires: clang-format)
	clang-format -i $(SOURCES)
	make -C test format
.PHONY: format

clean: ## clean up intermediate object files
	rm -f $(OBJECTS) warg-test.o
.PHONY: clean

testclean: ## clean up testing logs
	rm -f $(TESTDIR)/*.diff $(TESTDIR)/*.err $(TESTDIR)/*.out

realclean: clean testclean ## clean up *everything*
	rm -f $(LIBRARY) $(BINARY)
PHONY: realclean

help: ## Show this help
	@echo "\nSpecify a command. The choices are:\n"
	@grep -E '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[0;36m%-12s\033[m %s\n", $$1, $$2}'
	@echo ""
.PHONY: help

$(BINARY): warg-test.o $(LIBRARY)
	$(CC) $(CFLAGS) $^ -o $@

$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJECTS): $(SOURCES)

warg-test.o: warg-test.c
