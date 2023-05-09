// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_CE9C80D8A94245528EBE12D87E4AB285
#define H_CE9C80D8A94245528EBE12D87E4AB285

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan buffer definition.
////////////////////////////////////////////////////////////////////////////////

struct buffer : resource<VkBuffer, vkDestroyBuffer> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, VkBufferCreateInfo info)
    -> std::expected<buffer, error>;

} // namespace sgc::vk

#endif // H_CE9C80D8A94245528EBE12D87E4AB285
