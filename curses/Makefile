
OPTIMIZE=
#-O2 -fomit-frame-pointer

TARGET= aurex.exe

SRCS=	main.cpp	\
	curruputer.cpp	\
	graphic/curlcd.cpp	\
	log/curlogoutput.cpp

OBJS=$(SRCS:%.cpp=%.o)

LIBCOMMON=librupcommon.a
LIBCOMMONDIR=lib

LIBBDF=libbdffont.a
LIBBDFDIR=bdflib

LIBUNIX=libunixlib.a
LIBUNIXDIR=unixlib

INCLUDE=-Iinclude -I../common/include -I../bdflib/include -I../unixlib/include

CXX=g++
CXXFLAGS=-Wall $(INCLUDE) $(OPTIMIZE)


$(TARGET): commonlib bdflib unixlib $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) -L$(LIBUNIXDIR) -lunixlib  -L$(LIBBDFDIR) -lbdffont -lbdfparser -L$(LIBCOMMONDIR) -lrupcommon -lcurses -Wl,-Map,aurex.map

commonlib: $(LIBCOMMONDIR)/$(LIBCOMMON)
	$(MAKE) -C ../common/ TARGET_DIR=../curses/$(LIBCOMMONDIR) CXXFLAGS_SD="-DHAVE_SDCONFIG" INCLUDE_SD=-I../curses/include

$(LIBCOMMONDIR)/$(LIBCOMMON):
	mkdir -p $(LIBCOMMONDIR)


bdflib: $(LIBBDFDIR)/$(LIBBDF)
	$(MAKE) -C ../bdflib/ TARGET_DIR=../curses/$(LIBBDFDIR)

unixlib: $(LIBUNIXDIR)/$(LIBUNIX)
	$(MAKE) -C ../unixlib/ TARGET_DIR=../curses/$(LIBUNIXDIR)

$(LIBBDFDIR)/$(LIBBDF):
	mkdir -p $(LIBBDFDIR)

$(LIBUNIXDIR)/$(LIBUNIX):
	mkdir -p $(LIBUNIXDIR)

clean:
	-rm $(OBJS) $(TARGET)
	-rm -rf $(LIBCOMMONDIR) $(LIBBDFDIR)

distclean: clean
	-rm $(DEPFILE)
	$(MAKE) -C ../common/ distclean




DEPFILE=makefile.dep
depend: dependlocal dependcommon dependunix

dependcommon:
	$(MAKE) -C ../common depend

dependunix:
	$(MAKE) -C ../$(LIBUNIXDIR) depend

dependlocal:
	for file in $(SRCS); do					\
		cpp -MM $(INCLUDE) $$file ;			\
	done > $(DEPFILE)

-include $(DEPFILE)
