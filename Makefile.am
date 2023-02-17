lib_LTLIBRARIES = libwarg.la
libwarg_la_SOURCES = warg.c warg.h

ACLOCAL_AMFLAGS = -I m4

check_PROGRAMS = warg.test
warg_test_SOURCES = test.c
warg_test_LDADD = libwarg.la
TESTS = warg.test

EXTRA_DIST = LICENSE README.md TODO.md VERSION $(check_SCRIPTS)
