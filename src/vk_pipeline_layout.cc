// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_pipeline_layout.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, pipeline_layout_parameters params)
    -> std::expected<pipeline_layout, error> {
    // Initialize pipeline layout creation info.
    auto info = VkPipelineLayoutCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .flags = params.flags,
        .setLayoutCount =
            static_cast<uint32_t>(params.descriptor_set_layouts.size()),
        .pSetLayouts = params.descriptor_set_layouts.data(),
        .pushConstantRangeCount =
            static_cast<uint32_t>(params.push_constant_ranges.size()),
        .pPushConstantRanges = params.push_constant_ranges.data()};

    // Initialize a null handle.
    auto handle = VkPipelineLayout{};

    // Try creating a new pipeline layout.
    if(auto code = vkCreatePipelineLayout(device, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created pipeline layout.
    return pipeline_layout{device, handle};
}

} // namespace sgc::vk
