// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_2517D99CCB2743B7BE70BE31D1A9A447
#define H_2517D99CCB2743B7BE70BE31D1A9A447

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan render pass definition.
////////////////////////////////////////////////////////////////////////////////

struct render_pass : resource<VkRenderPass, vkDestroyRenderPass> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan render pass initialization parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct render_pass_parameters {
    std::span<VkAttachmentDescription const> attachment_descriptions;
    std::span<VkSubpassDescription const> subpass_descriptions;
    std::span<VkSubpassDependency const> subpass_dependencies;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, render_pass_parameters params)
    -> std::expected<render_pass, error>;

} // namespace sgc::vk

#endif // H_2517D99CCB2743B7BE70BE31D1A9A447
