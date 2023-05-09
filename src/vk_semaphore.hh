// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_68DC268E92C64025AB7D88BC6686487F
#define H_68DC268E92C64025AB7D88BC6686487F

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan semaphore definition.
////////////////////////////////////////////////////////////////////////////////

struct semaphore : resource<VkSemaphore, vkDestroySemaphore> {
    using resource::resource;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan semaphore initialization parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct semaphore_parameters {
    // Empty.
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, semaphore_parameters params)
    -> std::expected<semaphore, error>;

} // namespace sgc::vk

#endif // H_68DC268E92C64025AB7D88BC6686487F
