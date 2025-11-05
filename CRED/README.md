# Placement and Scheduling Data Intensive Jobs

## Overview

This repository contains C++ code for solving the Placement and Scheduling Data Intensive Jobs problem and codes for generating test cases and generating output files.
Our code uses a mixture of heuristic and random allocation algorithm as basis to perform binary search on the number of nodes resulting in better resource allocation. Moreover we are also displaying the detailed result, i.e, which chunk is processed in which node in which vm at what timestamp for every node.  

## Files

1. `final.cpp`: The main program to be executed.
2. `hpc.cpp` : Code to write output in output.txt.
3. `execute.cpp`: Code to run `hpc.cpp` multiple times using the random test cases and save the outputs in `output.txt`.
4. `input.txt` : `trace.txt` generated in "Our_Approcah" is formatted as `input.txt` in CRED .


### Generating Random Test Cases

1. Copy and Paste `input.txt` or `google_CRED.txt` from "Our_Approach" folder:
   ```
   Rename `google_CRED.txt` as `input.txt`
   ```
### Executing the Main Program

1. Compile `final.cpp` using:
   ```
   g++ final.cpp
   ```
2. Run the compiled executable to execute the main program:
   ```
   a.exe final.cpp
   ```
   

### Running Multiple Times on Random Test Cases

1. Compile `execute.cpp` using:
   ```
   g++ execute.cpp
   ```
2. Run the compiled executable to execute `final.cpp` multiple times on random test cases:
   ```
   ./execute
   ```
   This will print the test cases along with their outputs to `output.txt`.


## Note

- Make sure to modify the code in `final.cpp` as per your requirements before executing.
