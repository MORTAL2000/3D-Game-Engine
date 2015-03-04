# GNU Make project makefile autogenerated by Premake
ifndef config
  config=release32
endif

ifndef verbose
  SILENT = @
endif

ifndef CC
  CC = gcc
endif

ifndef CXX
  CXX = g++
endif

ifndef AR
  AR = ar
endif

ifeq ($(config),release32)
  OBJDIR     = obj/x32/Release/lua-5.2.3
  TARGETDIR  = ../../lib
  TARGET     = $(TARGETDIR)/liblua-5.2.3_gmake.a
  DEFINES   += -DLUA_COMPAT_ALL
  INCLUDES  += -I../../btgui/lua-5.2.3/src
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -O2 -msse2 -ffast-math -m32
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -s -m32 -L/usr/lib32
  LIBS      += 
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug32)
  OBJDIR     = obj/x32/Debug/lua-5.2.3
  TARGETDIR  = ../../lib
  TARGET     = $(TARGETDIR)/liblua-5.2.3_gmake_debug.a
  DEFINES   += -D_DEBUG=1 -DLUA_COMPAT_ALL
  INCLUDES  += -I../../btgui/lua-5.2.3/src
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -g -ffast-math -m32
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -m32 -L/usr/lib32
  LIBS      += 
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release64)
  OBJDIR     = obj/x64/Release/lua-5.2.3
  TARGETDIR  = ../../lib
  TARGET     = $(TARGETDIR)/liblua-5.2.3_gmake_x64_release.a
  DEFINES   += -DLUA_COMPAT_ALL
  INCLUDES  += -I../../btgui/lua-5.2.3/src
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -O2 -msse2 -ffast-math -m64
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -s -m64 -L/usr/lib64
  LIBS      += 
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug64)
  OBJDIR     = obj/x64/Debug/lua-5.2.3
  TARGETDIR  = ../../lib
  TARGET     = $(TARGETDIR)/liblua-5.2.3_gmake_x64_debug.a
  DEFINES   += -D_DEBUG=1 -DLUA_COMPAT_ALL
  INCLUDES  += -I../../btgui/lua-5.2.3/src
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -g -ffast-math -m64
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -m64 -L/usr/lib64
  LIBS      += 
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

OBJECTS := \
	$(OBJDIR)/lapi.o \
	$(OBJDIR)/lauxlib.o \
	$(OBJDIR)/lbaselib.o \
	$(OBJDIR)/lbitlib.o \
	$(OBJDIR)/lcode.o \
	$(OBJDIR)/lcorolib.o \
	$(OBJDIR)/lctype.o \
	$(OBJDIR)/ldblib.o \
	$(OBJDIR)/ldebug.o \
	$(OBJDIR)/ldo.o \
	$(OBJDIR)/ldump.o \
	$(OBJDIR)/lfunc.o \
	$(OBJDIR)/lgc.o \
	$(OBJDIR)/linit.o \
	$(OBJDIR)/liolib.o \
	$(OBJDIR)/llex.o \
	$(OBJDIR)/lmathlib.o \
	$(OBJDIR)/lmem.o \
	$(OBJDIR)/loadlib.o \
	$(OBJDIR)/lobject.o \
	$(OBJDIR)/lopcodes.o \
	$(OBJDIR)/loslib.o \
	$(OBJDIR)/lparser.o \
	$(OBJDIR)/lstate.o \
	$(OBJDIR)/lstring.o \
	$(OBJDIR)/lstrlib.o \
	$(OBJDIR)/ltable.o \
	$(OBJDIR)/ltablib.o \
	$(OBJDIR)/ltm.o \
	$(OBJDIR)/lundump.o \
	$(OBJDIR)/lvm.o \
	$(OBJDIR)/lzio.o \

RESOURCES := \

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif

.PHONY: clean prebuild prelink

all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking lua-5.2.3
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning lua-5.2.3
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(GCH): $(PCH)
	@echo $(notdir $<)
	-$(SILENT) cp $< $(OBJDIR)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
endif

$(OBJDIR)/lapi.o: ../../btgui/lua-5.2.3/src/lapi.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lauxlib.o: ../../btgui/lua-5.2.3/src/lauxlib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lbaselib.o: ../../btgui/lua-5.2.3/src/lbaselib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lbitlib.o: ../../btgui/lua-5.2.3/src/lbitlib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lcode.o: ../../btgui/lua-5.2.3/src/lcode.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lcorolib.o: ../../btgui/lua-5.2.3/src/lcorolib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lctype.o: ../../btgui/lua-5.2.3/src/lctype.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ldblib.o: ../../btgui/lua-5.2.3/src/ldblib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ldebug.o: ../../btgui/lua-5.2.3/src/ldebug.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ldo.o: ../../btgui/lua-5.2.3/src/ldo.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ldump.o: ../../btgui/lua-5.2.3/src/ldump.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lfunc.o: ../../btgui/lua-5.2.3/src/lfunc.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lgc.o: ../../btgui/lua-5.2.3/src/lgc.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/linit.o: ../../btgui/lua-5.2.3/src/linit.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/liolib.o: ../../btgui/lua-5.2.3/src/liolib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/llex.o: ../../btgui/lua-5.2.3/src/llex.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lmathlib.o: ../../btgui/lua-5.2.3/src/lmathlib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lmem.o: ../../btgui/lua-5.2.3/src/lmem.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/loadlib.o: ../../btgui/lua-5.2.3/src/loadlib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lobject.o: ../../btgui/lua-5.2.3/src/lobject.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lopcodes.o: ../../btgui/lua-5.2.3/src/lopcodes.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/loslib.o: ../../btgui/lua-5.2.3/src/loslib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lparser.o: ../../btgui/lua-5.2.3/src/lparser.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lstate.o: ../../btgui/lua-5.2.3/src/lstate.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lstring.o: ../../btgui/lua-5.2.3/src/lstring.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lstrlib.o: ../../btgui/lua-5.2.3/src/lstrlib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ltable.o: ../../btgui/lua-5.2.3/src/ltable.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ltablib.o: ../../btgui/lua-5.2.3/src/ltablib.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ltm.o: ../../btgui/lua-5.2.3/src/ltm.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lundump.o: ../../btgui/lua-5.2.3/src/lundump.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lvm.o: ../../btgui/lua-5.2.3/src/lvm.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"
$(OBJDIR)/lzio.o: ../../btgui/lua-5.2.3/src/lzio.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(CFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
