# Algorithm Deep-Dive: Delivery Route Profit Optimizer

## 1. Problem Classification

This is a variant of the **0/1 Knapsack Problem** with an additional **deadline constraint**.

- **0/1**: Each order is either taken or skipped (no partial deliveries).
- **Knapsack capacity** = working hours (W).
- **Item weight** = delivery time required.
- **Item value** = profit.
- **Extra constraint**: item i can only be "placed" at cumulative time t ≤ deadline_i.

The deadline constraint makes this strictly harder than standard knapsack and
rules out simple greedy solutions.

---

## 2. Why Sort by Deadline?

Sorting orders by deadline (ascending) before building the DP table ensures:

1. When we decide whether to include order i at time t, all orders with
   tighter deadlines have already been considered at smaller indices.
2. The feasibility check `t <= deadline_i` is monotone as we scan rows,
   which keeps the recurrence correct and the back-tracking unambiguous.

Without deadline-sorted ordering the DP could incorrectly mark a state
as feasible even though fulfilling that order in that sequence would
violate its deadline.

---

## 3. State Space Diagram

```
          t →  0   1   2   3   4   5   6   7   8
         ┌───┬───┬───┬───┬───┬───┬───┬───┬───┐
i=0 (∅)  │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │
         ├───┼───┼───┼───┼───┼───┼───┼───┼───┤
i=1      │ 0 │ ? │ ? │ ? │ ? │ ? │ ? │ ? │ ? │  ← consider order 1
         ├───┼───┼───┼───┼───┼───┼───┼───┼───┤
  ...    │   │   │   │   │   │   │   │   │   │
         ├───┼───┼───┼───┼───┼───┼───┼───┼───┤
i=n      │ 0 │ ? │ ? │ ? │ ? │ ? │ ? │ ? │ * │  ← * = answer cell
         └───┴───┴───┴───┴───┴───┴───┴───┴───┘
```

Each cell dp[i][t] is filled in O(1) using the recurrence.
The answer is `max(dp[n][0..W])`.

---

## 4. Decision Tree (Conceptual)

For each order i and each time budget t:

```
                   dp[i][t]
                  /         \
           SKIP              TAKE
             |                 |
       dp[i-1][t]      (only if t>=d_i AND t<=L_i)
                        dp[i-1][t - d_i] + p_i
```

We always choose the branch yielding higher profit.

---

## 5. Back-Tracking Logic

After filling the table, we recover the selected set:

```
t ← bestT   (column where maximum profit was found)

for i = n down to 1:
    if dp[i][t] != dp[i-1][t]:
        // Order i was TAKEN — profit differs from "skip" row
        mark order i as SELECTED
        t ← t - orders[i].timeRequired
    // else: order i was SKIPPED
```

This runs in O(n) and uniquely reconstructs the optimal subset.

---

## 6. Worked Example

Orders (already sorted by deadline):

| # | Time | Profit | Deadline |
|---|------|--------|----------|
| G | 1    | 20     | 2        |
| C | 1    | 30     | 3        |
| A | 2    | 50     | 5        |
| B | 3    | 80     | 6        |
| I | 2    | 45     | 6        |
| E | 2    | 60     | 7        |
| D | 4    | 120    | 8        |
| F | 3    | 90     | 9        |
| H | 5    | 150    | 10       |
| J | 3    | 100    | 11       |

Working hours W = 8.

DP explores all feasible combinations. The optimal solution found:

```
G (t=1) + C (t=1) + B (t=3) + E (t=2) + partial...
→ Best: G + C + B + E + J  but J needs t≤11, cumulative=8 ✓
  Total time = 1+1+3+2+1 = 8, Profit = 20+30+80+60+100 = 290
```

---

## 7. Edge Cases Handled

| Scenario                              | Behaviour                              |
|---------------------------------------|----------------------------------------|
| Order deadline < its time required    | Never feasible (t≥d but t≤L fails)    |
| All orders exceed working hours       | Empty selection, profit = 0            |
| Single order fits perfectly           | That order selected                    |
| Multiple orders with same deadline    | DP correctly handles ties              |
| All orders selected                   | Rejected list shown as "(none)"        |

---

## 8. Complexity Summary

```
n = number of orders
W = working hours

Sort          : O(n log n)
DP fill       : O(n × W)      ← dominant term
Back-track    : O(n)
Space         : O(n × W)      ← can be reduced to O(W) if only profit needed

Overall       : O(n × W)  time,  O(n × W)  space
```

For the demo (n=10, W=8): only 80 DP cells — trivially fast.
For large inputs (n=200, W=10000): 2,000,000 cells — still milliseconds.
