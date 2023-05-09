// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_render_pass.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, render_pass_parameters params)
    -> std::expected<render_pass, error> {
    // Initialize render pass creation info.
    auto info = VkRenderPassCreateInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount =
            static_cast<uint32_t>(params.attachment_descriptions.size()),
        .pAttachments = params.attachment_descriptions.data(),
        .subpassCount =
            static_cast<uint32_t>(params.subpass_descriptions.size()),
        .pSubpasses = params.subpass_descriptions.data(),
        .dependencyCount =
            static_cast<uint32_t>(params.subpass_dependencies.size()),
        .pDependencies = params.subpass_dependencies.data()};

    // Initialize a null handle.
    auto handle = VkRenderPass{};

    // Try creating a new render pass.
    if(auto code = vkCreateRenderPass(device, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created render pass.
    return render_pass{device, handle};
}

} // namespace sgc::vk
