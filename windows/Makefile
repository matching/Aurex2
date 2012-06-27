
OPTIMIZE=-O2
#-O2 -fomit-frame-pointer

TARGET= aurex.exe

SRCS=	main.cpp	\
	winruputer.cpp \
	sound/winsound.cpp	\
	font/winlcdfont.cpp	\
	font/winlcdfontaa.cpp	\
	font/winlcdfontsjis.cpp	\
	font/winlcdfontaasjis.cpp	\
	findfile/winfindfile.cpp	\
	graphic/winlcdfactory.cpp	\
	graphic/winlcd.cpp	\
	graphic/winlcdflip.cpp	\
	graphic/winlcd102x64.cpp	\
	log/winlogoutputevent.cpp	\
	option/winoption.cpp	\
	option/winpropwindow.cpp	\
	option/winpropwindowlog.cpp



RES= resource/all_resources.coff


OBJS=$(SRCS:%.cpp=%.o)

#	memory/winmemory.o	\

LIBCOMMON=librupcommon.a
LIBCOMMONDIR=lib/common

LIBWIDGET=libwidget.a
LIBWIDGETDIR=lib/widget

INCLUDE=-Iinclude -I../common/include -I../widgetlib/include

CXX=g++
CXXFLAGS=-DWINVER=0x0500 -D_WIN32_WINNT=0x500 -Wall $(INCLUDE) $(OPTIMIZE) -mno-cygwin -mwindows

$(TARGET): commonlib widgetlib resources $(OBJS)
	$(CXX) $(LDFLAGS) -mno-cygwin -mwindows -o $(TARGET) $(OBJS) $(RES) -Wl,-Map,$(TARGET).map  -L$(LIBCOMMONDIR) -lrupcommon -L$(LIBWIDGETDIR) -lwidget -lwinmm -lcomctl32

resources:
	$(MAKE) -C ./resource/

commonlib: $(LIBCOMMONDIR)/$(LIBCOMMON)
	$(MAKE) -C ../common/ TARGET_DIR=../windows/$(LIBCOMMONDIR) CXXFLAGS_SD="-mno-cygwin -mwindows -DHAVE_SDCONFIG" INCLUDE_SD=-I../windows/include

widgetlib: $(LIBWIDGETDIR)/$(LIBWIDGET)
	$(MAKE) -C ../widgetlib/ TARGET_DIR=../windows/$(LIBWIDGETDIR) CXXFLAGS_SD="-mno-cygwin -mwindows -DHAVE_SDCONFIG" INCLUDE_SD=-I../windows/include

$(LIBCOMMONDIR)/$(LIBCOMMON):
	mkdir -p $(LIBCOMMONDIR)

$(LIBWIDGETDIR)/$(LIBWIDGET):
	mkdir -p $(LIBWIDGETDIR)


clean:
	-rm $(OBJS) $(TARGET)
	-rm -rf lib/
	$(MAKE) -C ./resource/ clean


distclean: clean
	-rm $(DEPFILE)
	$(MAKE) -C ../common/ distclean


DEPFILE=makefile.dep
depend: dependlocal dependcommon dependwidget

dependcommon:
	$(MAKE) -C ../common depend

dependwidget:
	$(MAKE) -C ../widgetlib depend

dependlocal:
	for file in $(SRCS); do					\
		cpp -MM $(INCLUDE) $$file ;			\
	done > $(DEPFILE)

-include $(DEPFILE)
