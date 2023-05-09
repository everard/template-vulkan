// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_image.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, VkImageCreateInfo info)
    -> std::expected<image, error> {
    // Complete image creation info.
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;

    // Initialize a null handle.
    auto handle = VkImage{};

    // Try creating a new image.
    if(auto code = vkCreateImage(device, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created image.
    return image{device, handle};
}

} // namespace sgc::vk
