// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_339CC10A54A541A1AEAFC10355B1E874
#define H_339CC10A54A541A1AEAFC10355B1E874

#include "vk_surface.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan swapchain definition.
////////////////////////////////////////////////////////////////////////////////

struct swapchain : resource<VkSwapchainKHR, vkDestroySwapchainKHR> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan swapchain initialization parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct swapchain_parameters {
    // Surface description.
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surface_format;
    VkSurfaceTransformFlagBitsKHR surface_transform;

    // Presentation mode.
    VkPresentModeKHR present_mode;

    // Extent and usage flags of swapchain images.
    VkExtent2D image_extent;
    VkImageUsageFlags image_usage_flags;

    // Number of images in swapchain, and number of views in multiview surface.
    uint32_t n_images;
    uint32_t n_image_array_layers;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(swapchain swapchain_prev, swapchain_parameters params)
    -> std::expected<swapchain, error>;

auto
initialize(VkDevice device, swapchain_parameters params)
    -> std::expected<swapchain, error>;

////////////////////////////////////////////////////////////////////////////////
// Query interface.
////////////////////////////////////////////////////////////////////////////////

auto
obtain_images(swapchain const& swapchain)
    -> std::expected<std::vector<VkImage>, error>;

} // namespace sgc::vk

#endif // H_339CC10A54A541A1AEAFC10355B1E874
