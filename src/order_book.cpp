#include "lob/order_book.hpp"
#include "lob/event.hpp"

#include <iostream>

namespace lob {

bool OrderBook::apply(const Event& event) {
    switch (event.type) {
        case lob::EventType::Cancel:
            return cancel_order(event.order_id);
        case lob::EventType::Add:
            return add_order(event);
        default:
            break;
    }
    return false;
}

bool OrderBook::add_order(const Event& event) {
    switch (event.side) {
        case lob::Side::Buy:
            process_add_order(
                bids, asks, event.order_id, event.side, event.price, event.qty,
                [](Price best_ask, Price incoming_bid) {
                    return best_ask <= incoming_bid;
                }
            );
            break;

        case lob::Side::Sell:
            process_add_order(
                asks, bids, event.order_id, event.side, event.price, event.qty,
                [](Price best_bid, Price incoming_ask) {
                    return best_bid >= incoming_ask;
                }
            );
            break;
        default:
            // lob::Side::Cancel, invalid state
            return false;
    }

    return true;
}


// Assumes ID is a valid order that has not been filled.
bool OrderBook::cancel_order(OrderId id) {
    if (!order_location.contains(id)) {
        return false;
    }

    OrderHandle& order = order_location.at(id);
    switch (order.side) {
        case Side::Buy:
            bids.at(order.price).erase(order.order_it);
            if (bids.at(order.price).empty()) {
                bids.erase(order.price);
            }
            break;

        case Side::Sell:
            asks.at(order.price).erase(order.order_it);
            if (asks.at(order.price).empty()) {
                asks.erase(order.price);
            }
            break;

        default:
            // lob::Side::Cancel, invalid state
            return false;
    }

    order_location.erase(id);

    return true;
}

} // namespace lob
