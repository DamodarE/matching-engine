#pragma once

#include <functional>
#include <map>
#include <unordered_map>
#include <vector>

#include "Order.h"
#include "PriceLevel.h"
#include "Trade.h"

struct OrderLocation {
    Side side;
    int price;
    PriceLevel::OrderIt it;
};

class OrderBook {
public:
    void addOrder(const Order& order);
    bool cancelOrder(int order_id);
    bool modifyOrder(int order_id, int new_price, int new_quantity);
    std::vector<Trade> matchOrder(const Order& order);

    // Read-only introspection for tests: number of distinct resting price
    // levels on each side.
    std::size_t bidLevelCount() const;
    std::size_t askLevelCount() const;

private:
    std::map<int, PriceLevel, std::greater<int>> bids_;
    std::map<int, PriceLevel> asks_;
    std::unordered_map<int, OrderLocation> order_lookup_;
};
