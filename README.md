# PCheck - Parallel File Integrity Checker

PCheck is a high performance command-line tool designed to verify file integrity and compare directory contents.


## Getting Started

Follow the steps to set up the environment and build the project.

### Prerequisites

Ensure you have the compiler and build tools installed in your Linux machine

* **GCC**
* **Make**

### Installation

1.Clone the Repository
```bash
git clone https://github.com/yigitsvsr/PCheck.git
cd pcheck
```

2.Build the Project:
```bash
make
```

3.Verify the Installation:
```bash
./pcheck
# run the automated tests with:
make test
```

4.Clean Up (Not Necessary):
```bash
make clean
```


## Features

* **Multithreaded Architecture:** File processing using a Thread Pool.
* **Calculation:** Calculates the hash values of files.
* **Comparison:** Compare 2 directories to find `[NEW]`, `[MODIFIED]` and `[UNCHANGED]` files.
* **Pipeline Support:** Reads from `stdin`, allowing integration with tools like `find`.
* **Performance Report:** Detailed information about execution time, throughput and data processed.


## Usage

### 1. Basic Mode (Hash Calculation)

Scans a directory recursively and calculates hashes for all files. 
```bash
./pcheck <target_directory>
# Example:
./pcheck src
```

### 2. Comparison Mode 

Compares files in a source directory against a backup/reference directory.
```bash
./pcheck <source_dir> -c <reference_dir>
# Example:
./pcheck test/source -c test/backup
```

### 3. Pipeline Mode

Reads file paths directly from standard input.
```bash
find src -name "*.c" | ./pcheck
```

### 4. Custom Thread Number

Use the `-t` flag to specify the number of threads (Default Value: 4).
```bash
./pcheck src -t 8
```


## Testing 

The project includes an automated tests defined in the Makefile. This target creates a temporary test environment, runs tests for both Basic and Compare modes, checks the output and cleans it up.

To execute test cases:
```bash
make test
```
***Expected Output:*** You should see a performance report and a "Test Successfully Completed" message.


## Technical Details

* **Concurrency:** The Main thread pushing tasks to a thread-safe Queue. Worker threads pull tasks and process file hashes in parallel.
* **Synchronization:** Thread-safe architecture provided by using `pthread_mutex` for safe memory acces and `pthread_cond`to manage worker states.
* **Memory Safety:** Project is verified with Valgrind to ensure 0 memory leaks.
* **Hashing:** Implements the FNV-1a algo for fast non-cryptographic integrity cheks.


## AI Usage
We used an AI assistance as a supportive tool during the design and development. The usage was limited to consultation and debugging. We maintained the full control over the implementation logic.
After defining the core project idea, AI was used to generate a development roadmap and understand the architectural requirements of the Producer-Consumer architecture. We used to discuss and compare alternative data structures and types. But we decided and executed the final selection and implementations. It also used to optimize Makefile Syntax.
All source code logic, thread synchronization and integration were written and verified by us.


## Authors

* Yigit Savasir 6824019, Cagan Yetis 6824048, Devin Demirdöven 6824050
