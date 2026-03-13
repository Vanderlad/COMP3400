
CXXFLAGS=-std=c++23 -Wall -Wextra -pedantic -Wold-style-cast -O3 -march=native #-fconcepts-diagnostics-depth=10

all: a06-soln.exe

clean:
	rm -f *.exe

run: a06-soln.exe
	./$<

%.exe : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

