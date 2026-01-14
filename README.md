# Linux Synchronization & Scalability Analyzer

## Overview
This project analyzes **why multithreaded programs often fail to scale** on multi-core CPUs despite high CPU utilization.  
It demonstrates how **shared-state synchronization** (atomic operations) can become a performance bottleneck due to **cache-line contention**, and how **removing sharing** restores scalability.

The project uses **C++**, **POSIX threads**, and **Linux perf** to measure and reason about real hardware behavior.

---

## Problem Statement
Modern CPUs provide many cores, but adding threads does not always reduce execution time.

**Key question:**  
> Why does wall time stop improving (or worsen) when more threads are added?

---

## Experiment Design

Two controlled workloads are implemented:

### 1. Atomic Counter (Contention Case)
- All threads increment a **single shared `std::atomic` counter**
- Correctness guaranteed without locks
- Forces cache-line ownership transfer between cores

**Purpose:**  
Measure synchronization and cache-coherence overhead.

---

### 2. Private Counters (Scalable Case)
- Each thread increments its **own private counter**
- Final result is aggregated after threads finish
- No shared-state contention during execution

**Purpose:**  
Demonstrate performance improvement by removing synchronization.

---

## Build Instructions
### Atomic Counter
```bash
g++ src/atomic_counter.cpp -O2 -pthread -o atomic_counter
```

### Private Counter
```bash
g++ src/private_counter.cpp -O2 -pthread -o private_counter
```
---

## Running the Experiments
### Atomic Counter
```bash
./atomic_counter 1
./atomic_counter 2
./atomic_counter 4
```

### Private Counter
```bash
./private_counter 1
./private_counter 2
./private_counter 4
```

---
## Performance Measurement
**Enable perf (one-time)**
```bash
sudo sysctl -w kernel.perf_event_paranoid=1
```

**Measure Atomic Counter**
```bash
perf stat ./atomic_counter 4
```

**Measure Private Counter**
```bash
perf stat ./private_counter 4
```
---

## Observations
### Atomic Counter
- High CPU utilization
- Wall time does not scale with thread count
- User time increases faster than wall time
- Bottleneck caused by cache-line contention

### Private Counter
- Wall time improves with thread count
- Minimal synchronization overhead
- Demonstrates restored scalability

---
## Root Cause Analysis
- Atomic operations serialize updates at the cache-coherence level
- Only one core can own a cache line at a time
- Increased threads cause cache-line bouncing
- High CPU usage does not imply useful progress

---

## Key Takeaways
- Correctness ≠ scalability
- Atomics avoid locks but can still bottleneck performance
- Shared-state synchronization limits multicore scaling
- Data ownership and locality are critical for performance

---
## Conclusion
This project shows how synchronization choices directly affect real-world scalability on modern CPUs.
By combining controlled workloads with hardware performance counters, it demonstrates a measurement-driven approach to diagnosing and fixing multithreaded performance issues.


---
