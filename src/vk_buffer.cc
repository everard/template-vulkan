// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_buffer.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, VkBufferCreateInfo info)
    -> std::expected<buffer, error> {
    // Complete buffer creation info.
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    // Initialize a null handle.
    auto handle = VkBuffer{};

    // Try creating a new buffer.
    if(auto code = vkCreateBuffer(device, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created buffer.
    return buffer{device, handle};
}

} // namespace sgc::vk
