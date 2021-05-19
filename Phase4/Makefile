#------------------------------------------------------------------------------
# CPE/CSC 159 SPEDE Project Makefile
# California State University, Sacramento
#
# Written by Greg Crist <crist@csus.edu>
#
# Derived from orginal works sourced from:
#  - John Clevenger <clevenger@csus.edu>
#  - Brian Witt <wittb@ecs.csus.edu>
#  - Weide Chang <changw@ecs.csus.edu>
#
# Version History
#   2020-03-20  gmcrist   Makefile rewrite to simplify implementation
#                         Deprecates 'depends' and 'source' targets
#   2001-08-17  bwitt     Put OBJS= before include "local.mk" so it
#                         can work with $(OBJS) [2.10]
#   2001-03-31  bwitt     target "tarball" added [2.9]
#   2001-02-16  bwitt     Added .cpp for C++ source files [2.8]
#   2000-04-21  bwitt     "make bootable" copies RAWRITE.EXE and BOOT.COM
#                         Added "-include local.mk" [2.7]
#   2000-03-30  bwitt     Now for X86-SPEDE [2.1], based on [1.5]
#   2000-01-19  bwitt     Revised to 68-mkmf
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# (1) Name your operating system.  Must be a legal filename, and not contain
#     spaces or punctuation.  It will be used to name you DLI file.
#
#     Can be overridden via an environment variable, such as:
#        OS_NAME=SpedeOS make
#------------------------------------------------------------------------------
OS_NAME ?= sOS


#------------------------------------------------------------------------------
# (2) Specify additional compiler or linker flags.
#
#     EXTRA_CFLAGS          Additional flags to pass to the compiler
#
#     EXTRA_LDFLAGS         Additional flags to pass to the linker
#------------------------------------------------------------------------------
EXTRA_CFLAGS =
EXTRA_LDFLAGS =


#------------------------------------------------------------------------------
# (3) Specify any compiler optimizations, if desired.
#
# Examples:
#   -fno-defer-pop          Immediately after function return, pop
#                           the argument off (easier to debug).
#
#   -fdefer-pop             Wait until control flow splits before
#                           popping functions args from stack.
#
#   -fwritable-strings      Place string constants in .data segment
#                           so they can be modified (away from .code).
#
#   -fnowritable-strings    Please string constants inside .code segment,
#                           which makes code dis-assembly confusing.
#
#   -O1                     Perform minor optimizations. Also does code
#                           flow analysis for improved error reporting.
#------------------------------------------------------------------------------
OPTIMIZE =


#==============================================================================
# WARNING: Modify the below at your own risk!
#==============================================================================

# C source files
SRC_C = $(wildcard *.c)

# Assembly source files
SRC_S = $(wildcard *.S)

# All sources
SRC = $(SRC_C) $(SRC_S)

# Global include paths
INC = -I/gaia/home/project/spede2/Target-i386/i686/gnu/i386-unknown-gnu/include

# Object definitions from the sources
OBJECTS = $(SRC_C:.c=.o) $(SRC_S:.S=.o)

# DLI filename
DLI = $(OS_NAME).dli

#------------------------------------------------------------------------------
# Build rules
#
# .asm is an output extension so we can generate assembly files.
# These are for additional suffixes.
#
# NOTE: *.asm files are deleted for `make clean', and should ONLY
#       be used for a target, not a source.  Use .S for assembler
#       files you write (do cpp before assembling).
#------------------------------------------------------------------------------
.SUFFIXES: .S .asm .E .cxx .C .cc .cpp

%.o: %.c %.S %.cc

.c.o:
	$(CC) $(TARGET_MACH) $(CFLAGS) $(INC) -c -o $@ $<
.c.asm:
	$(CC) $(TARGET_MACH) $(CFLAGS) $(INC) -S -o $@ $<
	@echo NOTE: $@ is a temporary file
.c.E:
	$(CC) $(TARGET_MACH) $(CFLAGS) $(INC) -E -o $@ $<

.S.o:
	$(CC) -DASSEMBLER $(CFLAGS) $(INC) -c -o $@ $<
.S.E:
	$(CC) -DASSEMBLER $(CFLAGS) $(INC) -E -o $@ $<

.cc.o:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -c -o $@ $<
.cc.asm:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -S -o $@ $<
	@echo NOTE: $@ is a temporary file
.cc.E:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -E -o $@ $<

.C.o:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -c -o $@ $<
.C.asm:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -S -o $@ $<
	@echo NOTE: $@ is a temporary file
.C.E:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -E -o $@ $<

.cpp.o:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -c -o $@ $<
.cpp.asm:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -S -o $@ $<
	@echo NOTE: $@ is a temporary file
.cpp.E:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -E -o $@ $<

.cxx.o:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -c -o $@ $<
.cxx.asm:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -S -o $@ $<
	@echo NOTE: $@ is a temporary file
.cxx.E:
	$(CPLUS) $(TARGET_MACH) $(CFLAGS) $(INC) -E -o $@ $<

#------------------------------------------------------------------------------
# General definitions
#------------------------------------------------------------------------------

# Compilers
CC = i386-unknown-gnu-gcc
AS = i386-unknown-gnu-as
AR = i386-unknown-gnu-ar
NM = i386-unknown-gnu-nm

# Build utilities
CMD_LINKER = link386
CMD_TAGS = ctags
CMD_DELETE = rm -f
CMD_COPY = cp -p
CMD_DOWNLOAD = flash -d
CMD_MAKEMAKE = spede-mkmf

# Files to be removed when a 'clean' is performed
CLEAN_FILES = $(DLI) core *.o *.asm mapfile make.orig *.E tags TAGS *.RC

# Target machine
TARGET_MACH = -i386-unknown-gnu

ASFLAGS = $(TARGET_MACH)
CFLAGS  += -Wall $(OPTIMIZE) $(EXTRA_CFLAGS)
LDFLAGS += -g $(EXTRA_LDFLAGS)

#------------------------------------------------------------------------------
# Targets that be specified when invoking 'make'
# The use of double-colons allows the "local.mk" file to extend all targets.
#------------------------------------------------------------------------------
.PHONY: $(OS_NAME) all clean help debug d download source tags text \
	tarball bootable salone depend depends

all:: $(DLI)

$(OS_NAME):: $(DLI)

clean::
	$(CMD_DELETE) $(CLEAN_FILES)

debug:: CFLAGS += -DDEBUG -g
debug:: clean all

d:: download
download::
	$(CMD_DOWNLOAD) $(DLI)

tags::
	$(CMD_TAGS) $(SRC)

text:: $(DLI)
	objdump386 --disassemble --file-headers --reloc --source $(DLI) > $(DLI).asm
	@echo "Image dis-assembly into $(DLI).asm done."

# Uncommon build targets
tarball:
	tar cvf ../$(OS_NAME).tar $(SRC) *.h Makefile

bootable:: $(DLI) $(OS_NAME).ini RAWRITE.EXE
	bootmaker -floppy $(OS_NAME).ini $(OS_NAME).boot
	@echo "Raw copy using \`dd' or RAWRITE.EXE $(OS_NAME).boot to a floppy."
	@echo "   dd if=$(OS_NAME).boot of=/dev/rdiskette bs=512"

salone:: $(DLI) $(OS_NAME).ini BOOT.COM
	bootmaker  $(OS_NAME).ini $(OS_NAME).bin
	@echo "Copy $(OS_NAME).bin and BOOT.COM to a floppy."

# Deprecated targets
source:: $(SRC)
depends:: depend
depend::

help::
	@echo "This Makefile builds $(DLI)."
	@echo "  make all       -- Builds your program $(DLI)"
	@echo "  make clean     -- Remove files which can be regenerated"
	@echo "  make debug     -- Create program ready for symbolic debugging"
	@echo "  make download  -- Download *current* version of $(DLI)"
	@echo "  make tags      -- function tags for Emacs and vi"
	@echo "  make text      -- Dis-assemble $(DLI) into 'dli.asm'"
	@echo ""
	@echo "Uncommon build targets:"
	@echo "  make tarball   -- Create TAR file in parent directory"
	@echo "  make bootable  -- Create self-booting image '$(OS_NAME).boot'"
	@echo "  make salone    -- Create stand-alone version '$(OS_NAME).bin'"
	@echo ""
	@echo "Deprecated build targets (no longer maintained)"
	@echo "  make depend    -- Rebuild dependencies"
	@echo "  make source    -- Retrieve all source files from version control"

$(DLI): $(OBJECTS)
	$(CMD_LINKER) $(LDFLAGS) -o $(DLI) $(OBJECTS)

#------------------------------------------------------------------------------
# Include a local makefile (local.mk) if it exists
# This can be used to override certain definitions
#
# Can add more dependencies to "all".  If extending existing targets,
# you must use double-colon rules (see them above).  These rules are
# executed in the order seen, ie those above, then from "local.mk".
#
# Using "-include local.mk", GNU Make won't complain if file not found.
#------------------------------------------------------------------------------
-include "local.mk"

#------------------------------------------------------------------------------
# End of Makefile
#------------------------------------------------------------------------------
