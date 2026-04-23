#include "lob/order_book.hpp"
#include "lob/event.hpp"

namespace lob {

void OrderBook::apply(const Event& event) {
    switch (event.type) {
        case lob::EventType::Cancel:
            cancel_order(event.order_id);
            break;
        case lob::EventType::Add:
            // TODO:
            break;
        default:
            // TODO: Error Handling?
            break;
    }
}

int OrderBook::order_count() const noexcept {
    // TODO:
    uint64_t count{0};
    for (const auto& [_, list] : bids) {
        // TODO:
    }
    return 0;
}

} // namespace lob
