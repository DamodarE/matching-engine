# High-Performance Limit Order Matching Engine
## Project Instructions & Vision

Last Updated: July 2026

---

# Purpose

This project is a production-inspired implementation of a high-performance limit order matching engine written in modern C++.

The goal is not to recreate the complexity of a real-world exchange such as NASDAQ or the NYSE. Instead, the objective is to design and implement a clean, efficient, and well-documented matching engine that demonstrates strong computer science fundamentals, systems programming, and performance engineering.

This project should reflect how a professional software engineer approaches system design, optimization, benchmarking, and software architecture.

---

# Why This Project Exists

Most student software projects focus heavily on web development or AI integrations.

While valuable, they rarely demonstrate understanding of:

- Data structures
- Algorithms
- Memory management
- Cache locality
- Performance optimization
- Systems programming
- Concurrency
- Benchmarking

This project exists to fill that gap.

Upon completion, it should clearly demonstrate an ability to design efficient software rather than simply assemble existing frameworks.

---

# Primary Goals

The project should emphasize the following engineering principles.

## 1. Correctness

Correct behavior always comes before optimization.

The engine should faithfully implement:

- Price-time priority
- Limit order matching
- Partial fills
- Complete fills
- Order cancellation
- Order modification

The system must behave predictably for all edge cases.

---

## 2. Performance

After correctness has been achieved, the implementation should be optimized through measurement rather than guesswork.

Performance improvements should be driven by profiling and benchmarking.

Potential optimization areas include:

- Cache locality
- Memory allocation
- Data structure selection
- Branch prediction
- Copy elimination
- Lock contention (future)

Every optimization should have measurable impact.

---

## 3. Clean Architecture

The project should prioritize readability and maintainability.

Code should be modular.

Responsibilities should be clearly separated.

Large functions should be avoided.

The project should be understandable by another engineer without external explanation.

---

## 4. Engineering Documentation

This repository should document not only what was built, but why.

Important design decisions should be recorded throughout development.

Tradeoffs should be discussed.

Alternative implementations should be considered.

The repository should tell the engineering story behind the project.

---

# Project Scope

## Version 1 (Minimum Viable Product)

Implement:

- Limit buy orders
- Limit sell orders
- Price-time priority
- Partial fills
- Complete fills
- Order cancellation
- Order modification
- Unit tests

---

## Version 2

Introduce performance tooling.

- Benchmark suite
- Random order generation
- Throughput measurement
- Latency measurement
- Complexity analysis

---

## Version 3

Optimization phase.

Potential improvements include:

- Memory pools
- Cache-aware data structures
- Reduced allocations
- Improved lookup structures
- Profiling-guided optimization

---

## Stretch Goals

Only after the previous milestones are complete.

Possible extensions include:

- Multi-threaded processing
- Lock-free queues
- Networked order gateway
- Multiple trading symbols
- Market orders
- Historical replay
- Live visualization
- Order book snapshots

These are optional and should never come at the expense of code quality.

---

# Non-Goals

The following are intentionally out of scope.

- Building a complete stock exchange
- Financial modeling
- Trading strategies
- Authentication
- User accounts
- Web frontends
- Databases
- Cloud deployment

This project is a systems programming project—not a web application.

---

# Development Philosophy

This project follows several principles.

## Correctness before performance.

Never optimize incorrect software.

---

## Measure before optimizing.

Every optimization should be supported by benchmark data.

---

## Simplicity over cleverness.

Readable code is preferred over unnecessarily complex implementations.

---

## Professional engineering practices.

The repository should resemble a professional open-source systems project.

This includes:

- meaningful commits
- clear documentation
- automated testing
- consistent formatting
- organized project structure

---

# Success Criteria

A successful project should demonstrate the following.

### Functional

- Correct order matching
- Robust handling of edge cases
- Comprehensive automated tests

### Performance

- Benchmark suite
- Latency measurements
- Throughput measurements
- Profiling results

### Engineering

- Clean architecture
- Well-documented code
- Professional README
- Design documentation
- Clear commit history

---

# Learning Objectives

By the end of this project, I should have a deeper understanding of:

- Modern C++
- Memory management
- Data structures
- Algorithms
- Object-oriented design
- Systems programming
- Benchmarking
- Profiling
- Performance engineering
- Cache behavior
- Software architecture

---

# Repository Standards

Every commit should represent a meaningful engineering improvement.

Preferred commit prefixes include:

- feat:
- fix:
- perf:
- test:
- docs:
- refactor:
- build:
- ci:

Avoid meaningless commits or commits created solely to increase contribution counts.

---

# Guiding Principle

This project is not about writing the fastest matching engine ever built.

It is about demonstrating the engineering process:

1. Understand the problem.
2. Design the system.
3. Build a correct implementation.
4. Measure performance.
5. Identify bottlenecks.
6. Optimize using evidence.
7. Document decisions and tradeoffs.

If these principles are followed, the project will serve as a strong demonstration of software engineering ability, systems programming knowledge, and performance-oriented design.