# ompiler & Linker settings
CC = g++
CFLAGS = -g -Wall -std=c++11 -I./include

# Valgrind for memory issue
CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all --track-origins=yes 


# Source files and object files
SRCDIR = src
OBJDIR = obj
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Name of the executable
TARGET = 0811562

all: $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) main.cpp $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

check:
	$(CHECKCC) $(CHECKFLAGS)

example:
	./$(TARGET) Test_Case/example.v -l Test_Case/test_lib.lib

c17:
	./$(TARGET) Test_Case/c17.v -l Test_Case/test_lib.lib

c432:
	./$(TARGET) Test_Case/c432.v -l Test_Case/test_lib.lib

clean:
	rm -rf $(OBJDIR) $(TARGET)