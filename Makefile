# Compiler and flags
CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -O2

# Targets
TARGET   := main
SRC      := main.cpp

# Default rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Run the program
run: $(TARGET)
	./$(TARGET)

# Optional cleanup
clean:
	rm -f $(TARGET)

.PHONY: all clean run

