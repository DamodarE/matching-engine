#pragma once

#include <iterator>
#include <list>

#include "Order.h"

// std::list is a doubly linked list: push_back, pop_front, and erase(iterator)
// are all O(1), and iterators stay valid across insertions/removals elsewhere
// in the list. That's exactly the OrderNode* access pattern design.md calls
// for, so no custom linked list is needed.
struct PriceLevel {
    int price;
    std::list<Order> orders;
    int total_quantity = 0;

    using OrderIt = std::list<Order>::iterator;

    OrderIt add_order(const Order& order) {
        total_quantity += order.quantity;
        orders.push_back(order);
        return std::prev(orders.end());
    }

    void remove_order(OrderIt it) {
        total_quantity -= it->quantity;
        orders.erase(it);
    }
};
