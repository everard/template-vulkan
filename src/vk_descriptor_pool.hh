// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_E5A1753EA2D84ACA835A5826F012F0E7
#define H_E5A1753EA2D84ACA835A5826F012F0E7

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan descriptor pool definition.
////////////////////////////////////////////////////////////////////////////////

struct descriptor_pool : resource<VkDescriptorPool, vkDestroyDescriptorPool> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan descriptor pool initialization parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct descriptor_pool_parameters {
    VkDescriptorPoolCreateFlags flags;
    uint32_t max_sets;

    std::span<VkDescriptorPoolSize const> descriptor_pool_sizes;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan descriptor set allocation parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct descriptor_set_allocation_parameters {
    std::span<VkDescriptorSetLayout> descriptor_set_layouts;
    std::span<uint32_t> variable_descriptor_counts;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, descriptor_pool_parameters params)
    -> std::expected<descriptor_pool, error>;

////////////////////////////////////////////////////////////////////////////////
// Allocation interface.
////////////////////////////////////////////////////////////////////////////////

auto
allocate(descriptor_pool const& pool,
         descriptor_set_allocation_parameters params)
    -> std::vector<VkDescriptorSet>;

void
reset(descriptor_pool const& pool);

} // namespace sgc::vk

#endif // H_E5A1753EA2D84ACA835A5826F012F0E7
