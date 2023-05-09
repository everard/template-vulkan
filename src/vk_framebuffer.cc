// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_framebuffer.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, framebuffer_parameters params)
    -> std::expected<framebuffer, error> {
    // Initialize framebuffer creation info.
    auto info = VkFramebufferCreateInfo{
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = params.render_pass,
        .attachmentCount = static_cast<uint32_t>(params.attachments.size()),
        .pAttachments = params.attachments.data(),
        .width = params.width,
        .height = params.height,
        .layers = params.layers};

    // Initialize a null handle.
    auto handle = VkFramebuffer{};

    // Try creating a new framebuffer.
    if(auto code = vkCreateFramebuffer(device, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created framebuffer.
    return framebuffer{device, handle};
}

} // namespace sgc::vk
