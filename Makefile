CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17


EXECUTABLE = main.exe

SOURCES = main.cpp IntComputer.cpp

all: $(EXECUTABLE) tests.exe

$(EXECUTABLE): main.cpp IntComputer.o
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) main.cpp IntComputer.o

tests.exe: tests.cpp IntComputer.o
	$(CXX) $(CXXFLAGS) -o tests.exe tests.cpp IntComputer.o

IntComputer.o: IntComputer.cpp IntComputer.hpp
	$(CXX) $(CXXFLAGS) -o IntComputer.o -c IntComputer.cpp


