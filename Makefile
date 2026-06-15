# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Linker flags (Строго для сборки готового приложения, пойдут в самый конец)
LDFLAGS = -lSDL2 -lSDL2_image

# Target executable
TARGET = app

# Source files
SRCS = render.cpp input.cpp pieces.cpp board.cpp game.cpp main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build and run the executable
all: $(TARGET) run

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX)	$(CXXFLAGS)	$(OBJS)	-o	$(TARGET)	$(LDFLAGS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the executable
run: $(TARGET)
	./$(TARGET)

# Clean rule to remove generated files
clean:
	rm	-f	$(TARGET) $(OBJS)