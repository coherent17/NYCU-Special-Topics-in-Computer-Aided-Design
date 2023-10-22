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

s1:
	./$(TARGET) Sample_Testcase/input1.txt Sample_Testcase/my_output1.txt
	diff Sample_Testcase/my_output1.txt Sample_Testcase/output1.txt

s2:
	./$(TARGET) Sample_Testcase/input2.txt Sample_Testcase/my_output2.txt
	diff Sample_Testcase/my_output2.txt Sample_Testcase/output2.txt

clean:
	rm -rf $(OBJDIR) $(TARGET) Sample_Testcase/my*