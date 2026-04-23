#pragma once

#include "lob/event.hpp"

#include <map>
#include <list>
#include <unordered_map>

namespace lob {

struct Order {
    OrderId order_id;
    Price price;
    Qty qty;
};

struct BidCompare {
    bool operator()(const Price& a, const Price& b) const {
        return a > b;
    }
};

struct AskCompare {
    bool operator()(const Price& a, const Price& b) const {
        return a < b;
    }
};

struct OrderHandle {
    Side side;
    Price price;
    std::list<Order>::iterator order_it; // Since list iterators don't become invalidated
};

class OrderBook {
public:

    void apply(const Event& event);

    [[nodiscard]] int order_count() const noexcept;

private:

    std::map<Price, std::list<Order>, BidCompare> bids;
    std::map<Price, std::list<Order>, AskCompare> asks;

    std::unordered_map<OrderId, OrderHandle> order_location;

    bool cancel_order(OrderId id);
    bool add_order(const Event& event);
};

} // namespace lob
