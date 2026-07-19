#pragma once

#include <cstdint>

enum class Side { BUY, SELL };

enum class OrderType { LIMIT };

struct Order {
    int order_id;
    Side side;
    int price;       // fixed-point integer, e.g. $10.25 -> 1025 (see docs/design.md)
    int quantity;
    uint64_t timestamp; // monotonically increasing sequence number
};
