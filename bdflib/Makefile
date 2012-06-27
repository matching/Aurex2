

# OPTIMIZE=-O2 -fomit-frame-pointer
OPTIMIZE=
#-DCPU_DEBUG

INCLUDE=-Iinclude -I../common/include -Iparser

TARGET=libbdffont.a
TARGET_DIR=.
SRCS=	\
	./bdf/bdffontset.cpp		\
	./bdf/bdffontsets.cpp		\
	./bdf/bdffontsetsjis.cpp	\
	./bdf/bdfmgr.cpp		\
	./font/bdflcdfont.cpp		\
	./font/bdflcdfontjis.cpp

OBJS=	$(TARGET_DIR)/bdffontset.o		\
	$(TARGET_DIR)/bdffontsets.o		\
	$(TARGET_DIR)/bdffontsetsjis.o	\
	$(TARGET_DIR)/bdfmgr.o		\
	$(TARGET_DIR)/bdflcdfont.o		\
	$(TARGET_DIR)/bdflcdfontjis.o

CXX=g++
CXXFLAGS=$(CXXFLAGS_SD) -Wall $(INCLUDE) $(OPTIMIZE)


$(TARGET_DIR)/$(TARGET): parser $(OBJS)
#	$(CXX) -o $(TARGET) $(OBJS)
	ar cru $(TARGET_DIR)/$(TARGET) $(OBJS)
	ranlib $(TARGET_DIR)/$(TARGET)

parser:
	$(MAKE) -C ./parser/ TARGET_DIR=../$(TARGET_DIR)

distclean:
	-rm $(OBJS) $(TARGET)
	-rm $(DEPFILE)

clean:
	-rm $(OBJS) $(TARGET)

DEPFILE=makefile.dep
depend:
	for file in $(SRCS); do					\
		cpp -MM $(INCLUDE) $$file ;			\
		echo '	$$(CXX) $$(CXXFLAGS) -o $$@ -c $$<' ;	\
	done | sed -e '/:/s/\(.*\)/$$(TARGET_DIR)\/\1/' > $(DEPFILE)

-include $(DEPFILE)


.PHONY: parser
