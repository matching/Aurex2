
ruputer=/usr/local/ruputer
rupsdk=$(ruputer)/rupsdk

CFLAGS = -O2


TARGET= native.h
OBJS =	common/common.o	\
	common/emu_rupsys.o	\
	common/emu_lcdbios.o	\
	common/emu_lcdfunc.o	\
	common/emu_wbios.o	\
	common/emu_common.o	\
	util/itoa.o		\
	menu/emumenu.o	\
	menu/menu.o	\
	rupmenu/emurupmenu.o	\
	rupmenu/entryitem.o	\
	rupmenu/inputtable.o	\
	rupmenu/rupmenucommon.o	\
	softkey/emusoftkey.o	\
	softkey/softkey.o	\
	softkey/smallen.o	\
	disp/emudispmess.o	\
	disp/dispmess.o		\
	disp/emudispyn.o	\
	disp/dispyn.o		\
	disp/emudisprtc.o	\
	disp/disprtc.o		\
	disp/emudispwait.o	\
	disp/dispwait.o		\
	ruptool/emustrcat1.o	\
	ruptool/strcat1.o	\
	ruptool/emustrcpy1.o	\
	ruptool/strcpy1.o	\
	ruptool/emustrother.o	\
	ruptool/strother.o


CC = $(ruputer)/bin/mn10200-elf-gcc
AS = $(ruputer)/bin/mn10200-elf-as
LD = $(ruputer)/bin/mn10200-elf-ld
AR = $(ruputer)/bin/mn10200-elf-ar
RANLIB = $(ruputer)/bin/mn10200-elf-ranlib
OBJCOPY = $(ruputer)/bin/mn10200-elf-objcopy
OBJDUMP = $(ruputer)/bin/mn10200-elf-objdump
SIZE =  $(ruputer)/bin/mn10200-elf-size

SF2BIN = $(rupsdk)/bin/sf2bin
SETICO = $(rupsdk)/bin/setico


CFLAGS+=-Wall -Wshadow
EXLDFLAGS = -relax

CPPFLAGS = -I$(rupsdk)/inc -Icommon -Iutil
ASFLAGS = -I$(rupsdk)/inc
LDFLAGS = -Ttext 0x10000 -Map $(TARGET).map $(EXLDFLAGS)
LIBRUP = #-L$(rupsdk)/lib -lrupsys -lruptool -llcdbios -lpsdos -lwbios
LIBSTD = -L$(ruputer)/mn10200-elf/lib -lm -lc
LIBGCC = -L$(ruputer)/lib/gcc-lib/mn10200-elf/2.95.3-2 -lgcc

ALLLIB= $(EXLIBOPT) $(LIBRUP) $(LIBSTD) $(LIBGCC)


all: $(TARGET) rupsysaddress.h

$(TARGET): rupsys.bin a.exe tool/native_image.txt
	./a.exe tool/native_image.txt

a.exe: tool/im2cpp.c
	gcc -o $@ $<

SYMBOL_TABLE=rupsys.symbol.txt

rupsysaddress.h: $(SYMBOL_TABLE)
	./tool/sym2header.sh < $(SYMBOL_TABLE) > $@


$(SYMBOL_TABLE): rupsys.elf
	$(OBJDUMP) -x $< > $(SYMBOL_TABLE)

rupsys.bin: rupsys.elf
	$(OBJCOPY) -O binary -R .stack -R .zdata $< $@

rupsys.elf: $(OBJS)
	$(LD) $(LDFLAGS) -e _dummy_entry -o $@ $(OBJS) $(ALLLIB)
	$(SIZE) $@
	$(OBJCOPY) --set-section-flags .bss=CONTENTS,ALLOC -R .stack $@

common/keyin.o: common/keyin.s
common/emu_rupsys.o: common/emu_rupsys.s
common/emu_lcdcall.o: common/emu_lcdcall.s
common/emu_lcdfunc.o: common/emu_lcdfunc.s
menu/emumenu.o: menu/emumenu.s
menu/menu.o: menu/menu.c
util/itoa.o: util/itoa.c

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

.s.o :
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	-rm $(TARGET) rupsys.elf rupsys.bin $(OBJS) a.exe $(SYMBOL_TABLE) rupsysaddress.h *.map
