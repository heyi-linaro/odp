include $(top_srcdir)/platform/Makefile-paths.inc

LDADD = $(LIB)/lib$(ODP_LIB_STR).la $(LIB)/libodphelper.la $(DPDK_PMDS) $(OPENSSL_LIBS)

AM_CFLAGS += -I$(srcdir) \
	     -I$(top_srcdir)/example \
	     -I$(top_srcdir)/helper/include

AM_LDFLAGS += -L$(LIB)
