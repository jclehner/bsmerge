CXX = g++
CXXFLAGS = -std=c++0x -Wextra -g
LDFLAGS =
OBJDIR = obj/

all: libbsmerge

libbsmerge: lib/*.hh lib/*.cc 
	$(CXX) $(CXXFLAGS) -Ilib/ lib/*.cc -c 
	ar cru libbsmerge.a *.o
	ranlib libbsmerge.a
