#!/bin/bash

# Check if an argument is provided
if [ $# -eq 0 ]; then
    echo "Error: No input file provided."
    echo "Usage: $0 <filename.cpp>"
    exit 1
fi

# Store the input filename
input_file="$1"

# Check if the file exists and has a .cpp extension
if [ ! -f "$input_file" ] || [[ ! "$input_file" =~ \.cpp$ ]]; then
    echo "Error: Invalid input file. Please provide a .cpp file."
    exit 1
fi

# Compile the C++ file
g++ -std=c++14 -Wall -Wextra -pedantic -O3 "$input_file" -o a.out

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Run the compiled program
./a.out

# Check if execution was successful
if [ $? -ne 0 ]; then
    echo "Program execution failed."
    exit 1
fi
