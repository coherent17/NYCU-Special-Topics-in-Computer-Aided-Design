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
TARGET = 0811562.o

all: $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) 0811562.cpp $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

gen_table:
	$(PERLCC) Generate_Binary_Tabular.pl --max_num_var=8 --output=include/Binary_Bits.h	

check:
	$(CHECKCC) $(CHECKFLAGS) ./$(TARGET) input/input3.txt output/output3.txt

# Sample testcase
s1:
	./$(TARGET) Sample_Testcase/input1.txt Sample_Testcase/my_output1.txt
	diff Sample_Testcase/my_output1.txt Sample_Testcase/output1.txt

s2:
	./$(TARGET) Sample_Testcase/input2.txt Sample_Testcase/my_output2.txt
	diff Sample_Testcase/my_output2.txt Sample_Testcase/output2.txt

# Jimmy testcase
j1:
	./$(TARGET) Jimmy_Testcase/input1.txt Jimmy_Testcase/my_output1.txt
	diff Jimmy_Testcase/my_output1.txt Jimmy_Testcase/output1.txt

j2:
	./$(TARGET) Jimmy_Testcase/input2.txt Jimmy_Testcase/my_output2.txt
	diff Jimmy_Testcase/my_output2.txt Jimmy_Testcase/output2.txt

j3:
	./$(TARGET) Jimmy_Testcase/input3.txt Jimmy_Testcase/my_output3.txt
	diff Jimmy_Testcase/my_output3.txt Jimmy_Testcase/output3.txt

j4:
	./$(TARGET) Jimmy_Testcase/input4.txt Jimmy_Testcase/my_output4.txt
	diff Jimmy_Testcase/my_output4.txt Jimmy_Testcase/output4.txt

j5:
	./$(TARGET) Jimmy_Testcase/input5.txt Jimmy_Testcase/my_output5.txt
	diff Jimmy_Testcase/my_output5.txt Jimmy_Testcase/output5.txt

j6:
	./$(TARGET) Jimmy_Testcase/input6.txt Jimmy_Testcase/my_output6.txt
	diff Jimmy_Testcase/my_output6.txt Jimmy_Testcase/output6.txt

# Zip testcase
z1:
	./$(TARGET) Zip_Testcase/input1.txt Zip_Testcase/my_output1.txt
	diff -w Zip_Testcase/my_output1.txt Zip_Testcase/output1.txt

z2:
	./$(TARGET) Zip_Testcase/input2.txt Zip_Testcase/my_output2.txt
	diff -w Zip_Testcase/my_output2.txt Zip_Testcase/output2.txt

z3:
	./$(TARGET) Zip_Testcase/input3.txt Zip_Testcase/my_output3.txt
	diff -w Zip_Testcase/my_output3.txt Zip_Testcase/output3.txt

z4:
	./$(TARGET) Zip_Testcase/input4.txt Zip_Testcase/my_output4.txt
	diff -w Zip_Testcase/my_output4.txt Zip_Testcase/output4.txt

z5:
	./$(TARGET) Zip_Testcase/input5.txt Zip_Testcase/my_output5.txt
	diff -w Zip_Testcase/my_output5.txt Zip_Testcase/output5.txt

z6:
	./$(TARGET) Zip_Testcase/input6.txt Zip_Testcase/my_output6.txt
	diff -w Zip_Testcase/my_output6.txt Zip_Testcase/output6.txt

# My testcase
m1:
	./$(TARGET) My_Testcase/input1.txt My_Testcase/my_output1.txt

m2:
	./$(TARGET) My_Testcase/input2.txt My_Testcase/my_output2.txt

m3:
	./$(TARGET) My_Testcase/input3.txt My_Testcase/my_output3.txt

m4:
	./$(TARGET) My_Testcase/input4.txt My_Testcase/my_output4.txt

m5:
	./$(TARGET) My_Testcase/input5.txt My_Testcase/my_output5.txt

m6:
	./$(TARGET) My_Testcase/input6.txt My_Testcase/my_output6.txt

m7:
	./$(TARGET) My_Testcase/input7.txt My_Testcase/my_output7.txt

clean:
	rm -rf $(OBJDIR) $(TARGET) Sample_Testcase/my* Jimmy_Testcase/my* Zip_Testcase/my* My_Testcase/my*