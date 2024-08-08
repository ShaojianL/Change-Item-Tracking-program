# Makefile for Change Item Tracking System

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# SQLite library
LIBS = -lsqlite3

# Source files
SRCS = testFileOps.cpp Database.cpp Project.cpp Release.cpp Requester.cpp ChangeItem.cpp ChangeRequest.cpp UserInterface.cpp PrintUtils.cpp

# Header files
HEADERS = Database.h Project.h Release.h Requester.h ChangeItem.h ChangeRequest.h UserInterface.h PrintUtils.h

# Executable
EXEC = test_system

# Test executable
TEST_EXEC = test_ui

# Default target
all: $(EXEC) $(TEST_EXEC)

# Linking
$(EXEC): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRCS) $(LIBS)

# Test drive source files
TEST_SRCS = main.cpp Database.cpp Project.cpp Release.cpp Requester.cpp ChangeItem.cpp ChangeRequest.cpp UserInterface.cpp PrintUtils.cpp

# Linking for test drive
$(TEST_EXEC): $(TEST_SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) $(TEST_SRCS) $(LIBS)

# Clean target
clean:
	rm -f *.db $(EXEC) $(TEST_EXEC)

# Phony targets
.PHONY: all clean
