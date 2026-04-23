#pragma once

#include "lob/types.hpp"

namespace lob {

enum class EventType {
    Add,
    Cancel
};

struct Event {
    EventType type {};
    OrderId order_id {};
    Side side {};
    Price price {};
    Qty qty {};
    Timestamp ts {};
};

} // namespace lob
