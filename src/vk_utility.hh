// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_0A2ACB5CD1DD4FA8A75F12B49B856B83
#define H_0A2ACB5CD1DD4FA8A75F12B49B856B83

#include <algorithm>
#include <concepts>
#include <ranges>

#include <expected>
#include <optional>

#include <stddef.h>
#include <stdint.h>

#include <type_traits>
#include <utility>

#include <array>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Zero-terminated byte string definition.
////////////////////////////////////////////////////////////////////////////////

using utf8_ntbs = char const*;
using utf8_ntbs_span = std::span<utf8_ntbs const>;

////////////////////////////////////////////////////////////////////////////////
// Error definition.
////////////////////////////////////////////////////////////////////////////////

struct error {
    long long line, code;
};

} // namespace sgc::vk

#endif // H_0A2ACB5CD1DD4FA8A75F12B49B856B83
