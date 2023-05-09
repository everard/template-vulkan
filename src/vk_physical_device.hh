// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_95467FDC636D44409A9539603C9AD70E
#define H_95467FDC636D44409A9539603C9AD70E

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan physical device features definition.
////////////////////////////////////////////////////////////////////////////////

struct physical_device_features {
    ////////////////////////////////////////////////////////////////////////////
    // Construction/destruction.
    ////////////////////////////////////////////////////////////////////////////

    physical_device_features(VkPhysicalDevice device) noexcept
        : vulkan_1_1{}, vulkan_1_2{}, vulkan_1_3{}, common{} {
        if(this->link_structures(); device != nullptr) {
            vkGetPhysicalDeviceFeatures2(device, &(this->common));
        }
    }

    physical_device_features(physical_device_features const& other) noexcept
        : vulkan_1_1{other.vulkan_1_1}
        , vulkan_1_2{other.vulkan_1_2}
        , vulkan_1_3{other.vulkan_1_3}
        , common{other.common} {
        this->link_structures();
    }

    physical_device_features(physical_device_features&&) = delete;

    ////////////////////////////////////////////////////////////////////////////
    // Assignment operators.
    ////////////////////////////////////////////////////////////////////////////

    auto
    operator=(physical_device_features const& other) noexcept
        -> physical_device_features& {
        this->vulkan_1_1 = other.vulkan_1_1;
        this->vulkan_1_2 = other.vulkan_1_2;
        this->vulkan_1_3 = other.vulkan_1_3;
        this->common = other.common;
        return (this->link_structures(), *this);
    }

    auto
    operator=(physical_device_features&&) noexcept
        -> physical_device_features& = delete;

    ////////////////////////////////////////////////////////////////////////////
    // Data members.
    ////////////////////////////////////////////////////////////////////////////

    VkPhysicalDeviceVulkan11Features vulkan_1_1;
    VkPhysicalDeviceVulkan12Features vulkan_1_2;
    VkPhysicalDeviceVulkan13Features vulkan_1_3;
    VkPhysicalDeviceFeatures2 common;

private:
    ////////////////////////////////////////////////////////////////////////////
    // Private utility function.
    ////////////////////////////////////////////////////////////////////////////

    void
    link_structures() {
        // Set structure types.
        this->vulkan_1_1.sType =
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;

        this->vulkan_1_2.sType =
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;

        this->vulkan_1_3.sType =
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;

        this->common.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;

        // Link structures.
        this->common.pNext = &(this->vulkan_1_1);
        this->vulkan_1_1.pNext = &(this->vulkan_1_2);
        this->vulkan_1_2.pNext = &(this->vulkan_1_3);
        this->vulkan_1_3.pNext = nullptr;
    }
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan physical device definition.
////////////////////////////////////////////////////////////////////////////////

struct physical_device {
    ////////////////////////////////////////////////////////////////////////////
    // Conversion operator.
    ////////////////////////////////////////////////////////////////////////////

    operator VkPhysicalDevice() const noexcept {
        return this->handle;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Data members.
    ////////////////////////////////////////////////////////////////////////////

    VkPhysicalDevice handle;
    VkPhysicalDeviceMemoryProperties memory_properties;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan physical device preference definition.
////////////////////////////////////////////////////////////////////////////////

struct physical_device_preference {
    uint32_t api_version;
    utf8_ntbs name;
    VkPhysicalDeviceType type;
};

////////////////////////////////////////////////////////////////////////////////
// Selection interface.
////////////////////////////////////////////////////////////////////////////////

auto
select(VkInstance instance, physical_device_preference preference)
    -> std::expected<physical_device, error>;

} // namespace sgc::vk

#endif // H_95467FDC636D44409A9539603C9AD70E
