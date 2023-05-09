// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_instance.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(instance_parameters params) -> std::expected<instance, error> {
    // Initialize instance creation info.
    auto info_application =
        VkApplicationInfo{.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                          .apiVersion = params.api_version};

    auto info = VkInstanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &info_application,
        .enabledLayerCount = static_cast<uint32_t>(params.layers.size()),
        .ppEnabledLayerNames = params.layers.data(),
        .enabledExtensionCount =
            static_cast<uint32_t>(params.extensions.size()),
        .ppEnabledExtensionNames = params.extensions.data()};

    // Initialize a null handle.
    auto handle = VkInstance{};

    // Try creating a new instance.
    if(auto code = vkCreateInstance(&info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created instance.
    return instance{handle};
}

} // namespace sgc::vk
