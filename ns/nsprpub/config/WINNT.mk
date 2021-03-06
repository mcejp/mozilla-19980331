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

#
# Configuration common to all versions of Windows NT
# and Windows 95.  This file should probably be called
# WIN32.mk.
#

#
# Client build: make sure we use the shmsdos.exe under $(MOZ_TOOLS).
# $(MOZ_TOOLS_FLIPPED) is $(MOZ_TOOLS) with all the backslashes
# flipped, so that gmake won't interpret them as escape characters.
#
ifdef PR_CLIENT_BUILD_WINDOWS
SHELL = $(MOZ_TOOLS_FLIPPED)/bin/shmsdos.exe
endif

CC = cl
CCC = cl
LINK = link
AR = lib -NOLOGO -OUT:"$@"
RANLIB = echo
BSDECHO = echo
NSINSTALL = nsinstall
INSTALL	= $(NSINSTALL)
define MAKE_OBJDIR
if test ! -d $(@D); then rm -rf $(@D); $(NSINSTALL) -D $(@D); fi
endef
RC = rc.exe

GARBAGE = $(OBJDIR)/vc20.pdb $(OBJDIR)/vc40.pdb

XP_DEFINE = -DXP_PC
LIB_SUFFIX = lib
DLL_SUFFIX = dll

OS_CFLAGS = -W3 -nologo -GF -Gy

ifdef MOZ_PROF

#
# compile with debug symbols, but without DEBUG code and ASSERTs
#
ifdef USE_DEBUG_RTL
OS_CFLAGS += -MDd
else
OS_CFLAGS += -MD
endif
OPTIMIZER = -Od -Z7
#OPTIMIZER = -Zi -Fd$(OBJDIR)/ -Od
DEFINES = -UDEBUG -U_DEBUG -DNDEBUG
DLLFLAGS = -DEBUG -DEBUGTYPE:CV -OUT:"$@"
OBJDIR_TAG = _DBG
LDFLAGS = -DEBUG -DEBUGTYPE:CV

else

ifdef BUILD_OPT
OS_CFLAGS += -MD
OPTIMIZER = -O2
DEFINES = -UDEBUG -U_DEBUG -DNDEBUG
DLLFLAGS = -OUT:"$@"
OBJDIR_TAG = _OPT
else
#
# Define USE_DEBUG_RTL if you want to use the debug runtime library
# (RTL) in the debug build
#
ifdef USE_DEBUG_RTL
OS_CFLAGS += -MDd
else
OS_CFLAGS += -MD
endif
OPTIMIZER = -Od -Z7
#OPTIMIZER = -Zi -Fd$(OBJDIR)/ -Od
DEFINES = -DDEBUG -D_DEBUG -UNDEBUG
DLLFLAGS = -DEBUG -DEBUGTYPE:CV -OUT:"$@"
OBJDIR_TAG = _DBG
LDFLAGS = -DEBUG -DEBUGTYPE:CV
endif
endif

DEFINES += -DWIN32
ifeq ($(OS_TARGET),WINNT)
#
# Win NT needs -GT so that fibers can work
#
OS_CFLAGS += -GT
DEFINES += -DWINNT
else
DEFINES += -DWIN95 -D_PR_GLOBAL_THREADS_ONLY
endif

ifeq ($(CPU_ARCH),x386)
DEFINES += -D_X86_
else
ifeq ($(CPU_ARCH),MIPS)
DEFINES += -D_MIPS_
else
ifeq ($(CPU_ARCH),ALPHA)
DEFINES += -D_ALPHA_=1
else
CPU_ARCH = processor_is_undefined
endif
endif
endif

# Name of the binary code directories

ifeq ($(CPU_ARCH),x386)
CPU_ARCH_TAG =
else
CPU_ARCH_TAG = $(CPU_ARCH)
endif

ifdef USE_DEBUG_RTL
OBJDIR_SUFFIX = OBJD
else
OBJDIR_SUFFIX = OBJ
endif

OBJDIR_NAME = $(OS_CONFIG)$(CPU_ARCH_TAG)$(OBJDIR_TAG).$(OBJDIR_SUFFIX)

OS_DLLFLAGS = -nologo -DLL -SUBSYSTEM:WINDOWS -PDB:NONE
