// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_physical_device.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Selection interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
select(VkInstance instance, physical_device_preference preference)
    -> std::expected<physical_device, error> {
#define try_(expression)                               \
    if(auto code = expression; code != VK_SUCCESS) {   \
        return std::unexpected{error{__LINE__, code}}; \
    }

    // Obtain a list of physical devices.
    auto devices = std::vector<VkPhysicalDevice>{};
    try {
        if(auto n = uint32_t{}; true) {
            try_(vkEnumeratePhysicalDevices(instance, &n, nullptr));
            devices.resize(n);
            try_(vkEnumeratePhysicalDevices(instance, &n, devices.data()));
        }
    } catch(...) {
        return std::unexpected{error{__LINE__, 0}};
    }

#undef try_

    // Find a physical device which satisfies the requirements.
    for(auto device : devices) {
        // Obtain device properties.
        auto physical_device_properties = VkPhysicalDeviceProperties{};
        vkGetPhysicalDeviceProperties(device, &physical_device_properties);

        // Check supported Vulkan API version.
        if(preference.api_version != 0) {
            if(VK_VERSION_MAJOR(physical_device_properties.apiVersion) <
               VK_VERSION_MAJOR(preference.api_version)) {
                continue;
            }

            if(VK_VERSION_MINOR(physical_device_properties.apiVersion) <
               VK_VERSION_MINOR(preference.api_version)) {
                continue;
            }
        }

        // Check device name.
        if(preference.name != nullptr) {
            if(std::string_view{physical_device_properties.deviceName} !=
               std::string_view{preference.name}) {
                continue;
            }
        }

        // Check device type.
        if(preference.type != 0) {
            if(physical_device_properties.deviceType != preference.type) {
                continue;
            }
        }

        // A suitable device has been found.
        if(auto result = physical_device{.handle = device}; true) {
            // Obtain its memory properties.
            vkGetPhysicalDeviceMemoryProperties(
                result, &(result.memory_properties));

            // Return selected physical device.
            return result;
        }
    }

    // Suitable physical device could not be found.
    return std::unexpected{error{__LINE__, 0}};
}

} // namespace sgc::vk
