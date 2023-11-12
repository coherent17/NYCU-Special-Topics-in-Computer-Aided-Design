#ifndef _UTIL_H_
#define _UTIL_H_

#include <bits/stdc++.h>
using namespace std;

#define LUT_ROW_SIZE 7
#define LUT_COL_SIZE 7
#define STUDENT_ID "0811562"
#define PRIMARY_OUTPUT_LOADING 0.03
#define WIRE_DELAY 0.005

enum Cell_Type{
    NOR2X1,
    INVX1,
    NANDX1
};

enum Net_Type{
    input,
    output,
    wire
};

// The regular expression used to extract the pattern while parsing Library
const regex Comment_Space_Pattern("(\\/\\/.*)|(\\/\\*.*\\*\\/)|(\\s+)");
const regex Index_1_Pattern("index_1\\w*");
const regex Index_2_Pattern("index_2\\w*");
const regex Float_Pattern(R"(-?\d+(\.\d+)?)");
const regex Cell_Pattern("^cell\\(");
const regex Pin_Pattern("^pin\\(");
const regex Cap_Pattern("^capacitance");
const regex Table_Name_Pattern("(^rise_power)|(^fall_power)|(^cell_rise)|(^cell_fall)|(^rise_transition)|(^fall_transition)");
const regex Table_Pattern("(^values)|(^\")");

// The regular expression used to extract the pattern while parsing Netlist
const regex Multi_Line_Regex("/\\*.*?\\*/");
const regex Single_Line_Regex("//[^\x01]*\x01");
const regex Multi_Space_Pattern("(\\s+)");
const regex Net_Type_Pattern("(input)|(output)|(wire)");
const regex Cell_Type_Pattern("(NOR2X1)|(NANDX1)|(INVX1)");
const regex Word_Pattern("\\w+");
const regex Number_Pattern("\\d+");

#endif