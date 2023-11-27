# ompiler & Linker settings
CC = g++
CFLAGS = -g -Wall -O3 -std=c++11 -I./include
#CFLAGS = -g -Wall -DNDEBUG -O3 -std=c++11 -I./include
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
	$(CHECKCC) $(CHECKFLAGS) ./$(TARGET) Test_Case/c17.v Test_Case/c17.pat Test_Case/test_lib.lib


c17:
	./$(TARGET) Test_Case/c17.v Test_Case/c17.pat Test_Case/test_lib.lib
	diff -w 0811562_c17_load.txt Test_Case/golden/golden_c17_load.txt
	diff -w 0811562_c17_gate_info.txt Test_Case/golden/golden_c17_gate_info.txt
	diff -w 0811562_c17_gate_power.txt Test_Case/golden/golden_c17_gate_power.txt
	diff -w 0811562_c17_coverage.txt Test_Case/golden/golden_c17_coverage.txt

c432:
	./$(TARGET) Test_Case/c432.v Test_Case/c432.pat Test_Case/test_lib.lib
	diff -w 0811562_c432_load.txt Test_Case/golden/golden_c432_load.txt
	diff -w 0811562_c432_gate_info.txt Test_Case/golden/golden_c432_gate_info.txt
	diff -w 0811562_c432_gate_power.txt Test_Case/golden/golden_c432_gate_power.txt
	diff -w 0811562_c432_coverage.txt Test_Case/golden/golden_c432_coverage.txt

example:
	./$(TARGET) Test_Case/example.v Test_Case/example.pat Test_Case/test_lib.lib
	diff -w 0811562_example_load.txt Test_Case/golden/golden_example_load.txt
	diff -w 0811562_example_gate_info.txt Test_Case/golden/golden_example_gate_info.txt
	diff -w 0811562_example_gate_power.txt Test_Case/golden/golden_example_gate_power.txt
	diff -w 0811562_example_coverage.txt Test_Case/golden/golden_example_coverage.txt

c54321:
	time ./$(TARGET) Test_Case/c54321.v Test_Case/c54321.pat Test_Case/test_lib.lib

clean:
	rm -rf $(OBJDIR) $(TARGET) *load.txt *gate_info.txt *power.txt *coverage.txt