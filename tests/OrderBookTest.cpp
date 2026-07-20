#include <gtest/gtest.h>

#include "OrderBook.h"
#include "Order.h"

TEST(OrderBookTest, AddThenCancelRemovesEmptyPriceLevel) {
    OrderBook book;
    Order order{1, Side::BUY, 100, 10, 1};

    book.addOrder(order);
    ASSERT_EQ(book.bidLevelCount(), 1u);

    EXPECT_TRUE(book.cancelOrder(1));
    EXPECT_EQ(book.bidLevelCount(), 0u);
}

TEST(OrderBookTest, CancelOrderNeverAddedReturnsFalse) {
    OrderBook book;

    EXPECT_FALSE(book.cancelOrder(999));
}

TEST(OrderBookTest, CancelOneOfTwoAtSamePriceLeavesOtherOrder) {
    // Mirrors docs/scenarios.md Scenario 10: cancelling one order at a price
    // level must not remove the level while other orders remain there.
    OrderBook book;
    Order order1{1, Side::BUY, 50, 100, 1};
    Order order2{2, Side::BUY, 50, 50, 2};

    book.addOrder(order1);
    book.addOrder(order2);
    ASSERT_EQ(book.bidLevelCount(), 1u);

    EXPECT_TRUE(book.cancelOrder(1));
    EXPECT_EQ(book.bidLevelCount(), 1u);

    // Order#2 is still findable and cancellable, proving it survived.
    EXPECT_TRUE(book.cancelOrder(2));
    EXPECT_EQ(book.bidLevelCount(), 0u);

    // Order#1 was already removed; cancelling again must fail.
    EXPECT_FALSE(book.cancelOrder(1));
}

TEST(OrderBookMatchTest, EmptyBookRestsWholeOrderNoTrades) {
    // Mirrors docs/scenarios.md Scenario 1: nothing to cross against, so the
    // whole incoming order rests.
    OrderBook book;
    Order order{1, Side::BUY, 50, 100, 1};

    auto trades = book.matchOrder(order);

    EXPECT_TRUE(trades.empty());
    ASSERT_EQ(book.bidLevelCount(), 1u);
    EXPECT_EQ(book.bestBidPrice(), 50);
    EXPECT_EQ(book.bestBidQuantity(), 100);
}

TEST(OrderBookMatchTest, NonCrossingPriceRestsWholeOrderNoTrades) {
    // Mirrors docs/scenarios.md Scenario 3: a resting ask exists, but the
    // incoming buy's limit is below it, so there is nothing to cross.
    OrderBook book;
    Order restingSell{1, Side::SELL, 60, 100, 1};
    book.addOrder(restingSell);

    Order incomingBuy{2, Side::BUY, 50, 100, 2};
    auto trades = book.matchOrder(incomingBuy);

    EXPECT_TRUE(trades.empty());
    ASSERT_EQ(book.bidLevelCount(), 1u);
    EXPECT_EQ(book.bestBidPrice(), 50);
    EXPECT_EQ(book.bestBidQuantity(), 100);
    ASSERT_EQ(book.askLevelCount(), 1u);
    EXPECT_EQ(book.bestAskPrice(), 60);
}

TEST(OrderBookMatchTest, PerfectMatchFullyFillsBothSides) {
    // Mirrors docs/scenarios.md Scenario 5.
    OrderBook book;
    Order restingBuy{1, Side::BUY, 50, 100, 1};
    book.addOrder(restingBuy);

    Order incomingSell{2, Side::SELL, 50, 100, 2};
    auto trades = book.matchOrder(incomingSell);

    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].buy_order_id, 1);
    EXPECT_EQ(trades[0].sell_order_id, 2);
    EXPECT_EQ(trades[0].price, 50);
    EXPECT_EQ(trades[0].quantity, 100);

    EXPECT_EQ(book.bidLevelCount(), 0u);
    EXPECT_EQ(book.askLevelCount(), 0u);
}

TEST(OrderBookMatchTest, LargerIncomingBuyPartiallyFillsAndRestsRemainder) {
    // Mirrors docs/scenarios.md Scenario 6.
    OrderBook book;
    Order restingSell{1, Side::SELL, 50, 30, 1};
    book.addOrder(restingSell);

    Order incomingBuy{2, Side::BUY, 50, 100, 2};
    auto trades = book.matchOrder(incomingBuy);

    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].buy_order_id, 2);
    EXPECT_EQ(trades[0].sell_order_id, 1);
    EXPECT_EQ(trades[0].price, 50);
    EXPECT_EQ(trades[0].quantity, 30);

    EXPECT_EQ(book.askLevelCount(), 0u);
    ASSERT_EQ(book.bidLevelCount(), 1u);
    EXPECT_EQ(book.bestBidPrice(), 50);
    EXPECT_EQ(book.bestBidQuantity(), 70);
}

TEST(OrderBookMatchTest, LargerIncomingSellPartiallyFillsAndRestsRemainder) {
    // Mirrors docs/scenarios.md Scenario 7.
    OrderBook book;
    Order restingBuy{1, Side::BUY, 50, 30, 1};
    book.addOrder(restingBuy);

    Order incomingSell{2, Side::SELL, 50, 100, 2};
    auto trades = book.matchOrder(incomingSell);

    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].buy_order_id, 1);
    EXPECT_EQ(trades[0].sell_order_id, 2);
    EXPECT_EQ(trades[0].price, 50);
    EXPECT_EQ(trades[0].quantity, 30);

    EXPECT_EQ(book.bidLevelCount(), 0u);
    ASSERT_EQ(book.askLevelCount(), 1u);
    EXPECT_EQ(book.bestAskPrice(), 50);
    EXPECT_EQ(book.bestAskQuantity(), 70);
}

TEST(OrderBookMatchTest, BuyWalksMultiplePriceLevels) {
    // Mirrors docs/scenarios.md Scenario 8.
    OrderBook book;
    book.addOrder(Order{1, Side::SELL, 50, 40, 1});
    book.addOrder(Order{2, Side::SELL, 51, 40, 2});
    book.addOrder(Order{3, Side::SELL, 52, 40, 3});

    Order incomingBuy{4, Side::BUY, 52, 100, 4};
    auto trades = book.matchOrder(incomingBuy);

    ASSERT_EQ(trades.size(), 3u);

    EXPECT_EQ(trades[0].sell_order_id, 1);
    EXPECT_EQ(trades[0].price, 50);
    EXPECT_EQ(trades[0].quantity, 40);

    EXPECT_EQ(trades[1].sell_order_id, 2);
    EXPECT_EQ(trades[1].price, 51);
    EXPECT_EQ(trades[1].quantity, 40);

    EXPECT_EQ(trades[2].sell_order_id, 3);
    EXPECT_EQ(trades[2].price, 52);
    EXPECT_EQ(trades[2].quantity, 20);

    for (const auto& trade : trades) {
        EXPECT_EQ(trade.buy_order_id, 4);
    }

    // Buy order fully filled (40+40+20=100), nothing rests on the bid side.
    EXPECT_EQ(book.bidLevelCount(), 0u);
    ASSERT_EQ(book.askLevelCount(), 1u);
    EXPECT_EQ(book.bestAskPrice(), 52);
    EXPECT_EQ(book.bestAskQuantity(), 20);
}

TEST(OrderBookMatchTest, SellWalksMultiplePriceLevels) {
    // Mirrors docs/scenarios.md Scenario 9.
    OrderBook book;
    book.addOrder(Order{1, Side::BUY, 52, 40, 1});
    book.addOrder(Order{2, Side::BUY, 51, 40, 2});
    book.addOrder(Order{3, Side::BUY, 50, 40, 3});

    Order incomingSell{4, Side::SELL, 50, 100, 4};
    auto trades = book.matchOrder(incomingSell);

    ASSERT_EQ(trades.size(), 3u);

    EXPECT_EQ(trades[0].buy_order_id, 1);
    EXPECT_EQ(trades[0].price, 52);
    EXPECT_EQ(trades[0].quantity, 40);

    EXPECT_EQ(trades[1].buy_order_id, 2);
    EXPECT_EQ(trades[1].price, 51);
    EXPECT_EQ(trades[1].quantity, 40);

    EXPECT_EQ(trades[2].buy_order_id, 3);
    EXPECT_EQ(trades[2].price, 50);
    EXPECT_EQ(trades[2].quantity, 20);

    for (const auto& trade : trades) {
        EXPECT_EQ(trade.sell_order_id, 4);
    }

    EXPECT_EQ(book.askLevelCount(), 0u);
    ASSERT_EQ(book.bidLevelCount(), 1u);
    EXPECT_EQ(book.bestBidPrice(), 50);
    EXPECT_EQ(book.bestBidQuantity(), 20);
}

TEST(OrderBookMatchTest, SamePriceLevelRespectsTimePriority) {
    // Mirrors docs/scenarios.md Scenario 12: Order#1 arrived before Order#2
    // at the same price level, so Order#1 must be filled first.
    OrderBook book;
    book.addOrder(Order{1, Side::BUY, 50, 50, 1});
    book.addOrder(Order{2, Side::BUY, 50, 50, 2});

    Order incomingSell{3, Side::SELL, 50, 50, 3};
    auto trades = book.matchOrder(incomingSell);

    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].buy_order_id, 1);
    EXPECT_EQ(trades[0].quantity, 50);

    // Order#1 was consumed; Order#2 survives untouched at the head of the list.
    EXPECT_FALSE(book.cancelOrder(1));
    EXPECT_TRUE(book.cancelOrder(2));
}

TEST(OrderBookModifyTest, QuantityDecreaseSamePricePreservesTimePriority) {
    // Mirrors docs/scenarios.md Scenario 14.
    OrderBook book;
    book.addOrder(Order{1, Side::BUY, 50, 100, 1});
    book.addOrder(Order{2, Side::BUY, 50, 50, 2});

    EXPECT_TRUE(book.modifyOrder(1, 50, 40));

    ASSERT_EQ(book.bidLevelCount(), 1u);
    EXPECT_EQ(book.bestBidPrice(), 50);
    EXPECT_EQ(book.bestBidQuantity(), 90); // 40 + 50

    // Order#1 must still be at the head of the queue: a sell that exactly
    // matches its new quantity should fill Order#1, not Order#2.
    Order incomingSell{3, Side::SELL, 50, 40, 3};
    auto trades = book.matchOrder(incomingSell);

    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].buy_order_id, 1);
    EXPECT_EQ(trades[0].quantity, 40);

    // Order#1 fully consumed; Order#2 still resting untouched.
    EXPECT_FALSE(book.cancelOrder(1));
    EXPECT_TRUE(book.cancelOrder(2));
}

TEST(OrderBookModifyTest, QuantityIncreaseSamePriceLosesTimePriority) {
    // Mirrors docs/scenarios.md Scenario 15.
    OrderBook book;
    book.addOrder(Order{1, Side::BUY, 50, 40, 1});
    book.addOrder(Order{2, Side::BUY, 50, 50, 2});

    EXPECT_TRUE(book.modifyOrder(1, 50, 90));

    ASSERT_EQ(book.bidLevelCount(), 1u);
    EXPECT_EQ(book.bestBidPrice(), 50);
    EXPECT_EQ(book.bestBidQuantity(), 140); // 90 + 50

    // Order#2 now has priority: a sell matching its size should fill Order#2
    // first, even though Order#1 was the one already resting at this price.
    Order incomingSell{3, Side::SELL, 50, 50, 3};
    auto trades = book.matchOrder(incomingSell);

    ASSERT_EQ(trades.size(), 1u);
    EXPECT_EQ(trades[0].buy_order_id, 2);
    EXPECT_EQ(trades[0].quantity, 50);

    // Order#2 fully consumed; Order#1 still resting with its new quantity.
    EXPECT_FALSE(book.cancelOrder(2));
    EXPECT_TRUE(book.cancelOrder(1));
}

TEST(OrderBookModifyTest, PriceChangeLosesTimePriorityAndCrossesSpread) {
    // Mirrors docs/scenarios.md Scenario 16.
    OrderBook book;
    book.addOrder(Order{1, Side::BUY, 50, 100, 1});
    book.addOrder(Order{2, Side::SELL, 52, 60, 2});

    EXPECT_TRUE(book.modifyOrder(1, 52, 100));

    // Order#1 re-entered at $52 as a new incoming order and crossed the
    // spread against Order#2 instead of resting behind it.
    EXPECT_EQ(book.askLevelCount(), 0u);
    ASSERT_EQ(book.bidLevelCount(), 1u);
    EXPECT_EQ(book.bestBidPrice(), 52);
    EXPECT_EQ(book.bestBidQuantity(), 40); // 100 - 60

    // Order#2 was fully filled by the cross and is gone from the book.
    EXPECT_FALSE(book.cancelOrder(2));
    // Order#1's remainder rests in the book under the same order_id.
    EXPECT_TRUE(book.cancelOrder(1));
}

TEST(OrderBookModifyTest, ModifyNonexistentOrderReturnsFalseAndLeavesBookUnchanged) {
    // Mirrors docs/scenarios.md Scenario 17.
    OrderBook book;
    book.addOrder(Order{1, Side::BUY, 50, 100, 1});

    EXPECT_FALSE(book.modifyOrder(99, 50, 50));

    ASSERT_EQ(book.bidLevelCount(), 1u);
    EXPECT_EQ(book.bestBidPrice(), 50);
    EXPECT_EQ(book.bestBidQuantity(), 100);
    EXPECT_TRUE(book.cancelOrder(1));
}
