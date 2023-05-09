// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_image_view.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, VkImageViewCreateInfo info)
    -> std::expected<image_view, error> {
    // Complete image view creation info.
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

    // Initialize a null handle.
    auto handle = VkImageView{};

    // Try creating a new image view.
    if(auto code = vkCreateImageView(device, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created image view.
    return image_view{device, handle};
}

} // namespace sgc::vk
