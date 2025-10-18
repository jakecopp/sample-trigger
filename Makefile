# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g -Wextra -pedantic -ansi -O3 -std=c++17

# Target executable
# 	$(CXX) `wx-config --cxxflags` $(CXXFLAGS) -o sample_trigger main.cpp sample_trigger.cpp `wx-config --libs`

sample_trigger: main.cpp sample_trigger.cpp
	$(CXX) $(CXXFLAGS) -o sample_trigger main.cpp sample_trigger.cpp

clean:
	-rm -f sample_trigger