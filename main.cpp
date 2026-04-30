#include "Order.h"
#include "RoutePlanner.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>

// ──────────────────────────────────────────────────────────────────────────────
// Helper: flush cin error state after a bad read
// ──────────────────────────────────────────────────────────────────────────────
static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ──────────────────────────────────────────────────────────────────────────────
// Helper: read a positive integer with a prompt
// ──────────────────────────────────────────────────────────────────────────────
static int readPositiveInt(const std::string& prompt) {
    int val = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val && val > 0) {
            clearInput();
            return val;
        }
        std::cout << "  ⚠  Please enter a positive integer.\n";
        clearInput();
    }
}

// ──────────────────────────────────────────────────────────────────────────────
// Demo dataset — used when the user chooses the preset option
// ──────────────────────────────────────────────────────────────────────────────
static std::vector<Order> buildDemoOrders() {
    // id, label, timeRequired, profit, deadline
    return {
        {1,  "Order A",  2,  50,  5},
        {2,  "Order B",  3,  80,  6},
        {3,  "Order C",  1,  30,  3},
        {4,  "Order D",  4, 120,  8},
        {5,  "Order E",  2,  60,  7},
        {6,  "Order F",  3,  90,  9},
        {7,  "Order G",  1,  20,  2},
        {8,  "Order H",  5, 150, 10},
        {9,  "Order I",  2,  45,  6},
        {10, "Order J",  3, 100, 11},
    };
}

// ──────────────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║        DELIVERY ROUTE PROFIT OPTIMISER  v1.0            ║\n";
    std::cout << "║   Maximise profit under deadline & time constraints      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    // ── Choose input mode ─────────────────────────────────────────────────────
    std::cout << "Select input mode:\n"
              << "  1. Use built-in demo orders (10 orders, 8 working hours)\n"
              << "  2. Enter orders manually\n"
              << "Choice: ";

    int mode = 0;
    std::cin >> mode;
    clearInput();

    std::vector<Order> orders;
    int workingHours = 0;

    if (mode == 1) {
        // ── Demo mode ─────────────────────────────────────────────────────────
        orders       = buildDemoOrders();
        workingHours = 8;

        std::cout << "\n[Demo] Loaded " << orders.size()
                  << " orders.  Working hours = " << workingHours << "\n";
        std::cout << "\nAll candidate orders (unsorted):\n";
        std::cout << "────────────────────────────────────────────────────────────\n";
        for (const auto& o : orders) o.print();

    } else {
        // ── Manual entry mode ─────────────────────────────────────────────────
        workingHours = readPositiveInt("\nEnter total working hours available: ");
        int n        = readPositiveInt("How many delivery orders to enter?   ");

        std::cout << "\nFor each order enter: timeRequired  profit  deadline\n"
                  << "(deadline = latest finish time in the same time units)\n\n";

        for (int i = 1; i <= n; ++i) {
            std::cout << "--- Order " << i << " ---\n";
            int t = readPositiveInt("  Time required : ");
            int p = readPositiveInt("  Profit ($)    : ");
            int d = readPositiveInt("  Deadline (t≤) : ");
            orders.emplace_back(i, "Order " + std::to_string(i), t, p, d);
        }
    }

    // ── Run optimiser ─────────────────────────────────────────────────────────
    std::cout << "\n[Optimiser] Running DP…\n";
    RoutePlanner planner(orders, workingHours);
    planner.optimise();
    planner.printReport();

    return 0;
}
