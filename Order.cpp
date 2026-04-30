#include "Order.h"
#include <iostream>
#include <iomanip>

Order::Order(int id, std::string label, int timeRequired, int profit, int deadline)
    : id(id), label(std::move(label)),
      timeRequired(timeRequired), profit(profit), deadline(deadline) {}

void Order::print() const {
    std::cout << std::left
              << "  [" << std::setw(2) << id << "] "
              << std::setw(12) << label
              << "  Time: " << std::setw(3) << timeRequired
              << "  Profit: $" << std::setw(6) << profit
              << "  Deadline: t=" << deadline
              << "\n";
}
