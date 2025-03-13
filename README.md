# **Flow Shop Scheduling - Heuristic Approaches**

## **Overview**
This project implements various heuristic and algorithms for solving the **Permutation Flow Shop Scheduling Problem (PFSP)**.  
The available methods include:  
 **Branch & Bound** 
 **Palmer’s Heuristic** : palmer  
 **NEH (Nawaz-Enscore-Ham) & MNEH (Modified NEH)**  : neh
 **CDS (Campbell-Dudek-Smith) Heuristic**  : cds
 **Rajendran & Chaudhuri (RC) Heuristic**  : rc
 **Genetic Algorithm (GA)**  : ga
 **NEH with Local Search (NEHLS)**  :nehls

---

## **How to Run**
### **1. Clone the Repository**
```sh
git clone https://github.com/YOUR_USERNAME/YOUR_REPOSITORY.git
cd YOUR_REPOSITORY
```

### **2. Compile the Project (Already Built)**
The **`build/`** directory is included, so no need to compile. Just proceed to execution.  
If you need to build manually, run:
```sh
mkdir -p build && cd build
cmake ..
make
```

### **3. Run the Application**
```sh
./build/FlowShopBnb <input_file> <algorithm name>
```
Example:
```sh
./build/FlowShopBnb data/input.txt palmer
```


### **4. Choose a Heuristic**
The program will prompt you to choose one of the available algorithms to solve the instance.

---

## **Input Format**
The input file should contain the **number of jobs (N) and machines (M)**, followed by a **processing time matrix**.  
Each row corresponds to a **job**, and each column corresponds to a **machine**.

### **Example Input File (`data/input.txt`)**
```
5 3   # 5 Jobs, 3 Machines
12 15 14
8  9  6
5  8  9
11 13 7
6  4  10
```
#### **Explanation:**
- First line: `N M` (Number of jobs, Number of machines)
- The next **N lines** contain **M integers** (processing times for each job across machines).

---

## **Example Output**
```
Selected Algorithm: NEH
Best Makespan: 82
Job Sequence: 3 → 1 → 5 → 2 → 4
Execution Time: 0.0034s
```

---


