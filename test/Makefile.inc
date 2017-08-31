include $(top_srcdir)/platform/Makefile-paths.inc

# In the following line, the libs using the symbols should come before
# the libs containing them! The includer is given a chance to add things
# before libodp by setting PRE_LDADD before the inclusion.
LDADD = $(PRE_LDADD) $(LIB)/libodphelper.la $(LIB)/lib$(ODP_LIB_STR).la $(DPDK_PMDS)

INCFLAGS = -I$(top_srcdir)/helper/include \
	   -I$(top_srcdir)/test

AM_CFLAGS += $(INCFLAGS)
AM_CPPFLAGS += $(CUNIT_CPPFLAGS)
AM_CXXFLAGS += $(INCFLAGS)

AM_LDFLAGS += -L$(LIB) $(STATIC_MODULES_LDFLAGS)

@VALGRIND_CHECK_RULES@
valgrind_tools = memcheck

TESTS_ENVIRONMENT = ODP_PLATFORM=${with_platform} \
		    EXEEXT=${EXEEXT} \
		    ODP_SYSCONFIG_FILE=none
