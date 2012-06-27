#
# $Id: Makefile,v 1.1 2004/11/14 14:03:08 fumi Exp $


OPTIMIZE=

EXE=.exe
TARGET=print-test
OBJS=print-test.o

CXXFLAGS=$(CXXFLAGS_SD) -Wall -I../../include


$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) -L../../ -lrupcommon

print-test.o: src/print-test.cpp
	g++ $(CXXFLAGS) -c $<

PRINT_MN_CODE_OBJS=print-mn-code.o

print-mn-code.o: src/print-mn-code.c
	gcc -c $<

print-mn-code: $(PRINT_MN_CODE_OBJS)
	gcc $(PRINT_MN_CODE_OBJS)

clean:
	-rm print-mn-code.o print-test.o print-test$(EXE) print-mn-code$(EXE)
	-rm -rf ins/ out-aurex/ out-objdump/

test: $(TARGET) print-mn-code
	-mkdir ins/
	./print-mn-code
	./shell/aurex.sh
	./shell/objdump.sh
