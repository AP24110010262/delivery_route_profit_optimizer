#pragma once
#include "Order.h"
#include <vector>

/**
 * @class RoutePlanner
 * @brief Plans the optimal delivery route using Dynamic Programming.
 *
 * ## DP Formulation
 * ------------------
 * State:
 *   dp[i][t] = maximum profit achievable considering the first i orders
 *              and using exactly t time units so far.
 *
 * Dimensions:
 *   i  in [0 .. n]            (order index, 0 = no orders considered)
 *   t  in [0 .. workingHours] (elapsed time)
 *
 * Base case:
 *   dp[0][t] = 0  for all t  (no orders considered → zero profit)
 *
 * Recurrence (orders sorted by deadline):
 *   For each order i (1-based) with timeRequired d_i, deadline L_i, profit p_i:
 *
 *   SKIP order i:
 *     dp[i][t] = dp[i-1][t]
 *
 *   TAKE order i  (only if t >= d_i AND t <= L_i):
 *     dp[i][t] = max(dp[i][t], dp[i-1][t - d_i] + p_i)
 *
 *   The deadline constraint (t <= L_i) ensures the order finishes in time.
 *   The time constraint (t >= d_i) ensures we have enough room to do it.
 *
 * Answer:
 *   max over all t in [0..workingHours] of dp[n][t]
 *
 * ## Why DP over Greedy?
 * -----------------------
 * A greedy approach (e.g. sort by profit/time ratio) cannot correctly handle
 * the combination of:
 *   1. Hard deadlines — an order cannot be started after a certain time.
 *   2. Variable durations — choosing a short low-profit order now might unlock
 *      a large-profit order that a greedy pick would have blocked by deadline.
 * DP explores *all feasible subsets* implicitly and guarantees optimality.
 *
 * ## Complexity
 * --------------
 *   Time : O(n × W)  where n = number of orders, W = working hours
 *   Space: O(n × W)  for the DP table (can be reduced to O(W) with 1-D rolling)
 */
class RoutePlanner {
public:
    /**
     * @brief Construct a RoutePlanner.
     * @param orders       All candidate delivery orders.
     * @param workingHours Total available working time (time units).
     */
    RoutePlanner(std::vector<Order> orders, int workingHours);

    /// Sort orders by deadline (ascending) — prerequisite for DP correctness.
    void sortByDeadline();

    /**
     * @brief Run the DP algorithm and populate internal result state.
     *
     * After calling this, use getSelectedOrders(), getTotalProfit(),
     * getTimeUsed(), and getRejectedOrders() to inspect results.
     */
    void optimise();

    /// Print the full optimisation report to stdout.
    void printReport() const;

    // --- Result accessors ---
    const std::vector<Order>& getSelectedOrders()  const { return selectedOrders_;  }
    const std::vector<Order>& getRejectedOrders()  const { return rejectedOrders_;  }
    int                        getTotalProfit()     const { return totalProfit_;     }
    int                        getTimeUsed()        const { return timeUsed_;        }

private:
    std::vector<Order> orders_;       ///< All orders (sorted after sortByDeadline)
    int                workingHours_; ///< Vehicle's maximum working time

    // Results populated by optimise()
    std::vector<Order> selectedOrders_;
    std::vector<Order> rejectedOrders_;
    int                totalProfit_  = 0;
    int                timeUsed_     = 0;
    bool               optimised_    = false;

    /**
     * @brief Back-track through the DP table to identify selected orders.
     * @param dp    The filled DP table (size [n+1][W+1]).
     * @param n     Number of orders.
     * @param bestT The time column where the optimal profit was found.
     */
    void backtrack(const std::vector<std::vector<int>>& dp, int n, int bestT);
};
