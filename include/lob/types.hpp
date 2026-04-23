#pragma once

#include <cstdint>

namespace lob {

using OrderId = std::uint64_t;
using Price = std::int64_t;
using Qty = std::int64_t;
using Timestamp = std::uint64_t;

enum class Side {
    Buy,
    Sell
};

} // namespace lob
