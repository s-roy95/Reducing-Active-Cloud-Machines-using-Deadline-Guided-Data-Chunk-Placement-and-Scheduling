#include <iostream>
#include <cstdlib>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#define OS_NAME "Windows"
#define COMPILE_COMMAND "g++ -o test_cases test_cases.cpp"
#define TEST_CASES_COMMAND "test_cases.exe"
#define HPC_COMPILE_COMMAND "g++ -o hpc hpc.cpp"
#define HPC_COMMAND "hpc.exe"
#else
#include <unistd.h>
#define OS_NAME "Unix-like"
#define COMPILE_COMMAND "g++ -o test_cases test_cases.cpp"
#define TEST_CASES_COMMAND "./test_cases"
#define HPC_COMPILE_COMMAND "g++ -o hpc hpc.cpp"
#define HPC_COMMAND "./hpc"
#endif

using namespace std;

void compile_and_run() {
    cout << "Operating System: " << OS_NAME << endl;

    // Compile test_cases.cpp
    cout << "Compiling test_cases.cpp..." << endl;
    if (system(COMPILE_COMMAND) != 0) {
        cerr << "Error compiling test_cases.cpp" << endl;
        exit(EXIT_FAILURE);
    }

    // Generate test cases and store them in input.txt
    cout << "Generating test cases..." << endl;
    if (system(TEST_CASES_COMMAND) != 0) {
        cerr << "Error generating test cases" << endl;
        exit(EXIT_FAILURE);
    }

    //Compile hpc.cpp
    cout << "Compiling hpc.cpp..." << endl;
    if (system(HPC_COMPILE_COMMAND) != 0) {
       cerr << "Error compiling hpc.cpp" << endl;
       exit(EXIT_FAILURE);
    }

    // Read input from input.txt and print output
    cout << "Reading input and printing output..." << endl;
    if (system(HPC_COMMAND) != 0) {
        cerr << "Error running hpc" << endl;
        exit(EXIT_FAILURE);
    }

    // Clean up
    cout << "Cleaning up..." << endl;
    remove("test_cases");
    remove("hpc");
}

int main() {
    int num_iterations = 2; // Change this value to execute the code multiple times

    for (int i = 0; i < num_iterations; ++i) {
        cout << "Iteration " << i + 1 << "/" << num_iterations << endl;
        compile_and_run();
    }

    return 0;
}
