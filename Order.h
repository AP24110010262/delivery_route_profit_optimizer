#pragma once
#include <string>

/**
 * @class Order
 * @brief Represents a single delivery order with its constraints and reward.
 *
 * Each Order encapsulates:
 *  - A unique identifier and label
 *  - The time units required to complete the delivery
 *  - The profit (reward) earned upon completion
 *  - The deadline (latest finish time in time units) by which it must be done
 */
class Order {
public:
    int    id;           ///< Unique order ID (1-based)
    std::string label;   ///< Human-readable name, e.g. "Order A"
    int    timeRequired; ///< Delivery time needed (in time units)
    int    profit;       ///< Profit earned if this order is completed
    int    deadline;     ///< Latest finish time (inclusive, in time units)

    /**
     * @brief Construct an Order.
     * @param id           Unique ID
     * @param label        Display label
     * @param timeRequired Time units needed to complete
     * @param profit       Profit earned on completion
     * @param deadline     Must finish by this time unit
     */
    Order(int id, std::string label, int timeRequired, int profit, int deadline);

    /// Print a formatted summary of this order to stdout.
    void print() const;
};
