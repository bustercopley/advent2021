MACHINE:=$(subst -, ,$(shell gcc -dumpmachine))
CPU=$(word 1,$(MACHINE))
VENDOR=$(word 2,$(MACHINE))
SYSTEM=$(word 3,$(MACHINE))

# CPU-specific variables
i686_CFLAGS=-mfpmath=sse
x86_64_CFLAGS=-mfpmath=sse

# VENDOR-specific variables
w64_EXEEXT=.exe
w64_RMRF=rd /s /q
w64_CAT=type
w64_CPPFLAGS=-D_WIN32_WINNT=_WIN32_WINNT_WIN10
linux_RMRF=rm -rf
linux_CAT=cat

EXEEXT=$($(VENDOR)_EXEEXT)
RMRF=$($(VENDOR)_RMRF)
COOKIE:=$(shell $($(VENDOR)_CAT) .cookie)

CONFIG=release
CONFIGS=debug release
TARGETS=01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
TARGET=01

# Top-level targets.
all: $(CONFIG)-$(TARGET)-all
test: $(CONFIG)-$(TARGET)-test
debug: $(CONFIG)-$(TARGET)-debug
clean: ; -$(RMRF) .obj

.PHONY: all test debug clean

# Options common to all targets and configs.
SRCDIR=src
WARNFLAGS=-Wall -Wextra -Werror
CPPFLAGS=$($(VENDOR)_CPPFLAGS)
CFLAGS=-march=native -mtune=native $($(VENDOR)_CFLAGS) $($(CPU)_CFLAGS) $(WARNFLAGS)
CXXFLAGS=-std=c++2a
LDFLAGS=
LDLIBS=-lstdc++ -lmpfr -lgmp -lpcre2-8
PCH=precompiled.h
DEBUGGER_ARGS=-ex "set print thread-events off" -ex run -ex "bt full" -ex quit
OBJECTS=re.o

# Target-specific options.
01_OBJECTS=
01_LDLIBS=
01_CPPFLAGS=
01_PCH=

# Config-specific options.
release_CPPFLAGS=-DNDEBUG
release_CFLAGS=-O2
release_CXXFLAGS=
release_LDFLAGS=-s
release_LDLIBS=

debug_SUFFIX=-debug
debug_CPPFLAGS=
debug_CFLAGS=-O0 -ggdb3
debug_CXXFLAGS=
debug_LDFLAGS=
debug_LDLIBS=

# Internal variables.
filename=.obj/$(target)$($(config)_SUFFIX)$(EXEEXT)
objects=$(objdir)/$(target).o $($(target)_OBJECTS:%=$(objdir)/%) $(OBJECTS:%=.obj/$(config)/%)
cppflags=$(CPPFLAGS) $($(target)_CPPFLAGS) $($(config)_CPPFLAGS)
cflags=$(CFLAGS) $($(target)_CFLAGS) $($(config)_CFLAGS)
cxxflags=$(cflags) $(CXXFLAGS) $($(target)_CXXFLAGS) $($(config)_CXXFLAGS)
ldflags=$(LDFLAGS) $($(target)_LDFLAGS) $($(config)_LDFLAGS)
ldlibs=$(LDLIBS) $($(target)_LDLIBS) $($(config)_LDLIBS)
pch_object=$(PCH:%=.obj/$(config)/%.gch)
pch_cppflags=$(PCH:%=-include .obj/$(config)/%)
depends=$(objects:%.o=%.d) $(pch_object:%.gch=%.d)
objdir=.obj/$(config)/$(target)
url=https://adventofcode.com/2021/day/$(target:0%=%)/input

# Rules that depend on target and config.
define target-config-defs
$(config)-$(target)-all: get-$(target) $(filename)
$(config)-$(target)-test: $(config)-$(target)-all ; "./$(filename)" $(ARGS)
$(config)-$(target)-debug: $(config)-$(target)-all ; gdb --quiet $(DEBUGGER_ARGS) --args "./$(filename)" $(ARGS)
$(config)-all: $(config)-$(target)-all

$(filename): $(objects)
	$(CXX) -o $$@ $(cxxflags) $(ldflags) $$^ $(ldlibs)

$(objdir)/%.o: $(SRCDIR)/%.cpp $(pch_object) $(objdir)/%.d | $(objdir)
	$(CXX) -c -o $$@ -MMD -MP $(cppflags) $(pch_cppflags) $(cxxflags) $$<

.obj/$(config)/%.o: $(SRCDIR)/%.cpp $(pch_object) .obj/$(config)/%.d | .obj/$(config)
	$(CXX) -c -o $$@ -MMD -MP $(cppflags) $(pch_cppflags) $(cxxflags) $$<

.obj/$(config)/%.gch: $(SRCDIR)/% | .obj/$(config)
	$(CXX) -MMD $(cppflags) $(cxxflags) -x c++-header $$< -c -o $$@

.PHONY: $(config)-$(target)-all $(config)-$(target)-test $(config)-$(target)-debug $(config)-all
.SECONDARY: $(pch_object)
.obj/$(config)/$(target): | .obj/$(config) ; -mkdir "$$@"
# Automatic prerequisites.
include $(wildcard $(depends))
$(depends):
endef

define target-defs
get-$(target): input/$(target)
input/$(target): ; curl -s -o "input/$(target)" "$(url)" -X GET -H "Cookie: session=$(COOKIE)"
endef

define config-defs
$(foreach target,$(TARGETS),$(eval $(target-config-defs)))
.obj/$(config): | .obj ; -mkdir "$$@"
endef

$(foreach config,$(CONFIGS),$(eval $(config-defs)))
$(foreach target,$(TARGETS),$(eval $(target-defs)))
.obj: ; -mkdir "$@"
