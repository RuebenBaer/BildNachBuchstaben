CXX = g++
CPPFLAGS =  \
	-Wall \
	-pipe \
	-mthreads \
	-O2 \
	-std=gnu++11

INCLUDES =  \
	-I${WXINCLUDE} \
	-I${WXINCLUDESETUP}

DEFS =  \
	-D__WXMSW__ \
	-DwxUSE_UNICODE

#WXLIBPATH = -L${WXLIBPATH}
include wxlibLaptop.mk
LIBS += \
	-lkernel32 \
	-luser32 \
	-lgdi32 \
	-lcomdlg32 \
	-lwinspool \
	-lwinmm \
	-lshell32 \
	-lshlwapi \
	-lcomctl32 \
	-lole32 \
	-loleaut32 \
	-luuid \
	-lrpcrt4 \
	-ladvapi32 \
	-lversion \
	-lws2_32 \
	-lwininet \
	-loleacc \
	-luxtheme

OBJDIR = obj
SRCDIR = src
OBJS = \
	$(OBJDIR)\base.o \
	$(OBJDIR)\bildzerlegung.o
OUT = BIB.exe

.PHONEY: all clean

all: $(OBJS)
	$(CXX) $(CPPFLAGS) $(DEFS) -L$(WXLIBPATH) -o $(OUT) $(OBJS) -s $(LIBS)

$(OBJDIR)\bildzerlegung.o: $(SRCDIR)\bildzerlegung.cpp
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\base.o: $(SRCDIR)\base.cpp $(SRCDIR)\filter.h
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

clean:
	del $(OBJS) $(OUT)
