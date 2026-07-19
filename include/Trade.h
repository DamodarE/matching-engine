#pragma once

#include <cstdint>

struct Trade {
    int buy_order_id;
    int sell_order_id;
    int price;
    int quantity;
    uint64_t timestamp;
};
