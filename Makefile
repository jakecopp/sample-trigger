# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g -Wextra -pedantic -ansi -O3 -std=c++17

# Target executable
sample_trigger_cli: sample_trigger/main.cpp sample_trigger/sample_trigger.cpp
	$(CXX) $(CXXFLAGS) -o sample_trigger_cli sample_trigger/main.cpp sample_trigger/sample_trigger.cpp

sample_trigger_gui: gui/MainFrame.cpp gui/App.cpp sample_trigger/sample_trigger.cpp
	$(CXX) `wx-config --cxxflags` $(CXXFLAGS) -o sample_trigger_gui gui/MainFrame.cpp gui/App.cpp sample_trigger/sample_trigger.cpp `wx-config --libs`

clean:
	-rm -f sample_trigger_cli sample_trigger_gui