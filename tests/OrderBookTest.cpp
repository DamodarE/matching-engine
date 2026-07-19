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
