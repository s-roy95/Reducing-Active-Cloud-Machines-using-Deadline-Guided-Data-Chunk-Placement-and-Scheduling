# Adaptive Cloud Resizing for Deadline-Constrained, Data-Locality Aware Workloads
## 📁 Directory Structure

### 1. `Datasets/`
Contains all the datasets used in the experiments.

  - `GOOGLE_ORG.txt`: Real life google dataset
  - `google.txt`: Real life data is formatted according to our approach
  - `trace.txt`: Generated synthetic dataset.
  - `google_CRED.txt`: Real life data is formatted according to CRED approach
  - `input.txt`: Generated synthetic dataset formatted acoording to CRED approach
 
   - 
- **User Request/**
  - Each User job 'j' in 'trace.txt' contains 5 fields:
    - `id_j`: Identification of the job 'j'
    - `arrival time`: Considered all jobs arrive at the same time
    - `deadline D`: Uniform for all the jobs in the trace
    - `num_task`: Number of tasks in the job 'j'.
    - `tasklist_j`: List of tasks associated with job 'j'.

---

### 2. `Code/`

Running Our approach
- ` g++ main.cpp`
- ` a.exe 1`     // Generates job 
- ` a.exe 2`       // Executes our approach

In order to simulate real life data
- In line 188, change file name from "trace.txt" to "google.txt" 

In order to simulate synthetic data
- In line 188, change file name from "google.txt" to "trace.txt"

### 2. `SHED+/`

Running SHED+
- `g++ SHED+.cpp`
- `a.exe SHED+.cpp`


Line 78 
-  `task.POCD < (0.70-0.95)`  //Range

Line 166
- `ComputePOCD() < (0.70-0.95)`  //Range

- we need to adjust the Range in-order to reduce number of "speculative attempts".













