

# PATH=/usr/local/gcc2/bin:$PATH make
# /opt/Embedix/tools/bin/arm-linux-strip qtaurex


OPTIMIZE=
#-O2 -fomit-frame-pointer

TARGET= qtaurex

MOC_SRCS=include/qteruputer.h include/qtefiledialog.h
MOC_OBJS=include/moc_qteruputer.o include/moc_qtefiledialog.o

SRCS=	main.cpp		\
	qteruputer.cpp 		\
	graphic/qtelcd.cpp	\
	font/qtelcdfont.cpp	\
	filedialog/qtefiledialog.cpp

OBJS=$(SRCS:%.cpp=%.o) $(MOC_OBJS)

LIBCOMMON=librupcommon.a
LIBCOMMONDIR=lib

LIBUNIX=libunixlib.a
LIBUNIXDIR=unixlib

INCLUDE=-Iinclude -I../common/include -I../unixlib/include

QTBASE=/opt/Qtopia
# QTBASE=/usr/local/zaurus/devel/Qtopia

QTINCLUDE=$(QTBASE)/include
QTLIBDIR=$(QTBASE)/lib
QTLIBS=-lqpe #-lqte
# QTLIBS=-lzdtm -lsl -lqpe -lqte

MOC=$(QTBASE)/bin/moc

DEFINES=-DOPTION_DEFAULT_EXPANTION=2


## file dialog test
DEFINES += -DFILEDIALOG
QTLIBS  +=-lzdtm -lsl

CXX=g++
CXXFLAGS=-pthread -DQWS -fno-rtti -fno-exceptions -DNO_DEBUG -Wall $(INCLUDE) -I$(QTINCLUDE) $(OPTIMIZE) $(DEFINES)

CXXFLAGS_SD="-pthread $(DEFINES)"

$(TARGET): commonlib unixlib $(OBJS)
#	$(CXX) --mno-cygwin -mwindows -o $(TARGET) $(OBJS)  lib/librupcommon.a
	LD_LIBRARY_PATH=$(QTLIBDIR)  && \
	$(CXX) -o $(TARGET) $(OBJS)  -lpthread -L$(LIBUNIXDIR) -lunixlib -L$(LIBCOMMONDIR) -lrupcommon -L$(QTLIBDIR) $(QTLIBS) -Wl,-rpath=$(QTLIBDIR)

commonlib: $(LIBCOMMONDIR)/$(LIBCOMMON)
	make -C ../common/ TARGET_DIR=../qte/$(LIBCOMMONDIR) CXXFLAGS_SD=$(CXXFLAGS_SD) INCLUDE_SD=-I../qte/include

$(LIBCOMMONDIR)/$(LIBCOMMON):
	mkdir -p $(LIBCOMMONDIR)

unixlib: $(LIBUNIXDIR)/$(LIBUNIX)
	$(MAKE) -C ../unixlib/ TARGET_DIR=../qte/$(LIBUNIXDIR) CXXFLAGS_SD=$(CXXFLAGS_SD)

$(LIBUNIXDIR)/$(LIBUNIX):
	mkdir -p $(LIBUNIXDIR)


clean:
	-rm $(OBJS) $(TARGET)
	-rm -rf lib/ $(LIBUNIXDIR)

distclean: clean
	-rm $(DEPFILE)
	make -C ../common/ distclean


DEPFILE=makefile.dep
depend: dependlocal dependcommon dependunix

dependcommon:
	make -C ../common depend

dependunix:
	$(MAKE) -C ../unixlib depend

dependlocal:
	-for file in $(SRCS); do					\
		cpp -MM $(INCLUDE) $$file ;			\
	done > $(DEPFILE)

include/moc_qteruputer.cpp: include/qteruputer.h
	$(MOC) $< -o $@

include/moc_qtefiledialog.cpp: include/qtefiledialog.h
	$(MOC) $< -o $@

-include $(DEPFILE)
