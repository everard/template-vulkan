// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_command_pool.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, VkCommandPoolCreateInfo info)
    -> std::expected<command_pool, error> {
    // Complete command pool creation info.
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

    // Initialize a null handle.
    auto handle = VkCommandPool{};

    // Try creating a new command pool.
    if(auto code = vkCreateCommandPool(device, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created command pool.
    return command_pool{device, handle};
}

////////////////////////////////////////////////////////////////////////////////
// Allocation interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
allocate(command_pool const& pool, VkCommandBufferAllocateInfo info)
    -> std::vector<VkCommandBuffer> {
    // Complete command buffer allocation info.
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool = pool;

    // Initialize an empty result.
    auto result = std::vector<VkCommandBuffer>{};
    try {
        result.resize(info.commandBufferCount);
    } catch(...) {
        return {};
    }

    // Try allocating command buffers.
    if(auto code = vkAllocateCommandBuffers(pool.parent, &info, result.data());
       code != VK_SUCCESS) {
        return {};
    }

    // Return allocated command buffers.
    return result;
}

auto
reset(command_pool const& pool, VkCommandPoolResetFlags flags)
    -> std::expected<void, error> {
    if(auto code = vkResetCommandPool(pool.parent, pool, flags);
       code != VK_SUCCESS) {
        // Operation failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    return {};
}

} // namespace sgc::vk
