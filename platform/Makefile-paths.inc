# Common Makefile paths for all ODP applications and implementations

LIB = $(top_builddir)/lib
pkgconfigdir = $(libdir)/pkgconfig

GLOBAL_INCLUDES = -I$(top_srcdir)/include
GLOBAL_INCLUDES += -I$(top_srcdir)/frameworks/modular
GLOBAL_INCLUDES += -I$(top_srcdir)/include/odp/arch/$(ARCH_ABI)
GLOBAL_INCLUDES += -I$(top_srcdir)/platform/$(with_platform)/include
GLOBAL_INCLUDES += -I$(top_builddir)/include
GLOBAL_INCLUDES += -I$(top_builddir)/platform/$(with_platform)/include

AM_CFLAGS   += $(GLOBAL_INCLUDES)
AM_CXXFLAGS  = $(GLOBAL_INCLUDES)

# Per implementation paths specification
include $(top_srcdir)/platform/$(with_platform)/Makefile-paths.inc
