#!/bin/bash

# Automated Public Test Cases for LSPT HW1 - Regular Expressions and Pattern Matching in C
# Adam Gibbons RPI F19

#INSTRUCTIONS:
#   1. Place script in same dirctory as your main_homework_file.c as well as test files
#   2. execute with: " ./LSPT-hw1-test.sh <name-of-your-main.c> <dev or prod>
#       "dev" (development) option will run with DEBUG_MODE (which may defeat the 
#       purpose if DEBUG_MODE produces extraneous output)
#       "prod" (production) option will run without DEBUG_MODE (preferred for these tests)

#REQUIRES:
#   1. Run in directory with all input output files (dont forget to make regex files)
#   2. your-output.txt file is available. Program will overwrite this file. If you
#       wish to use a different file, change the temp_output var below.

# if logs are clogging up your directory and you don't need them use 'rm *.log' 
# to delete all of them at once

temp_output=your-output.txt
total_passed=0
total_tests=15

function usage {
    printf "ERROR: invalid arguments\n"
    printf "Usage: $0 <filename> [dev|prod]\n"
}

if [ -z "$1" ]; then
    usage
    exit 1
fi

if ! [[ "$2" =~ ^(dev|prod)$ ]]; then
    usage
    exit 1
fi

# name of flag for debug/developer mode
# (change this var if you arn't using DEBUG_MODE)
devel_flag="-D DEBUG_MODE"

if [ "$2" == "prod" ]; then
    devel_flag=""
fi

c_file=$(echo "$1" | cut -f 1 -d '.')

# Creating log file

test_log="$c_file-$2-test-`date +%Y%m%d%H%M%S`.log"

# Beginning Test

printf "Test of $1 run on `date +%m/%d/%Y-%H:%M` with option: $2\n" > $test_log 
#tail $test_log -n +1

# Compilation

printf "\nCompiling program with: gcc -Wall -Werror $devel_flag $1\n" >> $test_log

gcc -Wall -Werror $devel_flag $1 >> $test_log 2>&1

# Testing command-line args

printf "\nTesting 0 arguments: expect ERROR\n" >> $test_log

./a.out >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "Program should have thrown error for invalid arguments\n" >> $test_log
    cat $test_log
    exit 1
fi

printf "\nTesting 1 argument: expect ERROR\n" >> $test_log

./a.out one-arg >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "Program should have thrown error for invalid arguments\n" >> $test_log
    cat $test_log
    exit 1
fi

printf "\nTesting 3 arguments: expect ERROR\n" >> $test_log

./a.out one-arg second-arg third-arg >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "Program should have thrown error for invalid arguments\n" >> $test_log
    cat $test_log
    exit 1
fi

printf "\nInvalid Argument Tests Successful\n" >> $test_log

printf "\nTesting correct args but bad files: expect ERROR\n" >> $test_log

./a.out one-arg second-arg >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "Program should have thrown error for no such file\n" >> $test_log

    cat $test_log
    exit 1
fi

printf "\nInvalid Argument Tests Successful\n" >> $test_log

# TEST 1-1
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex01-input01 (expected output: hw1-regex01-output01.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex01.txt >> $test_log

./a.out regex01.txt hw1-input01.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex01-output01.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex01-output01.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * Test Passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 2-1
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex02-input01 (expected output: hw1-regex02-output01.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex02.txt >> $test_log

./a.out regex02.txt hw1-input01.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex02-output01.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex02-output01.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * Test Passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 2-2
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex02-input02 (expected output: hw1-regex02-output02.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex02.txt >> $test_log

./a.out regex02.txt hw1-input02.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex02-output02.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex02-output02.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 3-1
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex03-input01 (expected output: hw1-regex03-output01.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex03.txt >> $test_log

./a.out regex03.txt hw1-input01.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex03-output01.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex03-output01.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 4-1
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex04-input01 (expected output: hw1-regex04-output01.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex04.txt >> $test_log

./a.out regex04.txt hw1-input01.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex04-output01.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex04-output01.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 4-3
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex04-input03 (expected output: hw1-regex04-output03.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex04.txt >> $test_log

./a.out regex04.txt hw1-input03.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex04-output03.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex04-output03.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 5-3
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex05-input03 (expected output: hw1-regex05-output03.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex05.txt >> $test_log

./a.out regex05.txt hw1-input03.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex05-output03.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex05-output03.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 6-1
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex06-input01 (expected output: hw1-regex06-output01.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex06.txt >> $test_log

./a.out regex06.txt hw1-input01.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex06-output01.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex06-output01.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 6-3
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex06-input03 (expected output: hw1-regex06-output03.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex06.txt >> $test_log

./a.out regex06.txt hw1-input03.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex06-output03.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex06-output03.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 7-1
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex07-input01 (expected output: hw1-regex07-output01.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex07.txt >> $test_log

./a.out regex07.txt hw1-input01.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex07-output01.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex07-output01.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 7-3
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex07-input03 (expected output: hw1-regex07-output03.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex07.txt >> $test_log

./a.out regex07.txt hw1-input03.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex07-output03.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex07-output03.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 8-3
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex08-input03 (expected output: hw1-regex08-output03.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex08.txt >> $test_log

./a.out regex08.txt hw1-input03.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex08-output03.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex08-output03.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 9-4
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex09-input04 (expected output: hw1-regex09-output04.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex09.txt >> $test_log

./a.out regex09.txt hw1-input04.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex09-output04.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex09-output04.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 10-2
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex10-input02 (expected output: hw1-regex10-output02.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex10.txt >> $test_log

./a.out regex10.txt hw1-input02.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex10-output02.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex10-output02.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# TEST 10-4
printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\nRunning Test: regex10-input04 (expected output: hw1-regex10-output04.txt)\n" >> $test_log

printf "regex: " >> $test_log
cat regex10.txt >> $test_log

./a.out regex10.txt hw1-input04.txt > $temp_output 2>&1

if [ $? -ne 0 ]; then
    printf "ERROR: Program returned non-zero value\n" >> $test_log
    cat $test_log
    cat $temp_output
    exit 1
fi

printf "Expected Output:\n" >> $test_log

cat hw1-regex10-output04.txt >> $test_log 2>&1

printf "\nYour Output:\n" >> $test_log

cat $temp_output >> $test_log 2>&1

printf "\nRunning diff on Expected Output VS Your Output:\n\n" >> $test_log

diff $temp_output hw1-regex10-output04.txt -s >> $test_log 2>&1

if [ $? -eq 0 ]; then
    printf "\n           ***************\n           * test passed *\n           ***************\n" >> $test_log
    total_passed=$((total_passed+1))
fi

# Total Tests Passed

printf "\n--------------------------------------------------------------------------------\n" >> $test_log

printf "\n    Tests Passed: [ $total_passed / $total_tests ]\n\n" >> $test_log

# Print log to terminal

cat $test_log
