# file: GNUmakefile
#

# setup the environment
#

# DESTDIR is used by install scripts to install to a fake root
DESTDIR =

# ISIP and isip both point to the base of the ISIP source tree
#
ISIP = $(ISIP_IFC)
isip = $(ISIP)
ISIP_DEVEL = $(ISIP)
export ISIP isip ISIP_DEVEL

# we need to capture these configure variables as variables so they
# can be referenced by other configure variables
#
prefix := $ISIP
exec_prefix := ${prefix}
target := linux-gnu
export prefix exec_prefix target

# define the location of g++
#
ifndef ISIP_CPLUS_COMPILER
	ISIP_CPLUS_COMPILER := g++
	export ISIP_CPLUS_COMPILER
endif

# define a binary directory. if uname does not work on your system,
# you need to create some arbitrary string which will be unique across
# each architecture you use. our values for ISIP_BINARY are
# i386_SunOS_5.6 or sparc_SunOS_5.6. 
#
ifndef ISIP_BINARY
	ISIP_BINARY := linux-gnu
	export ISIP_BINARY
endif

# ----------------------------------------
# you shouldn't need to edit anything below this line
# ----------------------------------------

ifndef ISIP_INCLUDE
	ISIP_INCLUDE := -I$(ISIP_DEVEL)/include
	export ISIP_INCLUDE
endif

ifndef ISIP_LIBS
	ISIP_LIBS := -L$(ISIP_DEVEL)/lib/$(ISIP_BINARY)  -l_asr -l_pr -l_search -l_sp -l_algo -l_stat  -l_mmedia -l_shell -l_numeric -l_math_matrix -l_math_vector -l_math_scalar -l_io -l_system -lm -lgomp
	export ISIP_LIBS
endif

# if a partial ISIP envioronment is running, it could cause problems. 
# unset a few variables
#
unexport ISIP_WORK

# force our default compilation options: optimize
#
DEBUG=
OPTIMIZE=-O2
export DEBUG OPTIMIZE

# define the order these directories are built
#
ISIP_ORDER = scripts/ class/ util/

# define additional things to be deleted with distclean
#
ISIP_DISTCLEAN = config.cache config.log config.status GNUmakefile ISIP_BASE_ENV.sh class/system/Integral/IntegralConfigure.h scripts/make/compile_configure.make include lib bin

# define the full install directive. this triggers traverse.make's
# install directive to call the install-release directive.
#
ISIP_POST_INSTALL := install-release

# we will only install things if the target directories are outside
# the source tree -- if we don't do this it may try to copy a file
# over itself.
#
ifneq "${exec_prefix}/bin" "$(ISIP)/bin"
	RELEASE_BIN := install-release-bin
endif

ifneq "${exec_prefix}/lib" "$(ISIP)/lib"
	RELEASE_LIB := install-release-lib
endif

ifneq "${prefix}/include" "$(ISIP)/include"
	RELEASE_HEADER := install-release-header
endif

# include the isip standard make template. make this a quite include, since
# very likely the make directory will not exist and it will have to execute
# the rule below which installs the makefiles
#
-include $(ISIP_DEVEL)/lib/scripts/make/traverse.make

# define additional rules: install makefiles 
#
$(ISIP_DEVEL)/lib/scripts/make/traverse.make: $(ISIP_DEVEL)/scripts/make/traverse.make
	$(MAKE) --directory $(ISIP_DEVEL)/scripts/make install

# define additional rules: debug environment
#
debug_env:
	echo "ISIP =" $(ISIP)
	echo "isip =" $(isip)
	echo "ISIP_DEVEL =" $(ISIP_DEVEL)
	echo "ISIP_CPLUS_COMPILER =" $(ISIP_CPLUS_COMPILER)
	echo "ISIP_BINARY =" $(ISIP_BINARY)
	echo "ISIP_INCLUDE =" $(ISIP_INCLUDE)
	echo "ISIP_LIBS =" $(ISIP_LIBS)

# define additional rules: full installation
#
install-release: $(RELEASE_BIN) $(RELEASE_LIB) $(RELEASE_HEADER)

install-release-bin:
	echo "copying binaries -> " $(DESTDIR)${exec_prefix}/bin
	mkdir -p $(DESTDIR)${exec_prefix}/bin
	-cp $(ISIP_DEVEL)/bin/scripts/* $(DESTDIR)${exec_prefix}/bin
	-cp $(ISIP_DEVEL)/bin/$(ISIP_BINARY)/* $(DESTDIR)${exec_prefix}/bin

install-release-lib:
	echo "copy libraries -> " $(DESTDIR)${exec_prefix}/lib
	mkdir -p $(DESTDIR)${exec_prefix}/lib
	-cp -r $(ISIP_DEVEL)/lib/* $(DESTDIR)${exec_prefix}/lib

install-release-header:
	echo "copy includes -> " $(DESTDIR)${prefix}/include
	mkdir -p $(DESTDIR)${prefix}/include
	-cp $(ISIP_DEVEL)/include/* $(DESTDIR)${prefix}/include

#
# end of file
