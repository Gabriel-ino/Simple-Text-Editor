TARGET=editor
CXX=g++
DEBUG=-g
OPT=-O0
WARN=-Wall -Wno-unknown-pragmas
NCURSES=-lncurses -ltinfo
CXXFLAGS=$(DEBUG) $(OPT) $(WARN) $(NCURSES) -pipe
LD=g++
LDFLAGS=$(NCURSES)
OBJS= main.o editor.o
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@rm -rf *.o
 
main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) main.cpp -o main.o
 
editor.o: editor.cpp
	$(CXX) -c $(CXXFLAGS) editor.cpp  -o editor.o
