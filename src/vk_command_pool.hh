// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_2B5A0A27EA1B4B77AC443CBDC9D9D1FC
#define H_2B5A0A27EA1B4B77AC443CBDC9D9D1FC

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan command pool definition.
////////////////////////////////////////////////////////////////////////////////

struct command_pool : resource<VkCommandPool, vkDestroyCommandPool> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, VkCommandPoolCreateInfo info)
    -> std::expected<command_pool, error>;

////////////////////////////////////////////////////////////////////////////////
// Allocation interface.
////////////////////////////////////////////////////////////////////////////////

auto
allocate(command_pool const& pool, VkCommandBufferAllocateInfo info)
    -> std::vector<VkCommandBuffer>;

auto
reset(command_pool const& pool, VkCommandPoolResetFlags flags = 0)
    -> std::expected<void, error>;

} // namespace sgc::vk

#endif // H_2B5A0A27EA1B4B77AC443CBDC9D9D1FC
