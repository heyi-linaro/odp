AM_CFLAGS   += -I$(top_srcdir)/platform/$(with_platform)/arch/$(ARCH_DIR)
AM_CXXFLAGS += -I$(top_srcdir)/platform/$(with_platform)/arch/$(ARCH_DIR)

# Pktio operations subsystem and its modules, please notice that
# dashes(-) is allowed in subsystem names but should be replaced
# by underscores(_) in MODULES variable definition.
SUBSYSTEMS = pktio-ops
__subsystem_pktio_ops_MODULES = dpdk		\
				ipc		\
				loopback	\
				netmap		\
				pcap		\
				socket		\
				socket_mmap	\
				tap

# Pool public APIs subsystem and its modules, also demonstrates
# how to select an active(default) module by appending ":active"
# after the module name.
SUBSYSTEMS += pool
__subsystem_pool_MODULES = generic:active

# New subsystems can be added follows:
# SUBSYSTEMS += new
# __subsystem_new_MODULES = one:active two three four

# Subsystem's SOURCES variable mentions which sources should be
# linked into ODP library for the subsystem, it automatically
# appends subsystem.c by default, so subsystem writers wouldn't
# need to specify here, unless there are extras, like pktio-ops
# subsystem, which has extra SOURCES needed to be linked into ODP
# library.
__subsystem_pktio_ops_SOURCES += common.c	\
				 ethtool.c	\
				 ring.c		\
				 sysfs.c

# TODO: refactory the code to make it a real decoupled and loadable
# module, currently it MUST be linked into ODP library due to tightly
# coupled symbol references.
__subsystem_pool_SOURCES += generic.c

# Below automatically generate LDFLAGS for application linkage:
# currently we only support static link since the modules are
# not actually decoupled and clearly modularized.
empty =
comma = ,
space = $(empty) $(empty)

STATIC_MODULES = $(foreach subs,$(SUBSYSTEMS), \
	$(foreach mod,$(__subsystem_$(subst -,_,$(subs))_MODULES), \
		$(MODULES)/$(subs)/.libs/$(subst :active,,$(mod)).a))

STATIC_MODULES_ = --whole-archive $(strip $(STATIC_MODULES)) --no-whole-archive
STATIC_MODULES_LDFLAGS = -Wl,$(subst $(space),$(comma),$(STATIC_MODULES_))
