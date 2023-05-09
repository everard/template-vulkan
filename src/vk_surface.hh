// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_93AF94BA712041BD9E08A331D832F809
#define H_93AF94BA712041BD9E08A331D832F809

#include "vk_physical_device.hh"

#include <SDL.h>
#include <SDL_vulkan.h>

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan surface definition.
////////////////////////////////////////////////////////////////////////////////

struct surface
    : generic::resource<VkInstance, VkSurfaceKHR, vkDestroySurfaceKHR> {
    using generic::resource< //
        VkInstance, VkSurfaceKHR, vkDestroySurfaceKHR>::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan surface initialization parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct surface_parameters {
    SDL_Window* window;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan surface properties definition.
////////////////////////////////////////////////////////////////////////////////

struct surface_properties {
    // Surface capabilities.
    VkSurfaceCapabilitiesKHR capabilities;

    // Supported formats and modes.
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> modes;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkInstance instance, surface_parameters params)
    -> std::expected<surface, error>;

////////////////////////////////////////////////////////////////////////////////
// Query interface.
////////////////////////////////////////////////////////////////////////////////

auto
obtain_properties(surface const& surface, VkPhysicalDevice device)
    -> std::expected<surface_properties, error>;

} // namespace sgc::vk

#endif // H_93AF94BA712041BD9E08A331D832F809
