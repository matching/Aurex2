

# OPTIMIZE=-O2 -fomit-frame-pointer
OPTIMIZE=
#-DCPU_DEBUG

TARGET=librupcommon.a
TARGET_DIR=.
SRCS=	\
	./coding/codingbase.cpp	\
	./coding/codingascii.cpp	\
	./coding/codingshiftjis.cpp	\
	./cpu/cpu.cpp	\
	./cpu/mncore.cpp	\
	./cpu/mnexpr.cpp	\
	./cpu/mnprint.cpp	\
	./lcd/lcd.cpp		\
	./lcd/lcdfunc.cpp	\
	./lcd/sdlcd.cpp		\
	./lcd/cmlcd.cpp		\
	./lcd/cmlcdfunc.cpp		\
	./lcd/cmlcdfuncaa.cpp		\
	./lcd/cmlcdfuncdefault.cpp		\
	./lcd/cmlcdpix.cpp	\
	./lcd/cmlcdpixaa.cpp	\
	./lcd/cmlcdpixaa_noai.cpp	\
	./lcd/cmlcdpixdefault.cpp	\
	./lcd/cmlcdpixdefault_noai.cpp	\
	./lcd/blink.cpp		\
	./lcd/blinkmgr.cpp	\
	./lcd/reverse.cpp	\
	./lcd/reversemgr.cpp	\
	./lcd/lcdblrv.cpp	\
	./lcd/lcdblrvmgr.cpp	\
	./lcd/lcdport.cpp	\
	./lcd/aapixel.cpp	\
	./log/log.cpp		\
	./log/logoutput.cpp	\
	./log/logoutputstderr.cpp 	\
	./log/logoutputfile.cpp		\
	./filerini/filerinibase.cpp	\
	./filerini/filerinimgr.cpp	\
	./filerini/ufilerini.cpp	\
	./filerini/extlist.cpp		\
	./font/sdlcdfont.cpp	\
	./font/cessjis.cpp	\
	./main/ruputer.cpp	\
	./main/ruputerexception.cpp	\
	./main/rupaddress.cpp	\
	./main/rupstruct.cpp	\
	./mmp/rommmp.cpp	\
	./mmp/romgaiji.cpp	\
	./other/frame.cpp	\
	./other/key.cpp		\
	./other/keybuffer.cpp	\
	./other/interruption.cpp	\
	./other/exffile.cpp		\
	./psdos/x86reg.cpp		\
	./psdos/fdmgr.cpp		\
	./psdos/psdos_execute.cpp	\
	./psdos/psdos_file.cpp		\
	./psdos/psdos_findfile.cpp	\
	./psdos/sdfindfile.cpp		\
	./psdos/psdos.cpp		\
	./wbios/wbios.cpp		\
	./wbios/wbiossound.cpp		\
	./memory/memory.cpp		\
	./memory/memorybase.cpp		\
	./memory/memoryinside.cpp	\
	./memory/port.cpp		\
	./memory/memorycpu.cpp		\
	./memory/internalfont.cpp	\
	./memory/exmemory.cpp		\
	./option/optionitem.cpp		\
	./option/optionlist.cpp		\
	./option/option.cpp		\
	./option/item/optionbool.cpp		\
	./option/item/optionint.cpp		\
	./option/item/optionstring.cpp		\
	./timer/timer.cpp		\
	./timer/timermgr.cpp		\
	./timer/timerresource.cpp		\
	./timer/timerresourcemgr.cpp		\
	./rupsys/rupsys.cpp		\
	./rupsys/systimer.cpp		\
	./thread/thread.cpp		\
	./sound/sound.cpp		\
	./sound/sdsound.cpp		\
	./sound/soundsync.cpp		\
	./sound/soundsyncnull.cpp	\
	./sound/soundasync.cpp		\
	./directory/directory.cpp		\
	./directory/directoryconverter.cpp	\
	./directory/dirstack.cpp		\
	./directory/dirstacksjis.cpp		\
	./directory/directoryelement.cpp	\
	./directory/dosdirectory.cpp		\
	./directory/ruputerdirectory.cpp	\
	./directory/unixdirectory.cpp

OBJS=	$(TARGET_DIR)/codingbase.o	\
	$(TARGET_DIR)/codingascii.o	\
	$(TARGET_DIR)/codingshiftjis.o	\
	$(TARGET_DIR)/cpu.o	\
	$(TARGET_DIR)/mnexpr.o	\
	$(TARGET_DIR)/mncore.o	\
	$(TARGET_DIR)/mnprint.o	\
	$(TARGET_DIR)/cmlcd.o		\
	$(TARGET_DIR)/cmlcdfunc.o		\
	$(TARGET_DIR)/cmlcdfuncaa.o		\
	$(TARGET_DIR)/cmlcdfuncdefault.o		\
	$(TARGET_DIR)/cmlcdpix.o		\
	$(TARGET_DIR)/cmlcdpixaa.o		\
	$(TARGET_DIR)/cmlcdpixaa_noai.o		\
	$(TARGET_DIR)/cmlcdpixdefault.o		\
	$(TARGET_DIR)/cmlcdpixdefault_noai.o		\
	$(TARGET_DIR)/aapixel.o		\
	$(TARGET_DIR)/lcd.o		\
	$(TARGET_DIR)/lcdfunc.o	\
	$(TARGET_DIR)/lcdport.o	\
	$(TARGET_DIR)/log.o		\
	$(TARGET_DIR)/logoutput.o	\
	$(TARGET_DIR)/logoutputstderr.o 	\
	$(TARGET_DIR)/logoutputfile.o		\
	$(TARGET_DIR)/blink.o	\
	$(TARGET_DIR)/blinkmgr.o	\
	$(TARGET_DIR)/reverse.o	\
	$(TARGET_DIR)/reversemgr.o	\
	$(TARGET_DIR)/lcdblrv.o	\
	$(TARGET_DIR)/lcdblrvmgr.o	\
	$(TARGET_DIR)/filerinibase.o	\
	$(TARGET_DIR)/filerinimgr.o	\
	$(TARGET_DIR)/ufilerini.o	\
	$(TARGET_DIR)/extlist.o		\
	$(TARGET_DIR)/sdlcd.o		\
	$(TARGET_DIR)/sdlcdfont.o	\
	$(TARGET_DIR)/cessjis.o		\
	$(TARGET_DIR)/rupaddress.o	\
	$(TARGET_DIR)/rupstruct.o	\
	$(TARGET_DIR)/ruputer.o	\
	$(TARGET_DIR)/ruputerexception.o	\
	$(TARGET_DIR)/memorycpu.o	\
	$(TARGET_DIR)/memory.o	\
	$(TARGET_DIR)/memorybase.o	\
	$(TARGET_DIR)/memoryinside.o	\
	$(TARGET_DIR)/rommmp.o		\
	$(TARGET_DIR)/romgaiji.o	\
	$(TARGET_DIR)/internalfont.o	\
	$(TARGET_DIR)/exmemory.o	\
	$(TARGET_DIR)/port.o		\
	$(TARGET_DIR)/interruption.o	\
	$(TARGET_DIR)/optionitem.o	\
	$(TARGET_DIR)/optionlist.o	\
	$(TARGET_DIR)/option.o		\
	$(TARGET_DIR)/optionbool.o	\
	$(TARGET_DIR)/optionint.o	\
	$(TARGET_DIR)/optionstring.o	\
	$(TARGET_DIR)/frame.o		\
	$(TARGET_DIR)/key.o		\
	$(TARGET_DIR)/keybuffer.o	\
	$(TARGET_DIR)/psdos.o		\
	$(TARGET_DIR)/x86reg.o	\
	$(TARGET_DIR)/fdmgr.o		\
	$(TARGET_DIR)/rupsys.o	\
	$(TARGET_DIR)/systimer.o	\
	$(TARGET_DIR)/timer.o	\
	$(TARGET_DIR)/timermgr.o	\
	$(TARGET_DIR)/timerresource.o	\
	$(TARGET_DIR)/timerresourcemgr.o	\
	$(TARGET_DIR)/thread.o	\
	$(TARGET_DIR)/psdos_execute.o	\
	$(TARGET_DIR)/psdos_file.o	\
	$(TARGET_DIR)/psdos_findfile.o	\
	$(TARGET_DIR)/sdfindfile.o	\
	$(TARGET_DIR)/wbios.o		\
	$(TARGET_DIR)/wbiossound.o	\
	$(TARGET_DIR)/sound.o		\
	$(TARGET_DIR)/sdsound.o		\
	$(TARGET_DIR)/soundsync.o	\
	$(TARGET_DIR)/soundsyncnull.o	\
	$(TARGET_DIR)/soundasync.o	\
	$(TARGET_DIR)/directory.o		\
	$(TARGET_DIR)/directoryconverter.o	\
	$(TARGET_DIR)/dirstack.o		\
	$(TARGET_DIR)/dirstacksjis.o		\
	$(TARGET_DIR)/directoryelement.o	\
	$(TARGET_DIR)/dosdirectory.o		\
	$(TARGET_DIR)/ruputerdirectory.o	\
	$(TARGET_DIR)/unixdirectory.o	\
	$(TARGET_DIR)/exffile.o


CXX=g++
INCLUDE=-Iinclude -Inative
CXXFLAGS=$(CXXFLAGS_SD) -Wall $(INCLUDE_SD) $(INCLUDE) $(OPTIMIZE)



$(TARGET_DIR)/$(TARGET): $(OBJS)
#	$(CXX) -o $(TARGET) $(OBJS)
	$(AR) cru $(TARGET_DIR)/$(TARGET) $(OBJS)
	$(RANLIB) $(TARGET_DIR)/$(TARGET)

distclean:
	-rm $(OBJS) $(TARGET)
	-rm $(DEPFILE)

clean:
	-rm $(OBJS) $(TARGET)

#	make -C native clean

DEPFILE=makefile.dep
depend:
	for file in $(SRCS); do					\
		cpp -MM $(INCLUDE) $$file ;			\
		echo '	$$(CXX) $$(CXXFLAGS) -o $$@ -c $$<' ;	\
	done | sed -e '/:/s/\(.*\)/$$(TARGET_DIR)\/\1/' > $(DEPFILE)

-include $(DEPFILE)

#
#
# $(TARGET_DIR)/exmemory.o: memory/exmemory.cpp native/native.h
# 	$(CXX) $(CXXFLAGS) -o $@ -c $<
#
# native/rupsysaddress.h: native_header
# native/native..h: native_header
# native_header:
# #	make -C native/
#
# $(TARGET_DIR)/rupsys.o: rupsys/rupsys.cpp include/cpu.h include/mncore.h \
#   include/memory.h include/thread.h include/address.h include/rupsys.h \
#   include/ruputer.h include/keybuffer.h include/state.h \
#   include/systimer.h include/rupaddress.h native/rupsysaddress.h
# 	$(CXX) $(CXXFLAGS) -o $@ -c $<

