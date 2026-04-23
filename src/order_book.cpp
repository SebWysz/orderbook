#include "lob/order_book.hpp"
#include "lob/event.hpp"

namespace lob {

void OrderBook::apply(const Event& event) {
    switch (event.type) {
        case lob::EventType::Cancel:
            cancel_order(event.order_id);
            break;
        case lob::EventType::Add:
            add_order(event);
            break;
        default:
            // TODO: Error Handling?
            break;
    }
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
    }

    return true;
}


// Assumes ID is a valid order that has not been filled.
bool OrderBook::cancel_order(OrderId id) {
    OrderHandle& order = order_location[id];
    switch (order.side) {
        case Side::Buy:
            bids[order.price].erase(order.order_it);
            break;

        case Side::Sell:
            asks[order.price].erase(order.order_it);
            break;
    }

    return true;
}

} // namespace lob
