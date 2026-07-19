# Design Document

## Overview

This document outlines the core data model and data structure decisions for the matching engine.
No implementation details are included here — this captures the reasoning behind the design
before any code is written.

---

## Core Data Structures

### Order

An order represents a single instruction to buy or sell a quantity of shares at a specified price.

Fields:

- `order_id` — unique identifier for the order
- `side` — BUY or SELL
- `price` — stored as a fixed-point integer (see Price Representation below)
- `quantity` — number of shares
- `timestamp` — time the order was received, used for time priority

---

### Price Level

A price level represents all orders resting at a single price on one side of the book.

Each price level contains:

- `price` — the price this level represents
- `orders` — a FIFO doubly linked list of orders at this price
- `total_quantity` — sum of all order quantities at this level

Orders within a price level are stored in a doubly linked list sorted by arrival time.
New orders are appended to the tail. Matching always consumes from the head.
This enforces time priority within a price level.

---

### Order Book

The order book maintains all resting orders on both sides of the market.

Two separate balanced BSTs (red-black trees via `std::map`) hold the price levels:

- **Bid side** — `std::map<int, PriceLevel, std::greater<int>>`
  - Descending order so the best bid (highest price) is always at `begin()`
- **Ask side** — `std::map<int, PriceLevel>`
  - Ascending order so the best ask (lowest price) is always at `begin()`

Each map is keyed by price (fixed-point integer).

A pointer to `begin()` on each side gives O(1) access to the best bid and best ask.

---

### Order Lookup

A `std::unordered_map<int, OrderNode*>` maps order IDs to their corresponding node
in the linked list at their price level.

This enables:

- O(1) order lookup by ID
- O(1) cancellation in the common case
- O(log n) worst case when cancellation removes the last order at a price level
  and the price level must be removed from the BST

---

### Trade

A trade record is generated every time two orders match.

Fields:

- `buy_order_id` — ID of the aggressing or resting buy order
- `sell_order_id` — ID of the aggressing or resting sell order
- `price` — the price at which the trade executed
- `quantity` — the number of shares exchanged
- `timestamp` — time the trade occurred

The matching function returns a list of trades rather than mutating state silently.
This makes the engine's behavior observable and easy to test.

---

## Price Representation

All prices are stored as fixed-point integers rather than floating-point values.

For example, $10.25 is stored as `1025` with an implied divisor of `100`.

### Why not floating point?

Floating-point arithmetic (double, float) cannot exactly represent many decimal values.
For example, `0.1 + 0.2` does not equal `0.3` in IEEE 754 arithmetic.

In a financial system this is unacceptable. Rounding errors in price comparisons
could cause incorrect matches or missed matches.

### Why fixed-point integers?

Integer arithmetic is exact. Comparisons are reliable.
Integer operations are also faster than floating-point operations on most hardware.
Real exchanges use fixed-point or integer representations for exactly this reason.

---

## Time Complexity Summary

| Operation              | Complexity       | Notes                                              |
|------------------------|------------------|----------------------------------------------------|
| Insert order           | O(log n)         | BST insertion at price level                       |
| Cancel order           | O(1) / O(log n)  | O(1) via hashmap, O(log n) if price level removed  |
| Find best bid/ask      | O(1)             | Always at begin() of each map                      |
| Match order            | O(k log n)       | k = number of price levels consumed                |
| Look up order by ID    | O(1)             | Hashmap lookup                                     |

---

## Tradeoffs Considered

### Why std::map over a heap?

A heap provides O(1) access to the best price but O(n) access to arbitrary price levels.
When a price level is exhausted and the engine needs the next best price, a heap requires
a linear scan. A BST gives O(log n) for all operations and maintains full order book state.

### Why std::map over a sorted array?

Sorted arrays require O(n) shifts on insertion and deletion.
A BST gives O(log n) for both. At high order throughput, O(n) insertion is not acceptable.

### Why a doubly linked list within each price level?

A doubly linked list allows O(1) insertion at the tail and O(1) removal from the head,
which is exactly the access pattern needed for FIFO time priority.
It also allows O(1) removal of any node given a pointer to it, which is used during cancellation.

### Why fixed-point integers over doubles?

Exact arithmetic, no rounding errors, faster operations.
This is the industry standard approach for financial systems.