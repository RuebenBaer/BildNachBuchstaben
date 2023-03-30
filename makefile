CXX = D:\mingw64\bin\g++
CPPFLAGS =  \
	-Wall \
	-pipe \
	-mthreads \
	-O2 \
	-std=gnu++11

INCLUDES =  \
	-ID:\wxWidgets-3.2.1\include \
	-ID:\wxWidgets-3.2.1\lib\gcc_dll-dll_64\mswu

DEFS =  \
	-D__WXMSW__ \
	-DwxUSE_UNICODE

WXLIBPATH = -LD:\wxWidgets-3.2.1\lib\gcc_dll-dll_64
LIBS = \
	-lwxbase32u \
	-lwxbase32u_net \
	-lwxbase32u_xml \
	-lwxexpat \
	-lwxjpeg \
	-lwxmsw32u_adv \
	-lwxmsw32u_aui \
	-lwxmsw32u_core \
	-lwxmsw32u_gl \
	-lwxmsw32u_html \
	-lwxmsw32u_media \
	-lwxmsw32u_propgrid \
	-lwxmsw32u_ribbon \
	-lwxmsw32u_richtext \
	-lwxmsw32u_stc \
	-lwxmsw32u_webview \
	-lwxmsw32u_xrc \
	-lwxpng \
	-lwxregexu \
	-lwxscintilla \
	-lwxtiff \
	-lwxzlib \
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
	$(CXX) $(CPPFLAGS) $(DEFS) $(WXLIBPATH) -o $(OUT) $(OBJS) -s $(LIBS)

$(OBJDIR)\bildzerlegung.o: $(SRCDIR)\bildzerlegung.cpp
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(OBJDIR)\base.o: $(SRCDIR)\base.cpp
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

clean:
	del $(OBJS) $(OUT)
