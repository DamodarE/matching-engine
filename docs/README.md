# High-Performance Limit Order Matching Engine

A production-inspired limit order matching engine written in modern C++20.

---

## Goal

Build a high-performance matching engine implementing price-time priority,
benchmarking millions of simulated orders, and demonstrating systems programming
fundamentals including memory management, cache locality, and performance optimization.

---

## Status

🚧 In Development

---

## Planned Features

- [ ] Core matching engine
- [ ] Price-time priority
- [ ] Limit buy and sell orders
- [ ] Partial fills
- [ ] Order cancellation
- [ ] Order modification
- [ ] Unit test suite
- [ ] Benchmark suite
- [ ] Memory optimization
- [ ] Profiling and performance analysis

---

## Architecture

The engine is built around four core components:

- **Order Book** — two red-black trees (via `std::map`) maintaining price levels for bids and asks
- **Price Level** — a FIFO doubly linked list of orders at each price point
- **Order Lookup** — an `unordered_map` providing O(1) access to any order by ID
- **Matching Engine** — processes incoming orders against the book and generates trade records

See `docs/design.md` for full data structure decisions and tradeoffs.

---

## Documentation

| Document | Description |
|---|---|
| `docs/glossary.md` | Trading terminology reference |
| `docs/design.md` | Data model and design decisions |
| `docs/matching-flow.md` | Order matching flowchart |
| `docs/scenarios.md` | Matching scenarios and edge cases |
| `docs/roadmap.md` | Project roadmap and versioning |

---

## Project Structure

```text
matching-engine/
├── README.md
├── CMakeLists.txt
├── docs/
├── src/
├── include/
├── tests/
├── benchmarks/
└── scripts/
```

---

## Development Phases

| Phase | Focus | Status |
|---|---|---|
| V1 | Core matching engine + unit tests | 🚧 In Progress |
| V2 | Benchmarking suite | ⬜ Planned |
| V3 | Performance optimization | ⬜ Planned |

---

## Built With

- C++20
- CMake
- Google Test