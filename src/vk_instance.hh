// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_BA9BB9AD5200405E9A1AF796F7E9C813
#define H_BA9BB9AD5200405E9A1AF796F7E9C813

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan instance definition.
////////////////////////////////////////////////////////////////////////////////

struct instance : object<VkInstance, vkDestroyInstance> {
    using object::object;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan instance initialization parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct instance_parameters {
    uint32_t api_version;
    utf8_ntbs_span extensions;
    utf8_ntbs_span layers;
};

////////////////////////////////////////////////////////////////////////////////
// Initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(instance_parameters params) -> std::expected<instance, error>;

} // namespace sgc::vk

#endif // H_BA9BB9AD5200405E9A1AF796F7E9C813
