// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_9E74F869D14943B086FC2E2119402B1B
#define H_9E74F869D14943B086FC2E2119402B1B

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan pipeline definition.
////////////////////////////////////////////////////////////////////////////////

struct pipeline : resource<VkPipeline, vkDestroyPipeline> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, VkComputePipelineCreateInfo info)
    -> std::expected<pipeline, error>;

auto
initialize(VkDevice device, VkGraphicsPipelineCreateInfo info)
    -> std::expected<pipeline, error>;

} // namespace sgc::vk

#endif // H_9E74F869D14943B086FC2E2119402B1B
