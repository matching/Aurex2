# cd ../bdflib
# make depdnd
# cd parse
# make depend
# cd ../curses
# make depend
# make
#


TARGET_DIR=.

OPTIMIZE=

TARGET=$(TARGET_DIR)/libbdfparser.a
SRCS=parse.c bdf.tab.c lex.yy.c


OBJS=	$(TARGET_DIR)/parse.o \
	$(TARGET_DIR)/bdf.tab.o \
	$(TARGET_DIR)/lex.yy.o

CXXFLAGS= $(OPTIMIZE) -I../include -U__GNUC_MINOR__
INCLUDE=-I../include
CXX=g++
FLEXFLAGS=-F

## debug...
# CFLAGS=-DYYDEBUG=1 -DYYERROR_VERBOSE
# FLEXFLAGS=-d

#TARGET=aaa.exe
#$(TARGET): $(OBJS)
#	gcc -o $(TARGET) $(CFLAGS) $(OBJS)
# all: $(OBJS)

$(TARGET): $(OBJS)
	ar cru $(TARGET) $(OBJS)

%.o: %.c
	$(CXX) -c $(CXXFLAGS) $<

lex.yy.c: bdf.l bdf.tab.c
	flex $(FLEXFLAGS) $<

bdf.tab.c: bdf.y
	bison -d $<

clean:
	rm $(TARGET) bdf.tab.c lex.yy.c bdf.tab.h $(OBJS)

DEPFILE=makefile.dep
depend:
	for file in $(SRCS); do					\
		cpp -MM $(INCLUDE) -x c++ $$file ;			\
		echo '	$$(CXX) $$(CXXFLAGS) -o $$@ -c $$<' ;	\
	done | sed -e '/:/s/\(.*\)/$$(TARGET_DIR)\/\1/' > $(DEPFILE)

-include $(DEPFILE)
