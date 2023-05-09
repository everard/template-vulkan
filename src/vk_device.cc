// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_device.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(physical_device parent, device_parameters params)
    -> std::expected<device, error> {
    // Initialization fails if no physical device is specified.
    if(parent.handle == nullptr) {
        return std::unexpected{error{__LINE__, 0}};
    }

    // Initialize an empty result.
    auto result = device{.parent = parent};

    // Select queue family indices.
    if(auto n = uint32_t{}; true) {
        // Obtain the number of queue families.
        vkGetPhysicalDeviceQueueFamilyProperties(parent, &n, nullptr);

        // Obtain properties of the queue families.
        auto queue_family_properties =
            std::vector<VkQueueFamilyProperties>{n, VkQueueFamilyProperties{}};

        vkGetPhysicalDeviceQueueFamilyProperties(
            parent, &n, queue_family_properties.data());

        // Select a compute queue family index.
        for(auto properties : queue_family_properties) {
            if(properties.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                break;
            } else {
                result.queues.family_indices.compute++;
            }
        }

        if(n == result.queues.family_indices.compute) {
            return std::unexpected{error{__LINE__, 0}};
        }

        // Select a graphics queue family index.
        for(auto properties : queue_family_properties) {
            if(properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                break;
            } else {
                result.queues.family_indices.graphics++;
            }
        }

        if(n == result.queues.family_indices.graphics) {
            return std::unexpected{error{__LINE__, 0}};
        }

        // Select a presentation queue family index.
        if(auto i = uint32_t{}; params.surface != nullptr) {
            while(i != queue_family_properties.size()) {
                auto is_supported = VkBool32{};
                if((vkGetPhysicalDeviceSurfaceSupportKHR(
                        parent, i, params.surface, &is_supported) ==
                    VK_SUCCESS) &&
                   is_supported) {
                    break;
                } else {
                    i++;
                }
            }

            result.queues.family_indices.presentation = i;
        } else {
            result.queues.family_indices.presentation =
                result.queues.family_indices.graphics;
        }

        if(n == result.queues.family_indices.presentation) {
            return std::unexpected{error{__LINE__, 0}};
        }
    }

    // Create a new device.
    if(true) {
        // Pack queue family indices in an array.
        uint32_t queue_family_indices[] = //
            {result.queues.family_indices.compute,
             result.queues.family_indices.graphics,
             result.queues.family_indices.presentation};

        // Sort the array.
        std::ranges::sort(queue_family_indices);

        // Initialize an array of device queue creation info structures.
        auto priority = 1.0f;
        VkDeviceQueueCreateInfo device_queue_create_infos[] = {
            {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
             .queueCount = 1,
             .pQueuePriorities = &priority},
            {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
             .queueCount = 1,
             .pQueuePriorities = &priority},
            {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
             .queueCount = 1,
             .pQueuePriorities = &priority}};

        auto n_queue_families = uint32_t{};
        if(auto idx_prev = uint32_t{0xFFFFFFFF}; true) {
            for(auto family_idx : queue_family_indices) {
                if(idx_prev == family_idx) {
                    continue;
                }

                device_queue_create_infos[n_queue_families++].queueFamilyIndex =
                    idx_prev = family_idx;
            }
        }

        // Initialize device creation info.
        auto info = VkDeviceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = &(params.features),
            .queueCreateInfoCount = n_queue_families,
            .pQueueCreateInfos = device_queue_create_infos,
            .enabledExtensionCount =
                static_cast<uint32_t>(params.extensions.size()),
            .ppEnabledExtensionNames = params.extensions.data()};

        // Try creating a new device.
        if(auto code = vkCreateDevice(parent, &info, nullptr, &(result.handle));
           code != VK_SUCCESS) {
            // Initialization failed: return error description.
            return std::unexpected{error{__LINE__, code}};
        }
    }

#define obtain_queue_(type) \
    vkGetDeviceQueue(       \
        result, result.queues.family_indices.type, 0, &(result.queues.type))

    // Obtain queues.
    obtain_queue_(compute);
    obtain_queue_(graphics);
    obtain_queue_(presentation);

    // Return created device.
    return result;
}

} // namespace sgc::vk
