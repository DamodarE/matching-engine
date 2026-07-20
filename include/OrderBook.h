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

    // Read-only introspection for tests: best price/quantity on each side.
    // Callers must check the corresponding *LevelCount() first; behavior is
    // undefined if the side is empty.
    int bestBidPrice() const;
    int bestBidQuantity() const;
    int bestAskPrice() const;
    int bestAskQuantity() const;

private:
    std::map<int, PriceLevel, std::greater<int>> bids_;
    std::map<int, PriceLevel> asks_;
    std::unordered_map<int, OrderLocation> order_lookup_;
};
