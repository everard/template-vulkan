// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_5D6FA26963E24512AA26B0297D2B1CD6
#define H_5D6FA26963E24512AA26B0297D2B1CD6

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan descriptor set layout definition.
////////////////////////////////////////////////////////////////////////////////

struct descriptor_set_layout
    : resource<VkDescriptorSetLayout, vkDestroyDescriptorSetLayout> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan descriptor set layout initialization parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct descriptor_set_layout_parameters {
    VkDescriptorSetLayoutCreateFlags flags;
    std::span<VkDescriptorSetLayoutBinding const> bindings;
    std::span<VkDescriptorBindingFlags const> binding_flags;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, descriptor_set_layout_parameters params)
    -> std::expected<descriptor_set_layout, error>;

} // namespace sgc::vk

#endif // H_5D6FA26963E24512AA26B0297D2B1CD6
