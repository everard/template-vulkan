// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_F6D7DCA3932A4E04B338DE8BF4EBD4F1
#define H_F6D7DCA3932A4E04B338DE8BF4EBD4F1

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan framebuffer definition.
////////////////////////////////////////////////////////////////////////////////

struct framebuffer : resource<VkFramebuffer, vkDestroyFramebuffer> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan framebuffer initialization parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct framebuffer_parameters {
    VkRenderPass render_pass;
    std::span<VkImageView const> attachments;

    uint32_t width;
    uint32_t height;
    uint32_t layers;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, framebuffer_parameters params)
    -> std::expected<framebuffer, error>;

} // namespace sgc::vk

#endif // H_F6D7DCA3932A4E04B338DE8BF4EBD4F1
