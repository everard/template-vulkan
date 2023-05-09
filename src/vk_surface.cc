// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_surface.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkInstance instance, surface_parameters params)
    -> std::expected<surface, error> {
    // Initialization fails if no window is specified.
    if(params.window == nullptr) {
        return std::unexpected{error{__LINE__, 0}};
    }

    // Initialize a null handle.
    auto handle = VkSurfaceKHR{};

    // Try creating a new surface.
    if(!SDL_Vulkan_CreateSurface(params.window, instance, &handle)) {
        return std::unexpected{error{__LINE__, 0}};
    }

    // Return created surface.
    return surface{instance, handle};
}

////////////////////////////////////////////////////////////////////////////////
// Query interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
obtain_properties(surface const& surface, VkPhysicalDevice device)
    -> std::expected<surface_properties, error> {
    // Properties can be obtained only from a valid surface and physical device.
    if((surface.handle == nullptr) || (device == nullptr)) {
        return std::unexpected{error{__LINE__, 0}};
    }

    // Initialize an empty result.
    auto result = surface_properties{};

#define try_(expression)                               \
    if(auto code = expression; code != VK_SUCCESS) {   \
        return std::unexpected{error{__LINE__, code}}; \
    }

    // Obtain surface capabilities.
    try_(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        device, surface, &result.capabilities));

    // Obtain the list of supported formats.
    if(auto n = uint32_t{}; true) {
        try_(
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &n, nullptr));

        try {
            result.formats.resize(n);
        } catch(...) {
            return std::unexpected{error{__LINE__, 0}};
        }

        try_(vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, surface, &n, result.formats.data()));
    }

    // Obtain the list of supported presentation modes.
    if(auto n = uint32_t{}; true) {
        try_(vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, surface, &n, nullptr));

        try {
            result.modes.resize(n);
        } catch(...) {
            return std::unexpected{error{__LINE__, 0}};
        }

        try_(vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, surface, &n, result.modes.data()));
    }

#undef try_

    // Return surface properties.
    return result;
}

} // namespace sgc::vk
