
Every human has a **unique genome**, yet the majority of our DNA is **highly similar** across individuals.  
Analyzing and processing these massive DNA sequences requires **high-performance computing techniques**, and that’s where **parallel programming** becomes essential.

---

##  Project Aim
This project demonstrates the power of **parallel programming** by analyzing DNA sequences using four computational approaches.  
The goal is to **measure performance and efficiency** while maintaining accurate biological results.

###  The Project Performs:
1. **Base Frequency Counting**  
   Counts how many times each character (`A`, `C`, `G`, `T`) appears in the DNA sequence.
   
3. **Subsequence Detection**  
   Finds how many times the specific pattern GCCAGATATTCCCCCCGTT appears in the given DNA test cases.
   
5. **Performance Evaluation**  
Measures and compares the **execution time** of four code designs:
- 🧩 **Sequential Design** — Basic single-threaded implementation  
- ⚙️ **OpenMP** — Shared-memory parallelism using threads  
- 🌐 **MPI** — Distributed-memory parallelism across processes or nodes  
- 🔗 **Hybrid (MPI + OpenMP)** — Combines both for optimal speed and scalability

---

## ⚙️ Program Designs

### 1️⃣ Sequential Design
- Standard single-threaded C/C++ program.
- Reads the DNA sequence file line by line.
- Counts character and subsequence occurrences using loops.
- **Used as a baseline** for performance comparison.

### 2️⃣ OpenMP Design (Shared Memory)
- Utilizes multiple threads within the same machine.
- Each thread processes a segment of the DNA sequence in parallel.
- Reduces runtime through **loop-level parallelism** and **atomic operations** for shared counters.

### 3️⃣ MPI Design (Distributed Memory)
- Distributes the DNA sequence across multiple **nodes or processes**.
- Each process counts locally, then sends results to a master process.
- Ideal for **cluster or multi-node environments**.
- Uses `MPI_Reduce()` to aggregate results efficiently.

### 4️⃣ Hybrid Design (MPI + OpenMP)
- Combines **MPI** (for inter-node communication) with **OpenMP** (for intra-node threading).
- Each MPI process handles a subset of data and uses OpenMP threads for local computation.
- Maximizes resource utilization across both **distributed and shared memory systems**.
- Provides the **best performance** for large-scale DNA data processing.

---

## 🏆 Best Design — Hybrid (MPI + OpenMP)

The **Hybrid Approach** is the **most efficient and scalable** design among the four implementations.

### 🚀 Why Hybrid Wins:
- **Dual-Level Parallelism:**  
Uses **MPI** across nodes (distributed memory) and **OpenMP** within each node (shared memory).  
This fully utilizes modern multi-core, multi-node architectures.

- **Improved Load Balancing:**  
Workload is evenly distributed between processes and threads, minimizing idle cores and maximizing throughput.

- **Reduced Communication Overhead:**  
MPI handles large-scale data distribution, while OpenMP minimizes inter-thread communication costs within nodes.

- **High Scalability:**  
Can scale from a single multicore workstation to a large computing cluster with minimal code changes.

- **Best Execution Time:**  
In all test scenarios, the Hybrid design consistently achieved the **lowest processing time** while maintaining accuracy.


 
 **Result:** Fastest, most balanced, and most scalable architecture for large genome datasets.

---
 Example Output

| Design        | A Count | C Count | G Count | T Count | Subsequence Count | Time (s) |
|----------------|----------|----------|----------|----------|-------------------|-----------|
| Sequential     | 152341   | 147890   | 156203   | 153888   | 120               | 5.234     |
| OpenMP         | 152341   | 147890   | 156203   | 153888   | 120               | 1.245     |
| MPI            | 152341   | 147890   | 156203   | 153888   | 120               | 0.984     |
| **Hybrid**     | **152341** | **147890** | **156203** | **153888** | **120** | **0.621** |

*(Sample results — actual times depend on test file size and hardware.)*






