// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_C7861FA1C38249FF9A98B43F0E8E9FD7
#define H_C7861FA1C38249FF9A98B43F0E8E9FD7

#include "vk_physical_device.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan device queue family index definition.
////////////////////////////////////////////////////////////////////////////////

struct queue_family_index {
    uint32_t compute, graphics, presentation;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan device definition.
////////////////////////////////////////////////////////////////////////////////

struct device : object<VkDevice, vkDestroyDevice> {
    // Parent physical device.
    physical_device parent;

    // Queues.
    struct {
        // Handles.
        VkQueue compute, graphics, presentation;

        // Queue family indices.
        queue_family_index family_indices;
    } queues;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan device initialization parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct device_parameters {
    physical_device_features features;
    utf8_ntbs_span extensions;

    VkSurfaceKHR surface;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(physical_device parent, device_parameters params)
    -> std::expected<device, error>;

} // namespace sgc::vk

#endif // H_C7861FA1C38249FF9A98B43F0E8E9FD7
