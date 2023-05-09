// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_9F46DEF13F054E26AB5BC25C3A261EAD
#define H_9F46DEF13F054E26AB5BC25C3A261EAD

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan image definition.
////////////////////////////////////////////////////////////////////////////////

struct image : resource<VkImage, vkDestroyImage> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, VkImageCreateInfo info)
    -> std::expected<image, error>;

} // namespace sgc::vk

#endif // H_9F46DEF13F054E26AB5BC25C3A261EAD
