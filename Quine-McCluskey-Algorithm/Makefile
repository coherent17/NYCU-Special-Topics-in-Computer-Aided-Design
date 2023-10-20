# ompiler & Linker settings
CC = g++
CFLAGS = -g -Wall -std=c++11 -DNDEBUG -O3 -I./include

# Valgrind for memory issue
CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all --track-origins=yes 

# Pre Gen the LUT for binary representation
PERLCC = perl

# Source files and object files
SRCDIR = src
OBJDIR = obj
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Name of the executable
TARGET = main

all: $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) main.cpp $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

gen_table:
	$(PERLCC) Generate_Binary_Tabular.pl --max_num_var=8 --output=include/Binary_Bits.h	

check:
	$(CHECKCC) $(CHECKFLAGS) ./$(TARGET) input/input3.txt output/output3.txt


do1:
	./$(TARGET) testcase2/input1.txt testcase2/ouput1.txt

do2:
	./$(TARGET) testcase2/input2.txt testcase2/ouput2.txt

do3:
	./$(TARGET) testcase2/input3.txt testcase2/ouput3.txt

do4:
	./$(TARGET) testcase2/input4.txt testcase2/ouput4.txt

do5:
	./$(TARGET) testcase2/input5.txt testcase2/ouput5.txt

do6:
	./$(TARGET) testcase2/input6.txt testcase2/ouput6.txt


clean:
	rm -rf $(OBJDIR) $(TARGET) output