# Order Matching Flow

## Main Entry Point

Order Arrives
      │
      ▼
What type of request?
      │
      ├──── CANCEL ──────────────────────────────────────────────┐
      │                                                           │
      ▼                                                           ▼
 BUY or SELL?                                          Look up Order ID in hashmap
      │                                                           │
      ├──── BUY                        ┌── Not Found ────► Return Error
      │                                │
      └──── SELL                       └── Found ──► Remove from linked list
                                                         │
                                                         ▼
                                             Was it the last order
                                             at that price level?
                                                         │
                                             ┌───────────┴───────────┐
                                             │                       │
                                            Yes                      No
                                             │                       │
                                             ▼                       ▼
                                     Remove price level           Done
                                     from BST, update
                                     best bid/ask pointer

---

## BUY Order Flow

BUY Order Arrives
      │
      ▼
Look at SELL side of book
      │
      ▼
Is there a best ask?
      │
      ├──── No ──► Add BUY order to BUY side of book at price level
      │                        │
      │                        ▼
      │            Price level exists? Add to end of FIFO list
      │            Price level new?   Create level, insert BST
      │
      └──── Yes
               │
               ▼
     buy_limit_price >= best_ask?
               │
               ├──── No ──► Add BUY order to BUY side of book
               │
               └──── Yes
                        │
                        ▼
              Execute trade at ask price
              fill_qty = min(buy_qty, ask_qty)
                        │
                        ▼
              Remove filled SELL orders from
              front of FIFO list at that price level
                        │
                        ▼
              Was that the last SELL at this price level?
                        │
               ┌────────┴────────┐
               │                 │
              Yes                No
               │                 │
               ▼                 ▼
      Remove price level      Continue
      from BST, update
      best ask pointer
               │
               ▼
      BUY order fully filled?
               │
        ┌──────┴──────┐
        │             │
       Yes            No (partial fill)
        │             │
        ▼             ▼
       Done     Reduce BUY quantity by fill_qty
                      │
                      ▼
               Go back to top of BUY flow
               with remaining quantity

---

## SELL Order Flow

SELL Order Arrives
      │
      ▼
Look at BUY side of book
      │
      ▼
Is there a best bid?
      │
      ├──── No ──► Add SELL order to SELL side of book at price level
      │                        │
      │                        ▼
      │            Price level exists? Add to end of FIFO list
      │            Price level new?   Create level, insert BST
      │
      └──── Yes
               │
               ▼
     sell_limit_price <= best_bid?
               │
               ├──── No ──► Add SELL order to SELL side of book
               │
               └──── Yes
                        │
                        ▼
              Execute trade at bid price
              fill_qty = min(sell_qty, bid_qty)
                        │
                        ▼
              Remove filled BUY orders from
              front of FIFO list at that price level
                        │
                        ▼
              Was that the last BUY at this price level?
                        │
               ┌────────┴────────┐
               │                 │
              Yes                No
               │                 │
               ▼                 ▼
      Remove price level      Continue
      from BST, update
      best bid pointer
               │
               ▼
      SELL order fully filled?
               │
        ┌──────┴──────┐
        │             │
       Yes            No (partial fill)
        │             │
        ▼             ▼
       Done     Reduce SELL quantity by fill_qty
                      │
                      ▼
               Go back to top of SELL flow
               with remaining quantity

