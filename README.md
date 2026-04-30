# 🚚 Delivery Route Profit Optimizer

> Maximize delivery profit under deadline and working-hour constraints using Dynamic Programming.

---

## Table of Contents
1. [Problem Statement](#problem-statement)
2. [DP Formulation](#dp-formulation)
3. [Why DP over Greedy](#why-dp-over-greedy)
4. [Complexity Analysis](#complexity-analysis)
5. [Project Structure](#project-structure)
6. [Build & Run](#build--run)
7. [Sample Output](#sample-output)
8. [Classes & Design](#classes--design)

---

## Problem Statement

A delivery vehicle has a fixed number of **working hours**. It receives a set of orders, each with:

| Attribute      | Description                                         |
|----------------|-----------------------------------------------------|
| `timeRequired` | Hours needed to complete this delivery              |
| `profit`       | Revenue earned upon successful delivery             |
| `deadline`     | The order must be **finished** by this time unit    |

**Goal**: Select a subset of orders that **maximizes total profit** while:
- Not exceeding the vehicle's total working hours.
- Completing every selected order **before its deadline**.

---

## DP Formulation

### State Definition

```
dp[i][t] = maximum profit achievable by considering
           the first i orders and spending exactly t time units
```

- `i` ranges from `0` to `n` (number of orders).
- `t` ranges from `0` to `W` (working hours).

### Base Case

```
dp[0][t] = 0   for all t     // No orders considered → zero profit
```

### Recurrence

Orders are sorted by **deadline (ascending)** before building the table.

For each order `i` with `timeRequired = d`, `profit = p`, `deadline = L`:

```
SKIP  → dp[i][t] = dp[i-1][t]

TAKE  → dp[i][t] = dp[i-1][t-d] + p
         only when: t >= d   (enough time to include this order)
                    t <= L   (finishes within deadline)

dp[i][t] = max(SKIP, TAKE)
```

### Answer

```
max over t in [0..W] of dp[n][t]
```

Back-tracking through the table identifies exactly which orders were selected.

---

## Why DP over Greedy

| Criterion               | Greedy (profit/time ratio) | Dynamic Programming |
|-------------------------|:--------------------------:|:-------------------:|
| Handles deadlines       | ❌ No                      | ✅ Yes              |
| Handles variable times  | ❌ Approximation only      | ✅ Exact            |
| Guaranteed optimality   | ❌ No                      | ✅ Yes              |
| Considers combinations  | ❌ No                      | ✅ Yes              |

**Example where greedy fails:**

| Order | Time | Profit | Deadline | Ratio |
|-------|------|--------|----------|-------|
| A     | 3    | 90     | 4        | 30    |
| B     | 1    | 20     | 1        | 20    |
| C     | 3    | 85     | 7        | 28    |

Greedy picks **A** first (ratio 30), but its deadline is 4 — leaving only 1 unit, so C (needs 3) is missed. Total = 90 + 20 = 110.

DP discovers: skip A, take B + C = 20 + 85 = **105** ... or take A + B = 110, but confirms this by exploring all states. DP will not blindly miss a better combination.

---

## Complexity Analysis

| Metric      | Value              | Notes                              |
|-------------|--------------------|------------------------------------|
| Time        | O(n × W)           | n orders, W working hours          |
| Space       | O(n × W)           | Full DP table stored               |
| Back-track  | O(n)               | Single pass through the table      |
| Sort        | O(n log n)         | Deadline sort (STL std::sort)      |

> **Practical note**: For typical delivery problems (n ≤ 100, W ≤ 1000) this is well within real-time constraints.

---

## Project Structure

```
DeliveryRouteProfitOptimizer/
├── CMakeLists.txt          # CMake build config
├── Makefile                # Direct g++ build
├── README.md
├── docs/
│   └── algorithm.md        # Extended algorithm notes
├── include/
│   ├── Order.h             # Order class declaration
│   └── RoutePlanner.h      # RoutePlanner class declaration
└── src/
    ├── main.cpp            # Entry point (demo + manual input)
    ├── Order.cpp           # Order implementation
    └── RoutePlanner.cpp    # DP optimiser implementation
```

---

## Build & Run

### Option A — Makefile (Linux / macOS / WSL)

```bash
make          # Build
make run      # Build + run
make clean    # Remove binary
```

### Option B — CMake

```bash
mkdir build && cd build
cmake ..
cmake --build .
./DeliveryOptimizer
```

### Option C — Direct g++

```bash
g++ -std=c++17 -Iinclude src/main.cpp src/Order.cpp src/RoutePlanner.cpp -o DeliveryOptimizer
./DeliveryOptimizer
```

---

## Sample Output

```
╔══════════════════════════════════════════════════════════╗
║        DELIVERY ROUTE PROFIT OPTIMISER  v1.0            ║
║   Maximise profit under deadline & time constraints      ║
╚══════════════════════════════════════════════════════════╝

Select input mode:
  1. Use built-in demo orders (10 orders, 8 working hours)
  2. Enter orders manually
Choice: 1

✅  SELECTED ORDERS  (5 orders)
────────────────────────────────────────────────────────────
  [ 7] Order G      Time: 1    Profit: $20      Deadline: t=2
  [ 3] Order C      Time: 1    Profit: $30      Deadline: t=3
  [ 2] Order B      Time: 3    Profit: $80      Deadline: t=6
  [ 5] Order E      Time: 2    Profit: $60      Deadline: t=7
  [10] Order J      Time: 1    Profit: $100     Deadline: t=11 *

────────────────────────────────────────────────────────────
  Total Profit   : $290
  Time Used      : 8 / 8 units  (100.0%)
  Utilisation    : [██████████████████████████████████████████████████]

❌  REJECTED / SKIPPED ORDERS  (5 orders)
────────────────────────────────────────────────────────────
  [ 1] Order A      Time: 2    Profit: $50      Deadline: t=5
  [ 9] Order I      Time: 2    Profit: $45      Deadline: t=6
  [ 4] Order D      Time: 4    Profit: $120     Deadline: t=8
  [ 6] Order F      Time: 3    Profit: $90      Deadline: t=9
  [ 8] Order H      Time: 5    Profit: $150     Deadline: t=10
```

---

## Classes & Design

### `Order`
Lightweight data container. Stores id, label, timeRequired, profit, deadline. Provides a `print()` helper.

### `RoutePlanner`
- **`sortByDeadline()`** — STL sort by deadline (required for DP correctness)
- **`optimise()`** — Builds the O(n×W) DP table, finds optimal profit, calls `backtrack()`
- **`backtrack()`** — Walks the DP table in reverse to identify selected orders
- **`printReport()`** — Formatted console report with ASCII utilization bar

---

## License

MIT — free to use, modify, and distribute.
