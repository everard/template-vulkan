// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_swapchain.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(swapchain swapchain_prev, swapchain_parameters params)
    -> std::expected<swapchain, error> {
    // Initialization fails if no surface is specified.
    if(params.surface == nullptr) {
        return std::unexpected{error{__LINE__, 0}};
    }

    // Initialize swapchain creation info.
    auto info = VkSwapchainCreateInfoKHR{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = params.surface,
        .minImageCount = params.n_images,
        .imageFormat = params.surface_format.format,
        .imageColorSpace = params.surface_format.colorSpace,
        .imageExtent = params.image_extent,
        .imageArrayLayers = params.n_image_array_layers,
        .imageUsage = params.image_usage_flags,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = params.surface_transform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = params.present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = swapchain_prev.handle};

    // Initialize a null handle.
    auto handle = VkSwapchainKHR{};

    // Try creating a new swapchain.
    if(auto code =
           vkCreateSwapchainKHR(swapchain_prev.parent, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created swapchain.
    return swapchain{swapchain_prev.parent, handle};
}

auto
initialize(VkDevice device, swapchain_parameters params)
    -> std::expected<swapchain, error> {
    // Delegate to the main initialization function.
    return initialize(swapchain{device, nullptr}, params);
}

////////////////////////////////////////////////////////////////////////////////
// Query interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
obtain_images(swapchain const& swapchain)
    -> std::expected<std::vector<VkImage>, error> {
    // Images can be obtained only from a valid swapchain.
    if(swapchain.handle == nullptr) {
        return std::unexpected{error{__LINE__, 0}};
    }

    // Initialize an empty result.
    auto result = std::vector<VkImage>{};

#define try_(expression)                               \
    if(auto code = expression; code != VK_SUCCESS) {   \
        return std::unexpected{error{__LINE__, code}}; \
    }

    // Obtain the list of swapchain images.
    if(auto n = uint32_t{}; true) {
        try_(vkGetSwapchainImagesKHR(
            swapchain.parent, swapchain.handle, &n, nullptr));

        try {
            result.resize(n);
        } catch(...) {
            return std::unexpected{error{__LINE__, 0}};
        }

        try_(vkGetSwapchainImagesKHR(
            swapchain.parent, swapchain.handle, &n, result.data()));
    }

    // Return the list.
    return result;
}

} // namespace sgc::vk
