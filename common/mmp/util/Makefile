
TARGET=../mmpdata.h ../gaijidata.h

CONVERT_SRC_MMP=aurexmmp.txt
CONVERT_SRC_GAIJI=aurexgaiji.txt


all: $(TARGET)

../mmpdata.h: convert $(CONVERT_SRC_MMP)
	./convert -mmp < $(CONVERT_SRC_MMP) | ./makemmp.sh > $@


../gaijidata.h: convert $(CONVERT_SRC_GAIJI)
	./convert -gaiji < $(CONVERT_SRC_GAIJI) | ./makegaiji.sh > $@



convert: convert.c
	gcc -o $@ $<


clean:
	-rm convert $(TARGET)