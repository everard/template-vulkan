// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_1D97B10FD87645388D66594B5FD384E3
#define H_1D97B10FD87645388D66594B5FD384E3

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan image view definition.
////////////////////////////////////////////////////////////////////////////////

struct image_view : resource<VkImageView, vkDestroyImageView> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, VkImageViewCreateInfo info)
    -> std::expected<image_view, error>;

} // namespace sgc::vk

#endif // H_1D97B10FD87645388D66594B5FD384E3
