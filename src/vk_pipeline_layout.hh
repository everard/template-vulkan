// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_1F73CB38FC37458D8A09725118196CBF
#define H_1F73CB38FC37458D8A09725118196CBF

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan pipeline layout definition.
////////////////////////////////////////////////////////////////////////////////

struct pipeline_layout : resource<VkPipelineLayout, vkDestroyPipelineLayout> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan pipeline layout initialization parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct pipeline_layout_parameters {
    VkPipelineLayoutCreateFlags flags;
    std::span<VkDescriptorSetLayout const> descriptor_set_layouts;
    std::span<VkPushConstantRange const> push_constant_ranges;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, pipeline_layout_parameters params)
    -> std::expected<pipeline_layout, error>;

} // namespace sgc::vk

#endif // H_1F73CB38FC37458D8A09725118196CBF
