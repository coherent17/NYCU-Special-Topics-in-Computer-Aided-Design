# ompiler & Linker settings
CC = g++
CFLAGS = -g -Wall -std=c++11 -I./include

# Valgrind for memory issue
CHECKCC = valgrind
CHECKFLAGS = -s --show-leak-kinds=all --track-origins=yes 


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
	$(CHECKCC) $(CHECKFLAGS) ./$(TARGET) Test_Case/High_Fan_Out.v -l Test_Case/test_lib.lib

example:
	./$(TARGET) Test_Case/example.v -l Test_Case/test_lib.lib
	diff -w --ignore-blank-lines 0811562_example_load.txt Test_Case/golden/golden_example_load.txt
	diff -w --ignore-blank-lines 0811562_example_delay.txt Test_Case/golden/golden_example_delay.txt
	diff -w --ignore-blank-lines 0811562_example_path.txt Test_Case/golden/golden_example_path.txt

c17:
	./$(TARGET) Test_Case/c17.v -l Test_Case/test_lib.lib
	diff -w --ignore-blank-lines 0811562_c17_load.txt Test_Case/golden/golden_c17_load.txt
	diff -w --ignore-blank-lines 0811562_c17_delay.txt Test_Case/golden/golden_c17_delay.txt
	diff -w --ignore-blank-lines 0811562_c17_path.txt Test_Case/golden/golden_c17_path_1.txt
c432:
	./$(TARGET) Test_Case/c432.v -l Test_Case/test_lib.lib
	diff -w --ignore-blank-lines 0811562_c432_load.txt Test_Case/golden/golden_c432_load.txt
	diff -w --ignore-blank-lines 0811562_c432_delay.txt Test_Case/golden/golden_c432_delay.txt
	diff -w --ignore-blank-lines 0811562_c432_path.txt Test_Case/golden/golden_c432_path.txt

c17_comment:
	./$(TARGET) Test_Case/c17_comment.v -l Test_Case/test_lib.lib
	diff -w --ignore-blank-lines 0811562_c17_comment_load.txt Test_Case/golden/golden_c17_load.txt
	diff -w --ignore-blank-lines 0811562_c17_comment_delay.txt Test_Case/golden/golden_c17_delay.txt
	diff -w --ignore-blank-lines 0811562_c17_comment_path.txt Test_Case/golden/golden_c17_path_1.txt

hellish:
	./$(TARGET) Test_Case/hellish.v -l Test_Case/test_lib.lib
	diff -w --ignore-blank-lines 0811562_hellish_load.txt Test_Case/golden/golden_c17_load.txt
	diff -w --ignore-blank-lines 0811562_hellish_delay.txt Test_Case/golden/golden_c17_delay.txt
	diff -w --ignore-blank-lines 0811562_hellish_path.txt Test_Case/golden/golden_c17_path_1.txt

NAND:
	./$(TARGET) Test_Case/NAND2.v -l Test_Case/test_lib.lib

NOR:
	./$(TARGET) Test_Case/NOR2.v -l Test_Case/test_lib.lib

INV:
	./$(TARGET) Test_Case/INV.v -l Test_Case/test_lib.lib

AOI22:
	./$(TARGET) Test_Case/AOI22.v -l Test_Case/test_lib.lib

c17_modified:
	./$(TARGET) Test_Case/c17_modified.v -l Test_Case/test_lib.lib

High_Fan_Out:
	./$(TARGET) Test_Case/High_Fan_Out.v -l Test_Case/test_lib.lib

clean:
	rm -rf $(OBJDIR) $(TARGET) *delay* *load* *path*