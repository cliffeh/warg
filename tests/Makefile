LIBRARY=../warg.a # in parent project
BINARY=warg-test
OBJECTS=warg-test.o
SOURCES=warg-test.c
ACCEPT_TESTS=$(wildcard accept_*.opts)
REJECT_TESTS=$(wildcard reject_*.opts)

check: warg-test ## run unit tests
	@./accept.test $(ACCEPT_TESTS)
	@./reject.test $(REJECT_TESTS)
.PHONY: check

format: ## format all source files (requires: clang-format)
	clang-format -i $(SOURCES)
.PHONY: format

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
	make -C ..
