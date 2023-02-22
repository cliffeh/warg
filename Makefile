LIBRARY=warg.a
OBJECTS=warg.o
SOURCES=warg.c warg.h
TESTS=$(wildcard test/*.opts)

all: $(LIBRARY) ## generate the warg.a library (default)

check: ## run unit tests (work in progress)
	make -C test check
.PHONY: check

format: ## format all source files (requires: clang-format)
	clang-format -i $(SOURCES)
	make -C test format
.PHONY: format

clean: ## clean up intermediate object files
	rm -f $(OBJECTS)
	make -C test clean
.PHONY: clean

realclean: clean ## clean up *everything*
	rm -f $(LIBRARY)
	make -C test realclean
.PHONY: realclean

help: ## Show this help
	@echo "\nSpecify a command. The choices are:\n"
	@grep -E '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[0;36m%-12s\033[m %s\n", $$1, $$2}'
	@echo ""
.PHONY: help

$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJECTS): $(SOURCES)
