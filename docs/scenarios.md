## Scenario 1 — Empty book, BUY arrives

Book before:
BUY  side: empty
SELL side: empty

Incoming: BUY 100 shares @ $50

Steps:
1. Look for the best ask if there one
2. Add to BUY book because no best bid

Book after:
BUY  side: $50 → [100 shares]
SELL side: empty

## Scenario 2 — Empty book, SELL arrives

Book before:
BUY  side: empty
SELL side: empty

Incoming: SELL 100 shares @ $50

Steps:
1. Look for the best bid if there one
2. Add to SELL book because no best bid

Book after:
BUY  side: empty
SELL side: $50 → [100 shares]

## Scenario 3 - BUY arrives, no matching SELL (price too low)

Book before:
BUY  side: empty
SELL side: $60 -> (100 shares)

Incoming: BUY 100 shares @ $50

Steps:
1. Look for the best ask if there is one
2. Add to BUY book because there is no matching or lower price than the bid

Book after:
BUY  side: $50 → [100 shares]
SELL side: $60 → [100 shares]

## Scenario 4 — SELL arrives, no matching BUY (price too high)

Book before:
BUY  side: $40 → [100 shares]
SELL side: empty

Incoming: SELL 100 shares @ $50

Steps:
1. Look for the best bid
2. Best bid is $40, sell limit is $50
3. $50 > $40, no match
4. Add SELL order to SELL side of book

Book after:
BUY  side: $40 → [100 shares]
SELL side: $50 → [100 shares]

---

## Scenario 5 — Perfect match, full fill both sides

Book before:
BUY  side: $50 → [100 shares]
SELL side: empty

Incoming: SELL 100 shares @ $50

Steps:
1. Look for the best bid
2. Best bid is $50, sell limit is $50
3. $50 <= $50, match found
4. fill_qty = min(100, 100) = 100
5. Execute trade at $50 for 100 shares
6. Remove SELL order, fully filled
7. Remove BUY order, fully filled
8. Price level $50 on BUY side is now empty, remove from BST
9. Update best bid pointer

Book after:
BUY  side: empty
SELL side: empty

---

## Scenario 6 — BUY larger than SELL, partial fill

Book before:
BUY  side: empty
SELL side: $50 → [30 shares]

Incoming: BUY 100 shares @ $50

Steps:
1. Look for the best ask
2. Best ask is $50, buy limit is $50
3. $50 >= $50, match found
4. fill_qty = min(100, 30) = 30
5. Execute trade at $50 for 30 shares
6. SELL order fully filled, remove from list
7. Price level $50 on SELL side is now empty, remove from BST
8. Update best ask pointer
9. BUY order partially filled, 70 shares remaining
10. No more SELL orders in book, add remaining BUY to BUY side

Book after:
BUY  side: $50 → [70 shares]
SELL side: empty

---

## Scenario 7 — SELL larger than BUY, partial fill

Book before:
BUY  side: $50 → [30 shares]
SELL side: empty

Incoming: SELL 100 shares @ $50

Steps:
1. Look for the best bid
2. Best bid is $50, sell limit is $50
3. $50 <= $50, match found
4. fill_qty = min(100, 30) = 30
5. Execute trade at $50 for 30 shares
6. BUY order fully filled, remove from list
7. Price level $50 on BUY side is now empty, remove from BST
8. Update best bid pointer
9. SELL order partially filled, 70 shares remaining
10. No more BUY orders in book, add remaining SELL to SELL side

Book after:
BUY  side: empty
SELL side: $50 → [70 shares]

---

## Scenario 8 — BUY walks through multiple price levels

Book before:
BUY  side: empty
SELL side: $50 → [40 shares], $51 → [40 shares], $52 → [40 shares]

Incoming: BUY 100 shares @ $52

Steps:
1. Look for the best ask, which is $50
2. $52 >= $50, match found
3. fill_qty = min(100, 40) = 40
4. Execute trade at $50 for 40 shares
5. SELL at $50 fully filled, remove from list
6. Price level $50 empty, remove from BST, update best ask to $51
7. BUY has 60 shares remaining
8. Check new best ask, which is $51
9. $52 >= $51, match found
10. fill_qty = min(60, 40) = 40
11. Execute trade at $51 for 40 shares
12. SELL at $51 fully filled, remove from list
13. Price level $51 empty, remove from BST, update best ask to $52
14. BUY has 20 shares remaining
15. Check new best ask, which is $52
16. $52 >= $52, match found
17. fill_qty = min(20, 40) = 20
18. Execute trade at $52 for 20 shares
19. BUY fully filled, remove
20. SELL at $52 has 20 shares remaining

Book after:
BUY  side: empty
SELL side: $52 → [20 shares]

---

## Scenario 9 — SELL walks through multiple price levels

Book before:
BUY  side: $52 → [40 shares], $51 → [40 shares], $50 → [40 shares]
SELL side: empty

Incoming: SELL 100 shares @ $50

Steps:
1. Look for the best bid, which is $52
2. $50 <= $52, match found
3. fill_qty = min(100, 40) = 40
4. Execute trade at $52 for 40 shares
5. BUY at $52 fully filled, remove from list
6. Price level $52 empty, remove from BST, update best bid to $51
7. SELL has 60 shares remaining
8. Check new best bid, which is $51
9. $50 <= $51, match found
10. fill_qty = min(60, 40) = 40
11. Execute trade at $51 for 40 shares
12. BUY at $51 fully filled, remove from list
13. Price level $51 empty, remove from BST, update best bid to $50
14. SELL has 20 shares remaining
15. Check new best bid, which is $50
16. $50 <= $50, match found
17. fill_qty = min(20, 40) = 20
18. Execute trade at $50 for 20 shares
19. SELL fully filled, remove
20. BUY at $50 has 20 shares remaining

Book after:
BUY  side: $50 → [20 shares]
SELL side: empty

---

## Scenario 10 — Cancel an order that exists

Book before:
BUY  side: $50 → [Order#1: 100 shares, Order#2: 50 shares]
SELL side: empty

Incoming: CANCEL Order#1

Steps:
1. Look up Order#1 in hashmap
2. Found — locate node in linked list at price level $50
3. Remove Order#1 from linked list
4. Price level $50 still has Order#2, do not remove from BST
5. Delete Order#1 from hashmap

Book after:
BUY  side: $50 → [Order#2: 50 shares]
SELL side: empty

---

## Scenario 11 — Cancel an order that does not exist

Book before:
BUY  side: $50 → [Order#1: 100 shares]
SELL side: empty

Incoming: CANCEL Order#99

Steps:
1. Look up Order#99 in hashmap
2. Not found
3. Return error

Book after:
BUY  side: $50 → [Order#1: 100 shares]
SELL side: empty

---

## Scenario 12 — Two orders at same price, time priority

Book before:
BUY  side: $50 → [Order#1: 50 shares (08:00), Order#2: 50 shares (08:01)]
SELL side: empty

Incoming: SELL 50 shares @ $50

Steps:
1. Look for the best bid, which is $50
2. $50 <= $50, match found
3. Price level $50 has two orders — check time priority
4. Order#1 arrived first (08:00), fill it first
5. fill_qty = min(50, 50) = 50
6. Execute trade at $50 for 50 shares
7. Order#1 fully filled, remove from front of list
8. SELL fully filled, remove
9. Order#2 remains at head of list

Book after:
BUY  side: $50 → [Order#2: 50 shares (08:01)]
SELL side: empty

---

## Scenario 13 — Order fills last remaining order at a price level

Book before:
BUY  side: $50 → [Order#1: 100 shares]
SELL side: empty

Incoming: SELL 100 shares @ $50

Steps:
1. Look for the best bid, which is $50
2. $50 <= $50, match found
3. fill_qty = min(100, 100) = 100
4. Execute trade at $50 for 100 shares
5. Order#1 fully filled, remove from linked list
6. Price level $50 is now empty, remove from BST
7. Update best bid pointer to next level — none exists
8. Best bid is now null
9. SELL fully filled, remove

Book after:
BUY  side: empty
SELL side: empty
