#! gmake
#
# The contents of this file are subject to the Netscape Public License
# Version 1.0 (the "NPL"); you may not use this file except in
# compliance with the NPL.  You may obtain a copy of the NPL at
# http://www.mozilla.org/NPL/
# 
# Software distributed under the NPL is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
# for the specific language governing rights and limitations under the
# NPL.
# 
# The Initial Developer of this code under the NPL is Netscape
# Communications Corporation.  Portions created by Netscape are
# Copyright (C) 1998 Netscape Communications Corporation.  All Rights
# Reserved.
#

################################################################################
# We have a 4 pass build process:
#
# Pass 1. export - Create generated headers and stubs. Publish public headers to
#		dist/<arch>/include.
#
# Pass 2. libs - Create libraries. Publish libraries to dist/<arch>/lib.
#
# Pass 3. all - Create programs. 
#
# Pass 4. install - Publish programs to dist/<arch>/bin.
#
# Parameters to this makefile (set these before including):
#
# a)
#	TARGETS	-- the target to create 
#			(defaults to $LIBRARY $PROGRAM)
# b)
#	DIRS	-- subdirectories for make to recurse on
#			(the 'all' rule builds $TARGETS $DIRS)
# c)
#	CSRCS   -- .c files to compile
#			(used to define $OBJS)
# d)
#	PROGRAM	-- the target program name to create from $OBJS
#			($OBJDIR automatically prepended to it)
# e)
#	LIBRARY	-- the target library name to create from $OBJS
#			($OBJDIR automatically prepended to it)
#
################################################################################

ifndef NSPR_CONFIG_MK
include $(MOD_DEPTH)/config/config.mk
endif

#
# This makefile contains rules for building the following kinds of
# libraries:
# - LIBRARY: a static (archival) library
# - SHARED_LIBRARY: a shared (dynamic link) library
# - IMPORT_LIBRARY: an import library, used only on Windows and OS/2
# - PURE_LIBRARY: a library for Purify
#
# The names of these libraries can be generated by simply specifying
# LIBRARY_NAME and LIBRARY_VERSION.
#

ifdef LIBRARY_NAME
ifeq ($(OS_ARCH), WINNT)

#
# Win16 and OS/2 require library names conforming to the 8.3 rule.
# other platforms do not.
#
ifeq (,$(filter-out WIN16 OS2,$(OS_TARGET)))
LIBRARY		= $(OBJDIR)/$(LIBRARY_NAME)$(LIBRARY_VERSION)_s.lib
SHARED_LIBRARY	= $(OBJDIR)/$(LIBRARY_NAME)$(LIBRARY_VERSION).dll
IMPORT_LIBRARY	= $(OBJDIR)/$(LIBRARY_NAME)$(LIBRARY_VERSION).lib
else
LIBRARY		= $(OBJDIR)/lib$(LIBRARY_NAME)$(LIBRARY_VERSION)_s.lib
SHARED_LIBRARY	= $(OBJDIR)/lib$(LIBRARY_NAME)$(LIBRARY_VERSION).dll
IMPORT_LIBRARY	= $(OBJDIR)/lib$(LIBRARY_NAME)$(LIBRARY_VERSION).lib
endif

else

LIBRARY		= $(OBJDIR)/lib$(LIBRARY_NAME)$(LIBRARY_VERSION).$(LIB_SUFFIX)
ifeq ($(OS_ARCH)$(OS_RELEASE), AIX4.1)
SHARED_LIBRARY	= $(OBJDIR)/lib$(LIBRARY_NAME)$(LIBRARY_VERSION)_shr.a
else
SHARED_LIBRARY	= $(OBJDIR)/lib$(LIBRARY_NAME)$(LIBRARY_VERSION).$(DLL_SUFFIX)
endif
ifdef HAVE_PURIFY
ifdef DSO_BACKEND
PURE_LIBRARY	= $(OBJDIR)/purelib$(LIBRARY_NAME)$(LIBRARY_VERSION).$(DLL_SUFFIX)
else
PURE_LIBRARY	= $(OBJDIR)/purelib$(LIBRARY_NAME)$(LIBRARY_VERSION).$(LIB_SUFFIX)
endif
endif

endif
endif

ifndef TARGETS
ifeq ($(OS_ARCH), WINNT)
TARGETS		= $(LIBRARY) $(SHARED_LIBRARY) $(IMPORT_LIBRARY)
else
TARGETS		= $(LIBRARY) $(SHARED_LIBRARY)
ifdef HAVE_PURIFY
TARGETS		+= $(PURE_LIBRARY)
endif
endif
endif

#
# OBJS is the list of object files.  It can be constructed by
# specifying CSRCS (list of C source files) and ASFILES (list
# of assembly language source files).
#

ifndef OBJS
OBJS		= $(addprefix $(OBJDIR)/,$(CSRCS:.c=.o)) \
		  $(addprefix $(OBJDIR)/,$(ASFILES:.s=.o))
endif

ifeq ($(OS_TARGET), WIN16)
	comma := ,
	empty :=
	space := $(empty) $(empty)
	W16OBJS = $(subst $(space),$(comma)$(space),$(strip $(OBJS)))
	W16TEMP =$(OS_LIBS) $(EXTRA_LIBS)
    ifeq ($(strip $(W16TEMP)),)
		W16LIBS =
    else
    	W16LIBS = library $(subst $(space),$(comma)$(space),$(strip $(W16TEMP)))
    endif
	W16DEF = $(notdir $(basename $(SHARED_LIBRARY))).DEF
endif

ifeq ($(OS_ARCH), WINNT)
ifneq ($(OS_TARGET), WIN16)
ifneq ($(OS_TARGET), OS2)
OBJS += $(RES)
endif
endif
endif

ALL_TRASH		= $(TARGETS) $(OBJS) $(OBJDIR) LOGS TAGS $(GARBAGE) \
			  $(NOSUCHFILE) \
			  so_locations

ifdef DIRS
LOOP_OVER_DIRS		=					\
	@for d in $(DIRS); do					\
		if test -d $$d; then				\
			set -e;					\
			echo "cd $$d; $(MAKE) $@";		\
			$(MAKE) -C $$d $@;			\
			set +e;					\
		else						\
			echo "Skipping non-directory $$d...";	\
		fi;						\
	done
endif

################################################################################

all:: export libs install

export::
	+$(LOOP_OVER_DIRS)

libs::
	+$(LOOP_OVER_DIRS)

install::
	+$(LOOP_OVER_DIRS)

clean::
	rm -rf $(OBJS) so_locations $(NOSUCHFILE)
	+$(LOOP_OVER_DIRS)

clobber::
	rm -f $(OBJS) $(TARGETS) $(GARBAGE) so_locations $(NOSUCHFILE)
	+$(LOOP_OVER_DIRS)

realclean clobber_all::
	rm -rf $(wildcard *.OBJ *.OBJD) dist $(ALL_TRASH)
	+$(LOOP_OVER_DIRS)

release:: export
ifdef RELEASE_LIBS
	@echo "Copying libraries to release directory"
	@if test -z "$(BUILD_NUMBER)"; then \
		echo "BUILD_NUMBER must be defined"; \
		false; \
	fi
	@if test ! -d $(RELEASE_LIB_DIR); then \
		rm -f $(RELEASE_LIB_DIR); \
		$(NSINSTALL) -D $(RELEASE_LIB_DIR);\
	fi
	cp $(RELEASE_LIBS) $(RELEASE_LIB_DIR)
endif
ifdef RELEASE_HEADERS
	@echo "Copying header files to release directory"
	@if test -z "$(BUILD_NUMBER)"; then \
		echo "BUILD_NUMBER must be defined"; \
		false; \
	fi
	@if test ! -d $(RELEASE_HEADERS_DEST); then \
		rm -rf $(RELEASE_HEADERS_DEST); \
		$(NSINSTALL) -D $(RELEASE_HEADERS_DEST);\
	fi
	cp $(RELEASE_HEADERS) $(RELEASE_HEADERS_DEST)
endif
	+$(LOOP_OVER_DIRS)

alltags:
	rm -f TAGS tags
	find . -name dist -prune -o \( -name '*.[hc]' -o -name '*.cp' -o -name '*.cpp' \) -print | xargs etags -a
	find . -name dist -prune -o \( -name '*.[hc]' -o -name '*.cp' -o -name '*.cpp' \) -print | xargs ctags -a

$(NFSPWD):
	cd $(@D); $(MAKE) $(@F)

$(PROGRAM): $(OBJS)
	@$(MAKE_OBJDIR)
ifeq ($(OS_ARCH),WINNT)
	$(CC) $(OBJS) -Fe$@ -link $(LDFLAGS) $(OS_LIBS) $(EXTRA_LIBS)
else
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS)
endif

$(LIBRARY): $(OBJS)
	@$(MAKE_OBJDIR)
	rm -f $@
	$(AR) $(OBJS) $(AR_EXTRA_ARGS)
	$(RANLIB) $@

ifeq ($(OS_TARGET), WIN16)
$(IMPORT_LIBRARY): $(SHARED_LIBRARY)
	wlib $(OS_LIB_FLAGS) $@ +$(SHARED_LIBRARY)
endif

ifeq ($(OS_TARGET), OS2)
$(IMPORT_LIBRARY): $(SHARED_LIBRARY)
	$(IMPLIB) $@ $(SHARED_LIBRARY).def
endif
    
$(SHARED_LIBRARY): $(OBJS)
	@$(MAKE_OBJDIR)
	rm -f $@
ifeq ($(OS_ARCH)$(OS_RELEASE), AIX4.1)
	echo "#!" > $(OBJDIR)/lib$(LIBRARY_NAME)_syms
	nm -B -C -g $(OBJS) \
		| awk '/ [T,D] / {print $$3}' \
		| sed -e 's/^\.//' \
		| sort -u >> $(OBJDIR)/lib$(LIBRARY_NAME)_syms
	$(LD) $(XCFLAGS) -o $@ $(OBJS) -bE:$(OBJDIR)/lib$(LIBRARY_NAME)_syms \
		-bM:SRE -bnoentry $(OS_LIBS) $(EXTRA_LIBS)

else
ifeq ($(OS_ARCH), WINNT)
ifeq ($(OS_TARGET), WIN16)
	echo system windows dll initinstance >w16link
	echo option map >>w16link
	echo option oneautodata >>w16link
	echo option heapsize=32K >>w16link
	echo option $(OS_DLL_OPTION) >>w16link
	echo debug $(DEBUGTYPE) all >>w16link
	echo name $@ >>w16link
	echo file >>w16link
	echo $(W16OBJS) >>w16link
	echo $(W16IMPORTS) >>w16link
	echo $(W16LIBS) >>w16link
	echo $(W16_EXPORTS) >>w16link
	echo libfile libentry >>w16link
	$(LINK) @w16link.
	rm w16link
else
ifeq ($(OS_TARGET), OS2)
# append ( >> ) doesn't seem to be working under OS/2 gmake. Run through OS/2 shell instead.	
	@cmd /C "echo LIBRARY $(notdir $(basename $(SHARED_LIBRARY))) INITINSTANCE TERMINSTANCE >$@.def"
	@cmd /C "echo PROTMODE >>$@.def"
	@cmd /C "echo CODE    LOADONCALL MOVEABLE DISCARDABLE >>$@.def"
	@cmd /C "echo DATA    PRELOAD MOVEABLE MULTIPLE NONSHARED >>$@.def"	
	@cmd /C "echo EXPORTS >>$@.def"
	@cmd /C "$(FILTER) -B -P $(LIBRARY) >> $@.def"
	$(LINK_DLL) -MAP $(DLLBASE) $(OS_LIBS) $(EXTRA_LIBS) $(OBJS) $@.def
else
	$(LINK_DLL) -MAP $(DLLBASE) $(OS_LIBS) $(EXTRA_LIBS) $(OBJS)
endif
endif
else
	$(MKSHLIB) -o $@ $(OBJS) $(LD_LIBS) $(EXTRA_LIBS) $(OS_LIBS)
endif
endif

$(PURE_LIBRARY):
	rm -f $@
ifneq ($(OS_ARCH), WINNT)
	$(AR) $(OBJS)
endif
	$(RANLIB) $@

ifeq ($(OS_ARCH), WINNT)
$(RES): $(RESNAME)
	@$(MAKE_OBJDIR)
ifeq ($(OS_TARGET),OS2)
	$(RC) -DOS2 -r $(RESNAME) $(RES)
else
	$(RC) -Fo$(RES) $(RESNAME)
endif
	@echo $(RES) finished
endif

$(OBJDIR)/%.o: %.cpp
	@$(MAKE_OBJDIR)
ifeq ($(OS_ARCH), WINNT)
	$(CCC) -Fo$@ -c $(CFLAGS) $<
else
	$(CCC) -o $@ -c $(CFLAGS) $< 
endif

WCCFLAGS1 = $(subst /,\\,$(CFLAGS))
WCCFLAGS2 = $(subst -I,-i=,$(WCCFLAGS1))
WCCFLAGS3 = $(subst -D,-d,$(WCCFLAGS2))
$(OBJDIR)/%.o: %.c
	@$(MAKE_OBJDIR)
ifeq ($(OS_ARCH), WINNT)
ifeq ($(OS_TARGET), WIN16)
#	$(MOD_DEPTH)/config/w16opt $(WCCFLAGS3)
	echo $(WCCFLAGS3) >w16wccf
	$(CC) -zq -fo$(OBJDIR)\\$*.o  @w16wccf $*.c
	rm w16wccf
else
	$(CC) -Fo$@ -c $(CFLAGS) $*.c
endif
else
	$(CC) -o $@ -c $(CFLAGS) $*.c
endif

$(OBJDIR)/%.o: %.s
	@$(MAKE_OBJDIR)
	$(AS) -o $@ $(ASFLAGS) -c $*.s

%.i: %.c
	$(CC) -C -E $(CFLAGS) $< > $*.i

%: %.pl
	rm -f $@; cp $*.pl $@; chmod +x $@

################################################################################
# Special gmake rules.
################################################################################

#
# Re-define the list of default suffixes, so gmake won't have to churn through
# hundreds of built-in suffix rules for stuff we don't need.
#
.SUFFIXES:
.SUFFIXES: .a .o .c .cpp .s .h .i .pl

#
# Fake targets.  Always run these rules, even if a file/directory with that
# name already exists.
#
.PHONY: all alltags clean export install libs realclean release
