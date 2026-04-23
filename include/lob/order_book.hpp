#pragma once

#include "lob/event.hpp"

#include <map>
#include <list>
#include <unordered_map>
#include <optional>

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

    [[nodiscard]] uint64_t order_count() const noexcept {
        return order_location.size();
    }

    std::optional<Price> best_bid() const noexcept {
        return bids.empty() ? std::nullopt : std::optional<Price>{bids.begin()->first};
    }

    std::optional<Price> best_ask() const noexcept {
        return asks.empty() ? std::nullopt : std::optional<Price>{asks.begin()->first};
    }

private:

    std::map<Price, std::list<Order>, BidCompare> bids;
    std::map<Price, std::list<Order>, AskCompare> asks;

    std::unordered_map<OrderId, OrderHandle> order_location;

    bool add_order(const Event& event);
    bool cancel_order(OrderId id);

    template <typename MyBook, typename OppBook, typename Crosses>
    void process_add_order(
        MyBook& my_book,
        OppBook& opp_book,
        OrderId order_id,
        Side side,
        Price price,
        Qty qty,
        Crosses crosses
    ) {
        while (!opp_book.empty() && qty > 0 && crosses(opp_book.begin()->first,price)) {
            // fill order immediately
            std::list<Order>& list = opp_book.begin()->second;
            while (!list.empty() && qty > 0) {
                Qty first_qty = list.front().qty;
                if (first_qty <= qty) {
                    list.pop_front();
                }
                qty -= std::min(first_qty, qty);
            }
        }
        if (qty > 0) {
            my_book[price].emplace_back(order_id, price, qty);
            std::list<Order>::iterator iter_to_item = std::prev(my_book[price].end());
            order_location.emplace(order_id, OrderHandle{side, price, iter_to_item});
        }

    }
};

} // namespace lob
