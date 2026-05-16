# Distance Vector Routing (DVR)

This project implements the Distance Vector Routing (DVR) algorithm with the following features:

- Weighted Routing
- Poisoned Reverse
- Link Failure Handling
- Routing Table Convergence using Minimum Cost Paths

---

# How to Run the Code

## 1. Go to the project directory

```bash
cd Distance-Vector-Routing/src
```

---

## 2. Compile the code

```bash
g++ -std=c++11 main.cpp routing_algo.cpp -o rip
```

If compilation succeeds, no errors will be shown.

This command creates the executable:

```txt
rip
```

---

## 3. Create or modify the input file

Edit `sampleinput.txt` with the required topology.

Example:

```txt
3
A B C

A 10.0.0.1 10.0.0.21 B 1
B 10.0.0.21 10.0.0.1 A 1

B 10.0.1.23 10.0.1.3 C 2
C 10.0.1.3 10.0.1.23 B 2

EOE
```

---

## Input Format

```txt
NodeName MyIP NeighborIP NeighborName LinkCost
```

Example:

```txt
A 10.0.0.1 10.0.0.21 B 1
```

Where:

- `A` → Current node
- `10.0.0.1` → Interface IP of A
- `10.0.0.21` → Connected neighbor IP
- `B` → Neighbor node
- `1` → Link cost

---

## 4. Run the program

```bash
./rip < sampleinput.txt
```

The routing tables after convergence will be displayed on the terminal.

---

# Features Implemented

## 1. Weighted Routing

Routing decisions are based on total path cost instead of hop count.

Routing update formula:

```txt
new_cost = received_cost + link_cost
```

---

## 2. Poisoned Reverse

Neighbor-specific routing advertisements are implemented.

If a route uses a neighbor as the next hop, the route is advertised back to that neighbor with infinite cost.

Infinity value used:

```txt
INF = 999
```

---

## 3. Link Failure Handling

Links can be removed or assigned infinite cost to simulate failures.

The routing algorithm reconverges after failure detection.

---

# Files

- `main.cpp` → Input parsing and node initialization
- `node.h` → Data structures and routing message handling
- `routing_algo.cpp` → DVR algorithm and routing updates
- `sampleinput.txt` → Sample topology input

---

# Compilation Example

```bash
g++ -std=c++11 main.cpp routing_algo.cpp -o rip
```

---

# Execution Example

```bash
./rip < sampleinput.txt
```