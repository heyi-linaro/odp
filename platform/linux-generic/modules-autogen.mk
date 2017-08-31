# Subsystem and module rules and targets autogen

# Take Makefile-paths.inc as input, generate Makefile-modules.inc
# to be included by Makefile.am before automake generates Makefile.in
include Makefile-paths.inc

# Subsystem's SOURCES variable mentions which sources should be linked
# into ODP library for the subsystem, it automatically appends subsystem.c
# by default.
$(foreach subs,$(SUBSYSTEMS), \
	$(eval __subsystem_$(subst -,_,$(subs))_SOURCES = \
		$(__subsystem_$(subst -,_,$(subs))_SOURCES) subsystem.c))

define subsystem_append_sources
	# Append subsystem's SOURCES to the ODP library build
	@echo '__LIB__libodp_linux_la_SOURCES += \
		$(patsubst %,$(1)/%,$(__subsystem_$(subst -,_,$(1))_SOURCES))'

endef

# xxx_LTLIBRARIES primary declares the libraries to be built by libtool,
# the xxx prefix is used to specify install directory of the libraries.
define subsystem_libtool_variables
	# Per subsystem _LTLIBRARIES primary
	@echo 'lib_modules_$(subst -,_,$(1))_LTLIBRARIES ='

	# Install directory for the subsystem modules
	@echo 'lib_modules_$(subst -,_,$(1))dir = $$(libdir)/modules/$(1)'

endef

define autogen_subsystem_variables
	$(call subsystem_append_sources,$(1))
	$(call subsystem_libtool_variables,$(1))
endef

# Tell libtool to build modules that can be dlopened, module names
# don’t need to be prefixed with ‘lib’.
MODULE_LDFLAGS = -module -avoid-version

define autogen__variables
endef

define autogen_common_variables
	# Generate module's libtool target
	@echo 'lib_modules_$(subst -,_,$(1))_LTLIBRARIES += \
		$(patsubst %,$$(MODULES)/$(1)/%.la,$(2))'

	# Generate per module libtool SOURCES and LDFLAGS, by default
	# it assumes each module has only one source with the same
	# name, and uses the default LDFLAGS as above defined:
	@echo '__MODULES__$(subst -,_,$(1))_$(2)_la_SOURCES = $(subs)/$(2).c'
	@echo '__MODULES__$(subst -,_,$(1))_$(2)_la_LDFLAGS = $(MODULE_LDFLAGS)'

	# In case the module has a private implementation header with
	# the same name, prevent it from being installed in distribution
	@if [ -n "$(wildcard $(1)/$(2).h)" ]; then \
		echo 'noinst_HEADERS += $(wildcard $(1)/$(2).h)'; \
	 fi
endef

define autogen_active_variables
	# Generate target specific CFLAGS variable if the module was
	# selected as the active(default) one.
	@echo '$(1)/$(word 1,$(subst :, ,$(2))).lo: \
		AM_CFLAGS += -DIM_ACTIVE_MODULE'
endef

define autogen_module_variables
	$(call autogen_common_variables,$(1),$(word 1,$(subst :, ,$(2))))
	$(call autogen_$(word 2,$(subst :, ,$(2)))_variables,$(1),$(2))
endef

# Generate Makefile portion to be included in platform/.../Makefile.am,
# because automake does not support dynamic variables computation.
autogen-Makefile.include:
	$(foreach subs,$(SUBSYSTEMS), \
		$(call autogen_subsystem_variables,$(subs)) \
		$(foreach mod,$(__subsystem_$(subst -,_,$(subs))_MODULES), \
			$(call autogen_module_variables,$(subs),$(mod))))
	# Install pkgconfig descriptions for ODP loadable modules
	@echo 'pkgconfig_DATA += $$(top_builddir)/pkgconfig/libodp-modules.pc'

# List all installed ODP loadable modules
MODULES = $(foreach subs,$(SUBSYSTEMS), \
		$(foreach mod,$(__subsystem_$(subst -,_,$(subs))_MODULES), \
		$${libdir}/modules/$(subs)/$(subst :active,,$(mod))))

# Package static modules as whole archive
STATIC_modules = --whole-archive $(strip $(MODULES:=.a)) --no-whole-archive
STATIC_modules_ = -Wl,$(subst $(space),$(comma),$(STATIC_modules))

# Link dynamic modules as needed even without symbol references
DYNAMIC_modules = --no-as-needed $(strip $(MODULES:=.so)) --as-needed
DYNAMIC_modules_ = -Wl,$(subst $(space),$(comma),$(DYNAMIC_modules))

# Generate libodp-modules.pc.in, application can use pkgconfig to link
# all ODP loadable modules.
autogen-libodp-modules.pc.in:
	@echo 'prefix=@prefix@'
	@echo 'exec_prefix=@exec_prefix@'
	@echo 'libdir=@libdir@'
	@echo 'includedir=@includedir@'
	@echo
	@echo 'Name: libodp-modules'
	@echo 'Description: All ODP loadable modules'
	@echo 'Version: @PKGCONFIG_VERSION@'
# Unfortunately not work yet!!! @echo 'Libs: -L$${libdir} ${DYNAMIC_modules_}'
	@echo 'Libs: -L$${libdir}'
	@echo 'Libs.private: ${STATIC_modules_}'
	@echo 'Cflags: -I$${includedir}'

.PHONY: autogen-Makefile.include autogen-libodp-modules.pc.in
