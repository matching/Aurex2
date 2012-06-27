

# OPTIMIZE=-O2 -fomit-frame-pointer
OPTIMIZE=
#-DCPU_DEBUG

INCLUDE=-Iinclude -I../common/include

TARGET=libwidget.a
TARGET_DIR=.
SRCS=	\
	./lcd/sdlcdfactory.cpp \
	./option/widgetoption.cpp

OBJS=	$(TARGET_DIR)/sdlcdfactory.o	\
	$(TARGET_DIR)/widgetoption.o

CXX=g++
CXXFLAGS=$(CXXFLAGS_SD) -Wall $(INCLUDE_SD) $(INCLUDE) $(OPTIMIZE)


$(TARGET_DIR)/$(TARGET): $(OBJS)
#	$(CXX) -o $(TARGET) $(OBJS)
	ar cru $(TARGET_DIR)/$(TARGET) $(OBJS)
	ranlib $(TARGET_DIR)/$(TARGET)

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
