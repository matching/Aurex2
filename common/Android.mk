LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := common

LOCAL_SRC_FILES  := \
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

LOCAL_CPPFLAGS   := -Icommon/include -Icommon/native -fexceptions
# for debug
#LOCAL_CPPFLAGS += -DDEBUG -include android/include/androidlog.h


LOCAL_LDLIBS     := 

include $(BUILD_STATIC_LIBRARY)

