#include "lob/event.hpp"
#include "lob/order_book.hpp"
#include "lob/types.hpp"

#include <catch2/catch_test_macros.hpp>
#include <optional>

/*
struct Event {
    EventType type {};
    OrderId order_id {};
    Side side {};
    Price price {};
    Qty qty {};
    Timestamp ts {};
};
*/

TEST_CASE("new order book starts empty") {
    lob::OrderBook book {};
    REQUIRE(book.order_count() == 0);
    REQUIRE_FALSE(book.best_bid().has_value());
    REQUIRE_FALSE(book.best_ask().has_value());
}

TEST_CASE("add and cancel one order") {
    using namespace lob;

    OrderBook book {};
    Event event1{EventType::Add, OrderId{1}, Side::Buy, Price{100}, Qty{10}, Timestamp{1}};
    REQUIRE(book.apply(event1));
    REQUIRE(book.order_count() == 1);
    REQUIRE(book.best_bid().has_value());
    REQUIRE(book.best_bid() == Price{100});

    Event event2{EventType::Cancel, OrderId{1}, Side::Cancel, 0, 0, 2};
    REQUIRE(book.apply(event2));
    REQUIRE(book.order_count() == 0);
    REQUIRE_FALSE(book.best_bid().has_value());
}
