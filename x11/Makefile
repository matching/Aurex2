
OPTIMIZE=
#-O2 -fomit-frame-pointer

TARGET= x11aurex
SRCS=	main.cpp		\
	x11ruputer.cpp 		\
	graphic/x11lcd.cpp	\
	font/x11lcdfont.cpp	\
	font/x11lcdfontsjis.cpp	\
	font/x11fontset.cpp

OBJS=$(SRCS:%.cpp=%.o)


LIBCOMMON=librupcommon.a
LIBCOMMONDIR=lib

LIBUNIX=libunixlib.a
LIBUNIXDIR=unixlib

INCLUDE=-Iinclude -I../common/include -I../unixlib/include

X11_INCLUDE=-I/usr/X11R6/include
X11_LIBS=-L/usr/X11R6/lib -lX11

CXX=g++
CXXFLAGS=-Wall $(INCLUDE) $(X11_INCLUDE) $(OPTIMIZE)

$(TARGET): commonlib unixlib $(OBJS)
#	$(CXX) --mno-cygwin -mwindows -o $(TARGET) $(OBJS)  lib/librupcommon.a
	LD_LIBRARY_PATH=$(QTLIBDIR)  && \
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS)  -L$(LIBUNIXDIR) -lunixlib -L$(LIBCOMMONDIR) -lrupcommon $(X11_LIBS)

commonlib: $(LIBCOMMONDIR)/$(LIBCOMMON)
	$(MAKE) -C ../common/ TARGET_DIR=../x11/$(LIBCOMMONDIR) CXXFLAGS_SD=""

$(LIBCOMMONDIR)/$(LIBCOMMON):
	mkdir -p $(LIBCOMMONDIR)

unixlib: $(LIBUNIXDIR)/$(LIBUNIX)
	$(MAKE) -C ../unixlib/ TARGET_DIR=../x11/$(LIBUNIXDIR)

$(LIBUNIXDIR)/$(LIBUNIX):
	mkdir -p $(LIBUNIXDIR)

clean:
	-rm $(OBJS) $(TARGET)
	-rm -rf lib/ $(LIBUNIXDIR)

distclean: clean
	-rm $(DEPFILE)
	$(MAKE) -C ../common/ distclean


DEPFILE=makefile.dep
depend: dependlocal dependcommon dependunixlib

dependcommon:
	$(MAKE) -C ../common depend

dependunixlib:
	$(MAKE) -C ../unixlib depend

dependlocal:
	for file in $(SRCS); do					\
		cpp -MM $(INCLUDE) $$file ;			\
	done > $(DEPFILE)

-include $(DEPFILE)

.PHONY: unixlib
