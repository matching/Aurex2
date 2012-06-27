

# OPTIMIZE=-O2 -fomit-frame-pointer
OPTIMIZE=
#-DCPU_DEBUG

INCLUDE=-Iinclude -I../common/include

TARGET=libunixlib.a
TARGET_DIR=.
SRCS=	\
	./findfile/unixfindfile.cpp \
	./unixuserfile.cpp

OBJS=	$(TARGET_DIR)/unixfindfile.o	\
	$(TARGET_DIR)/unixuserfile.o

CXX=g++
CXXFLAGS=$(CXXFLAGS_SD) -Wall $(INCLUDE) $(OPTIMIZE)

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
