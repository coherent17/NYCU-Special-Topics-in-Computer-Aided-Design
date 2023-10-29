#ifndef _UTIL_H_
#define _UTIL_H_

#include <bits/stdc++.h>
#include "Util.h"
using namespace std;

// The regular expression used to extract the pattern while parsing Library
const regex Comment_Space_Pattern("(\\/\\/.*)|(\\/\\*.*\\*\\/)|(\\s+)");
const regex Index_Head_Pattern("index\\w*");
const regex Float_Pattern(R"(-?\d+(\.\d+)?)");
const regex Cell_Pattern("^cell\\(");
const regex Pin_Pattern("^pin\\(");
const regex Cap_Pattern("^capacitance");
const regex Table_Name_Pattern("(^rise_power)|(^fall_power)|(^cell_rise)|(^cell_fall)|(^rise_transition)|(^fall_transition)");
const regex Table_Pattern("(^values)|(^\")");

// The regular expression used to extract the pattern while parsing Verilog code
const regex Comment_Front_Space_Pattern("(\\/\\/.*)|(\\/\\*.*\\*\\/)|(^\\s+)");
const regex Multi_Space_Pattern("(\\s+)");
const regex Net_Pattern("(input)|(output)|(wire)");
const regex Net_Name_Pattern("(\\w+)");
const regex Cell_Type_Pattern("(NOR2X1)|(NANDX1)|(INVX1)");
const regex Pin_Name_Pattern("(\\w+)");

// Cell Type
enum Cell_Type{
    NOR2X1,
    INVX1,
    NANDX1
};

// Net Type
enum Net_Type{
    input,
    output,
    wire
};

#endif