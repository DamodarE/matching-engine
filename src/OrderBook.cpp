#include "OrderBook.h"

void OrderBook::addOrder(const Order& order) {
    // Unconditionally rests the order in the book. Crossing the spread and
    // generating fills is matching logic, implemented in matchOrder().
    if (order.side == Side::BUY) {
        PriceLevel& level = bids_[order.price];
        level.price = order.price;
        auto it = level.add_order(order);
        order_lookup_[order.order_id] = OrderLocation{order.side, order.price, it};
    } else {
        PriceLevel& level = asks_[order.price];
        level.price = order.price;
        auto it = level.add_order(order);
        order_lookup_[order.order_id] = OrderLocation{order.side, order.price, it};
    }
}

bool OrderBook::cancelOrder(int order_id) {
    auto lookup_it = order_lookup_.find(order_id);
    if (lookup_it == order_lookup_.end()) {
        return false;
    }

    const OrderLocation& loc = lookup_it->second;
    if (loc.side == Side::BUY) {
        auto map_it = bids_.find(loc.price);
        map_it->second.remove_order(loc.it);
        if (map_it->second.orders.empty()) {
            bids_.erase(map_it);
        }
    } else {
        auto map_it = asks_.find(loc.price);
        map_it->second.remove_order(loc.it);
        if (map_it->second.orders.empty()) {
            asks_.erase(map_it);
        }
    }

    order_lookup_.erase(lookup_it);
    return true;
}

bool OrderBook::modifyOrder(int order_id, int new_price, int new_quantity) {
    // TODO: implement order modification in a later session.
    (void)order_id;
    (void)new_price;
    (void)new_quantity;
    return false;
}

std::size_t OrderBook::bidLevelCount() const {
    return bids_.size();
}

std::size_t OrderBook::askLevelCount() const {
    return asks_.size();
}

std::vector<Trade> OrderBook::matchOrder(const Order& order) {
    // Intentionally deferred: matching logic (crossing the spread, walking
    // price levels in price-time priority, and generating fills) is not yet
    // implemented. Scoped for later.
    (void)order;
    return {};
}
