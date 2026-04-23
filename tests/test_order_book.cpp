#include "lob/order_book.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("new order book starts empty") {
    lob::OrderBook book {};
    REQUIRE(book.order_count() == 0);
}
