
TARGET=all_resources.coff


RCS=	menu.rc	\
	property.rc \
	version.rc


INCLUDE=-I../include -I.
WININCLUDE=

all: $(TARGET)


all_resources.rc: $(RCS)
	cat $(RCS) > $@

%.coff: %.rc language.h
	windres $(INCLUDE) $(WININCLUDE) -o $@ -O COFF $<


clean:
	rm $(TARGET) all_resources.rc
