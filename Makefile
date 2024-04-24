### Generated by Winemaker 0.8.4
###
### Invocation command line was
### /usr/bin/winemaker . -I./. -igdi32


SRCDIR                = .
SUBDIRS               =
DLLS                  =
LIBS                  =
EXES                  = code.exe



### Common settings

CEXTRA                = -mno-cygwin
CXXEXTRA              =
RCEXTRA               =
DEFINES               =
INCLUDE_PATH          = -I./.
DLL_PATH              =
DLL_IMPORTS           = gdi32
LIBRARY_PATH          =
LIBRARIES             =


### code.exe sources and settings

code_exe_MODULE       = code.exe
code_exe_C_SRCS       =
code_exe_CXX_SRCS     = entry/main.cpp
code_exe_RC_SRCS      =
code_exe_LDFLAGS     += -mwindows \
			-mno-cygwin\
			-nostdlib\
			-kernel32\
			-user32\
    	-fno-stack-protector
	
code_exe_ARFLAGS      =
code_exe_DLL_PATH     =
code_exe_DLLS         = odbc32 \
			ole32 \
			oleaut32 \
			winspool \
			odbccp32
code_exe_LIBRARY_PATH =
code_exe_LIBRARIES    = uuid

code_exe_OBJS         = $(code_exe_C_SRCS:.c=.o) \
			$(code_exe_CXX_SRCS:.cpp=.o) \
			$(code_exe_RC_SRCS:.rc=.res)



### Global source lists

C_SRCS                = $(code_exe_C_SRCS)
CXX_SRCS              = $(code_exe_CXX_SRCS)
RC_SRCS               = $(code_exe_RC_SRCS)


### Tools

CC = winegcc
CXX = wineg++
RC = wrc
AR = ar


### Generic targets

all: $(SUBDIRS) $(DLLS:%=%.so) $(LIBS) $(EXES)

### Build rules

.PHONY: all clean dummy

$(SUBDIRS): dummy
	@cd $@ && $(MAKE)

# Implicit rules

.SUFFIXES: .cpp .cxx .rc .res
DEFINCL = $(INCLUDE_PATH) $(DEFINES) $(OPTIONS)

.c.o:
	$(CC) -c $(CFLAGS) $(CEXTRA) $(DEFINCL) -o $@ $<

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CXXEXTRA) $(DEFINCL) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(CXXEXTRA) $(DEFINCL) -o $@ $<

.rc.res:
	$(RC) $(RCFLAGS) $(RCEXTRA) $(DEFINCL) -fo$@ $<

# Rules for cleaning

CLEAN_FILES     = y.tab.c y.tab.h lex.yy.c core *.orig *.rej \
                  \\\#*\\\# *~ *% .\\\#*

clean:: $(SUBDIRS:%=%/__clean__) $(EXTRASUBDIRS:%=%/__clean__)
	$(RM) $(CLEAN_FILES) $(RC_SRCS:.rc=.res) $(C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o)
	$(RM) $(DLLS:%=%.so) $(LIBS) $(EXES) $(EXES:%=%.so)

$(SUBDIRS:%=%/__clean__): dummy
	cd `dirname $@` && $(MAKE) clean

$(EXTRASUBDIRS:%=%/__clean__): dummy
	-cd `dirname $@` && $(RM) $(CLEAN_FILES)

### Target specific build rules
DEFLIB = $(LIBRARY_PATH) $(LIBRARIES) $(DLL_PATH) $(DLL_IMPORTS:%=-l%)

$(code_exe_MODULE): $(code_exe_OBJS)
	$(CXX) $(code_exe_LDFLAGS) -o $@ $(code_exe_OBJS) $(code_exe_LIBRARY_PATH) $(code_exe_DLL_PATH) $(DEFLIB) $(code_exe_DLLS:%=-l%) $(code_exe_LIBRARIES:%=-l%)


