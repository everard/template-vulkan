// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_descriptor_set_layout.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, descriptor_set_layout_parameters params)
    -> std::expected<descriptor_set_layout, error> {
    // Initialization fails if the number of binding flags does not match the
    // number of bindings.
    if((params.binding_flags.size() != 0) &&
       (params.binding_flags.size() != params.bindings.size())) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, 0}};
    }

#define structure_type_(x) VK_STRUCTURE_TYPE_##x##_CREATE_INFO

    // Initialize a chain of descriptor set layout creation info structures.
    auto info_next = VkDescriptorSetLayoutBindingFlagsCreateInfo{
        .sType = structure_type_(DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS),
        .bindingCount = static_cast<uint32_t>(params.binding_flags.size()),
        .pBindingFlags = params.binding_flags.data()};

    auto info = VkDescriptorSetLayoutCreateInfo{
        .sType = structure_type_(DESCRIPTOR_SET_LAYOUT),
        .pNext = &info_next,
        .flags = params.flags,
        .bindingCount = static_cast<uint32_t>(params.bindings.size()),
        .pBindings = params.bindings.data()};

#undef structure_type_

    // Initialize a null handle.
    auto handle = VkDescriptorSetLayout{};

    // Try creating a new descriptor set layout.
    if(auto code = vkCreateDescriptorSetLayout(device, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created descriptor set layout.
    return descriptor_set_layout{device, handle};
}

} // namespace sgc::vk
