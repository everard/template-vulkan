// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_pipeline.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, VkComputePipelineCreateInfo info)
    -> std::expected<pipeline, error> {
    // Complete pipeline creation info.
    info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    info.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

    // Initialize a null handle.
    auto handle = VkPipeline{};

    // Try creating a new pipeline.
    if(auto code = vkCreateComputePipelines(
           device, nullptr, 1, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created pipeline.
    return pipeline{device, handle};
}

auto
initialize(VkDevice device, VkGraphicsPipelineCreateInfo info)
    -> std::expected<pipeline, error> {
    // Complete pipeline creation info.
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

    // Initialize a null handle.
    auto handle = VkPipeline{};

    // Try creating a new pipeline.
    if(auto code = vkCreateGraphicsPipelines( //
           device, nullptr, 1, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created pipeline.
    return pipeline{device, handle};
}

} // namespace sgc::vk
