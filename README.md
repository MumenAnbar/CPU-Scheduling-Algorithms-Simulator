# CPU Scheduling Algorithms Simulator

This repository contains a C++ program that simulates various CPU scheduling algorithms for a set of processes. The simulation runs over a 200-time-unit period and outputs the Gantt chart, average waiting time, and average turnaround time for each algorithm.

## Features

The following CPU scheduling algorithms are implemented:
1. **First Come First Served (FCFS)**
2. **Shortest Job First (SJF)**
3. **Shortest Remaining Time First (SRTF)**
4. **Round Robin (RR)** - Time quantum: 5
5. **Preemptive Priority Scheduling with Aging** - Priority decreases by 1 for every 5 units in the ready queue.
6. **Non-Preemptive Priority Scheduling with Aging** - Similar priority decrement as preemptive scheduling.

## How It Works

- **Process Management:**  
  The simulation dynamically manages the arrival, execution, waiting, and re-entry of processes.
- **Gantt Chart Generation:**  
  Visualizes the execution timeline for each scheduling algorithm.
- **Performance Metrics:**  
  Calculates and displays:
  - Average Waiting Time
  - Average Turnaround Time

## Setup and Usage

### Prerequisites
- C++ Compiler (e.g., GCC or Clang)

### Compile and Run
1. Clone the repository:
   ```bash
   git clone https://github.com/MumenAnbar/cpu-scheduling-simulator.git
   cd cpu-scheduling-simulator
## Output
### For each scheduling algorithm, the program will output:

Gantt Chart: A timeline of process execution.
Average Waiting Time: Average time processes spend waiting in the ready queue.
Average Turnaround Time: Average time from process arrival to completion.
### Example Output (FCFS)
```plaintext
GANTT CHART:
0  P1  10  P2  18  P3  32  ...

Average waiting time = 5.67
Average turnaround time = 13.33
