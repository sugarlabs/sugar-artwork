# This Makefile and everything is based on code from gtk-engines.

# We just have sugar in here ...
BUILD_ENGINES = sugar

# Initilize TESTS and XFAIL_TESTS
TESTS =
XFAIL_TESTS =

GTKRCS =				\
	gtkrcs/buildin			\
	gtkrcs/sugar

EXTRA_DIST = exported torture valgrind-suppressions $(GTKRCS)


EXTRA_PROGRAMS = torturetest
torturetest_CFLAGS = $(GTK2_CFLAGS)
torturetest_LDFLAGS = $(GTK2_LIBS)



# Setup enviroment variables for GTK+
# ie. add $(top_builddir)/tests to the module searchpath, so that GTK+ finds
# the engines in tests/engines
TESTS_ENVIRONMENT=GTK_PATH="."

VALGRIND_COMMAND=valgrind --leak-resolution=high --leak-check=full --suppressions=$(srcdir)/valgrind-suppressions --num-callers=20 --log-file-exactly=valgrind-logs/THEME
# set some flags to make glib more valgrind friendly
VALGRIND_ENV=G_SLICE=always-malloc G_DEBUG=gc-friendly,resident-modules

#################################################################
#################################################################

#############################################################
# Exported Symbols Test - Tests all engines unconditionally
#############################################################
# Prefix with exported_
EXPORTED_SYMBOLS_TESTS = $(patsubst %,exported_%,$(BUILD_ENGINES))

# The whole lua crap gets exported ... so right now it has to fail
XFAIL_TESTS += exported_lua

TESTS += $(EXPORTED_SYMBOLS_TESTS)


#############################################################
# Torture Test
#############################################################
SUPPORTED_TORTURE_TEST_ENGINES = \
	sugar

# Filter engines that are not build from the set of engines
TORTURE_TEST_ENGINES = $(filter $(SUPPORTED_TORTURE_TEST_ENGINES),$(BUILD_ENGINES))
# Prefix with torture_
TORTURE_TEST_TESTS = torture_buildin $(patsubst %,torture_%,$(TORTURE_TEST_ENGINES))

# Add TORTURE_TEST_ENGINES to list of tests
TESTS += $(TORTURE_TEST_TESTS)

# Possible other tests:
#  - An extensive theme switch tests that loads/unloads the engine
#    multiple times. A requirement here is that multiple rc styles need
#    to be merged. (ie. the test should find the recently fixed smooth
#    memory corruption)
#  - RC style merging? Don't think there is a way to do that.
#  - Maybe a visual test that looks for visible changes? That way one can
#    check that a modification does not break anything. (eg. when doing
#    code cleanups)
#  - I would love to have some coverage statistics for the torture test
#    anyone knows how to do that?

#################################################################
#################################################################

engines:
	-rm -rf engines
	mkdir engines
	cd engines; \
	ln -s ../$(top_builddir)/gtk/engine/.libs/libsugar.so libsugar.so

CLEANFILES = \
	torture_buildin-failed \
	$(EXTRA_PROGRAMS)

clean-local:
	-$(RM) torture_*
	-$(RM) exported_*
	-$(RM) -r engines
	-$(RM) -r valgrind-logs
	-$(RM) -r symbols

valgrind-logs:
	-$(RM) -r valgrind-logs
	mkdir valgrind-logs

symbols:
	mkdir symbols

torture_%: torture torturetest engines
	cp $(srcdir)/torture $@

exported_%: exported engines symbols
	cp $(srcdir)/exported $@

check-valgrind: valgrind-logs
	$(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT="$(TESTS_ENVIRONMENT) $(VALGRIND_ENV) EXEC_WRAPPER=\"$(VALGRIND_COMMAND)\""

test:
	@$(MAKE) $(AM_MAKEFLAGS) check

.PHONY: engines valgrind-logs
