// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_semaphore.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, [[maybe_unused]] semaphore_parameters params)
    -> std::expected<semaphore, error> {
    // Initialize semaphore creation info.
    auto info =
        VkSemaphoreCreateInfo{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

    // Initialize a null handle.
    auto handle = VkSemaphore{};

    // Try creating a new semaphore.
    if(auto code = vkCreateSemaphore(device, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created semaphore.
    return semaphore{device, handle};
}

} // namespace sgc::vk
