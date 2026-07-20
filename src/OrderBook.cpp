#include "OrderBook.h"

#include <algorithm>

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
    auto lookup_it = order_lookup_.find(order_id);
    if (lookup_it == order_lookup_.end()) {
        return false;
    }

    const OrderLocation loc = lookup_it->second;
    Order& existing = *loc.it;

    if (new_price == loc.price && new_quantity <= existing.quantity) {
        // Quantity decrease (or unchanged) at the same price preserves time
        // priority: mutate in place, keep the order's position in the queue.
        int delta = existing.quantity - new_quantity;
        existing.quantity = new_quantity;

        if (loc.side == Side::BUY) {
            bids_.find(loc.price)->second.total_quantity -= delta;
        } else {
            asks_.find(loc.price)->second.total_quantity -= delta;
        }
        return true;
    }

    // Quantity increase or any price change forfeits time priority: cancel
    // the resting order and resubmit it as a brand-new incoming order so it
    // goes through the normal matching path, including crossing the spread.
    Order resubmitted = existing;
    resubmitted.price = new_price;
    resubmitted.quantity = new_quantity;
    resubmitted.timestamp = existing.timestamp + 1;

    cancelOrder(order_id);
    matchOrder(resubmitted);
    return true;
}

std::size_t OrderBook::bidLevelCount() const {
    return bids_.size();
}

std::size_t OrderBook::askLevelCount() const {
    return asks_.size();
}

int OrderBook::bestBidPrice() const {
    return bids_.begin()->first;
}

int OrderBook::bestBidQuantity() const {
    return bids_.begin()->second.total_quantity;
}

int OrderBook::bestAskPrice() const {
    return asks_.begin()->first;
}

int OrderBook::bestAskQuantity() const {
    return asks_.begin()->second.total_quantity;
}

std::vector<Trade> OrderBook::matchOrder(const Order& order) {
    // Entry point for a brand-new incoming order (see docs/matching-flow.md).
    // Walks the opposite side in price-time priority while the incoming
    // order's limit crosses the best resting price, filling from the front
    // of each price level's FIFO list. Any unfilled remainder is rested in
    // the book via addOrder(), so callers only need to call matchOrder() to
    // fully process an incoming order.
    std::vector<Trade> trades;
    int remaining_qty = order.quantity;

    if (order.side == Side::BUY) {
        while (remaining_qty > 0 && !asks_.empty()) {
            auto level_it = asks_.begin();
            if (order.price < level_it->first) {
                break; // best ask is above the buy limit, no more crosses
            }
            PriceLevel& level = level_it->second;

            while (remaining_qty > 0 && !level.orders.empty()) {
                Order& resting = level.orders.front();
                int fill_qty = std::min(remaining_qty, resting.quantity);

                // Trade prints at the resting order's price (the order that
                // was already in the book), per Scenario 5-9.
                trades.push_back(Trade{order.order_id, resting.order_id,
                                        resting.price, fill_qty,
                                        order.timestamp});

                remaining_qty -= fill_qty;
                resting.quantity -= fill_qty;
                level.total_quantity -= fill_qty;

                if (resting.quantity == 0) {
                    order_lookup_.erase(resting.order_id);
                    level.orders.pop_front();
                }
            }

            if (level.orders.empty()) {
                asks_.erase(level_it);
            }
        }
    } else {
        while (remaining_qty > 0 && !bids_.empty()) {
            auto level_it = bids_.begin();
            if (order.price > level_it->first) {
                break; // best bid is below the sell limit, no more crosses
            }
            PriceLevel& level = level_it->second;

            while (remaining_qty > 0 && !level.orders.empty()) {
                Order& resting = level.orders.front();
                int fill_qty = std::min(remaining_qty, resting.quantity);

                trades.push_back(Trade{resting.order_id, order.order_id,
                                        resting.price, fill_qty,
                                        order.timestamp});

                remaining_qty -= fill_qty;
                resting.quantity -= fill_qty;
                level.total_quantity -= fill_qty;

                if (resting.quantity == 0) {
                    order_lookup_.erase(resting.order_id);
                    level.orders.pop_front();
                }
            }

            if (level.orders.empty()) {
                bids_.erase(level_it);
            }
        }
    }

    if (remaining_qty > 0) {
        Order remainder = order;
        remainder.quantity = remaining_qty;
        addOrder(remainder);
    }

    return trades;
}
