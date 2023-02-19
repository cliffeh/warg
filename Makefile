LIBRARY=warg.a
OBJECTS=warg.o
SOURCES=warg.c warg.h

all: $(LIBRARY) ## generate the warg.a library (default)

check: test/warg.test ## run unit tests (work in progress)
	test/warg.test
.PHONY: check

clean: ## clean up intermediate object files
	rm -f $(OBJECTS) test/test.o
.PHONY: clean

realclean: clean ## clean up *everything*
	rm -f $(LIBRARY) test/warg.test

help: ## Show this help
	@echo "\nSpecify a command. The choices are:\n"
	@grep -E '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[0;36m%-12s\033[m %s\n", $$1, $$2}'
	@echo ""
.PHONY: help

$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJECTS): $(SOURCES)

test/warg.test: test/test.o $(LIBRARY) 
	$(CC) $(CFLAGS) -g $^ -o $@
