BINARY=warg-test
OBJECTS=warg-test.o
LIBRARY=../warg.a
TESTS=$(wildcard *.opts)

check: warg-test ## run unit tests
	@./accept.test $(TESTS)
.PHONY: check

clean: ## clean up intermediate object files
	rm -f $(OBJECTS)
.PHONY: clean

realclean: clean ## clean up *everything*
	rm -f $(BINARY) *.diff *.err *.out

help: ## Show this help
	@echo "\nSpecify a command. The choices are:\n"
	@grep -E '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[0;36m%-12s\033[m %s\n", $$1, $$2}'
	@echo ""
.PHONY: help

$(OBJECTS): $(SOURCES)

$(BINARY): $(OBJECTS) $(LIBRARY)
	$(CC) $(CFLAGS) -g $^ -o $@

$(LIBRARY):
	make -C .. $@
