#!/usr/bin/perl

use strict;
use warnings;
use Getopt::Long;

my $max_num_var = 8;       # Default value for max_num_var
my $output_file = "Binary_Bits.h";  # Default output file

# Parse command-line options
GetOptions(
    "max_num_var=i" => \$max_num_var,   # Integer option for max_num_var
    "output=s"      => \$output_file,  # String option for output file
);

# Check if max_num_var is within a valid range (0 to 8)
if ($max_num_var < 0 || $max_num_var > 8) {
    die "Error: max_num_var must be in the range of 0 to 8.\n";
}

# Open the output file for writing
open my $fh, '>', $output_file or die "Could not open $output_file: $!";

# Generate the C++ header file content
print $fh "#ifndef BINARY_ARRAY_H\n";
print $fh "#define BINARY_ARRAY_H\n\n";
print $fh "#include <string>\n";
print $fh "#include <vector>\n";
print $fh "using namespace std;\n\n";
print $fh "const vector<vector<string>> binary_strings = {\n";

# Generate binary strings for num_var from 0 to $max_num_var
for my $num_var (0..$max_num_var) {
    my @binary_strings;
    my $max_value = 2**$num_var - 1;

    # Generate binary strings
    for my $num (0..$max_value) {
        my $binary = sprintf("%0${num_var}b", $num);
        push @binary_strings, $binary;
    }

    # Print binary strings as a C++ vector
    print $fh "    {";
    print $fh join(", ", map("\"$_\"", @binary_strings));
    print $fh "},\n";
}

# Close the array definition and the header guard
print $fh "};\n\n";
print $fh "#endif // BINARY_ARRAY_H\n";

# Close the output file
close $fh;

print "Header file '$output_file' generated successfully.\n";