#include "RoutePlanner.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <set>

// ──────────────────────────────────────────────────────────────────────────────
RoutePlanner::RoutePlanner(std::vector<Order> orders, int workingHours)
    : orders_(std::move(orders)), workingHours_(workingHours) {}

// ──────────────────────────────────────────────────────────────────────────────
void RoutePlanner::sortByDeadline() {
    std::sort(orders_.begin(), orders_.end(),
              [](const Order& a, const Order& b) {
                  return a.deadline < b.deadline;
              });
}

// ──────────────────────────────────────────────────────────────────────────────
void RoutePlanner::optimise() {
    sortByDeadline();

    const int n = static_cast<int>(orders_.size());
    const int W = workingHours_;

    // ── Build DP table ────────────────────────────────────────────────────────
    // dp[i][t] = best profit using some subset of the first i orders in t time
    // Initialise everything to 0 (base case already satisfied).
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(W + 1, 0));

    for (int i = 1; i <= n; ++i) {
        const Order& ord = orders_[i - 1];   // 0-based in vector
        const int d = ord.timeRequired;
        const int p = ord.profit;
        const int L = ord.deadline;           // latest finish time

        for (int t = 0; t <= W; ++t) {
            // Option 1: SKIP order i
            dp[i][t] = dp[i - 1][t];

            // Option 2: TAKE order i
            //   Requires: t >= d          (enough time elapsed to fit this order)
            //             t <= L          (finishes within deadline)
            //             t <= W          (within working hours — always true here)
            if (t >= d && t <= L) {
                int candidate = dp[i - 1][t - d] + p;
                if (candidate > dp[i][t]) {
                    dp[i][t] = candidate;
                }
            }
        }
    }

    // ── Find best total profit and the time column it occurs at ──────────────
    totalProfit_ = 0;
    int bestT = 0;
    for (int t = 0; t <= W; ++t) {
        if (dp[n][t] > totalProfit_) {
            totalProfit_ = dp[n][t];
            bestT = t;
        }
    }

    // ── Back-track to identify which orders were selected ────────────────────
    backtrack(dp, n, bestT);

    timeUsed_   = bestT;
    optimised_  = true;
}

// ──────────────────────────────────────────────────────────────────────────────
void RoutePlanner::backtrack(const std::vector<std::vector<int>>& dp,
                              int n, int bestT) {
    std::set<int> selectedIds;
    int t = bestT;

    for (int i = n; i >= 1; --i) {
        // If the profit changed compared to skipping this order, it was taken.
        if (dp[i][t] != dp[i - 1][t]) {
            selectedIds.insert(orders_[i - 1].id);
            t -= orders_[i - 1].timeRequired;
        }
    }

    selectedOrders_.clear();
    rejectedOrders_.clear();

    for (const auto& ord : orders_) {
        if (selectedIds.count(ord.id))
            selectedOrders_.push_back(ord);
        else
            rejectedOrders_.push_back(ord);
    }
}

// ──────────────────────────────────────────────────────────────────────────────
void RoutePlanner::printReport() const {
    if (!optimised_) {
        std::cout << "[RoutePlanner] Call optimise() before printReport().\n";
        return;
    }

    const int barWidth = 50;
    const double utilPct = workingHours_ > 0
        ? 100.0 * timeUsed_ / workingHours_ : 0.0;

    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║        DELIVERY ROUTE PROFIT OPTIMISER — REPORT         ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    // ── Selected orders ───────────────────────────────────────────────────────
    std::cout << "✅  SELECTED ORDERS  (" << selectedOrders_.size() << " orders)\n";
    std::cout << "────────────────────────────────────────────────────────────\n";
    if (selectedOrders_.empty()) {
        std::cout << "  (none)\n";
    } else {
        for (const auto& ord : selectedOrders_) ord.print();
    }

    // ── Summary ───────────────────────────────────────────────────────────────
    std::cout << "\n────────────────────────────────────────────────────────────\n";
    std::cout << "  Total Profit   : $" << totalProfit_ << "\n";
    std::cout << "  Time Used      : " << timeUsed_
              << " / " << workingHours_ << " units"
              << "  (" << std::fixed << std::setprecision(1) << utilPct << "%)\n";

    // ASCII progress bar
    int filled = static_cast<int>(utilPct / 100.0 * barWidth);
    std::cout << "  Utilisation    : [";
    for (int i = 0; i < barWidth; ++i)
        std::cout << (i < filled ? "\xe2\x96\x88" : "\xe2\x96\x91");
    std::cout << "]\n";

    // ── Rejected orders ───────────────────────────────────────────────────────
    std::cout << "\n❌  REJECTED / SKIPPED ORDERS  (" << rejectedOrders_.size() << " orders)\n";
    std::cout << "────────────────────────────────────────────────────────────\n";
    if (rejectedOrders_.empty()) {
        std::cout << "  (none — all orders selected!)\n";
    } else {
        for (const auto& ord : rejectedOrders_) ord.print();
    }

    std::cout << "\n════════════════════════════════════════════════════════════\n\n";
}
