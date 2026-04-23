#include "lob/order_book.hpp"

#include <iostream>

int main() {
    lob::OrderBook book {};

    lob::Event event {
        .type = lob::EventType::Add,
        .order_id = 1,
        .side = lob::Side::Buy,
        .price = 100,
        .qty = 10,
        .ts = 1
    };

    book.apply(event);

    std::cout << "LOB project builds successfully\n";
    return 0;
}
