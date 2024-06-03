# Compiler
CPP = g++

# Compiler flags
CPP_FLAGS = -g -Wall -Wextra -std=c++17

# Include directories
INCLUDES = -I../include

# Libraries
LIBS = -lGLEW -lGL -lglfw

# Source
SRCS = odev.cpp InitShader.cpp

#
OBJS = $(SRCS:.cpp=.o)

# Name of the program
TARGET = odev

# Default rule
all: $(TARGET)

# Rule to compile .cpp files to object files
%.o: %.cpp
	$(CPP) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CPP) $(CPP_FLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
