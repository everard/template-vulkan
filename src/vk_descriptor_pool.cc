// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_descriptor_pool.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, descriptor_pool_parameters params)
    -> std::expected<descriptor_pool, error> {
    // Initialize descriptor pool creation info.
    auto info = VkDescriptorPoolCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .flags = params.flags,
        .maxSets = params.max_sets,
        .poolSizeCount =
            static_cast<uint32_t>(params.descriptor_pool_sizes.size()),
        .pPoolSizes = params.descriptor_pool_sizes.data()};

    // Initialize a null handle.
    auto handle = VkDescriptorPool{};

    // Try creating a new descriptor pool.
    if(auto code = vkCreateDescriptorPool(device, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created descriptor pool.
    return descriptor_pool{device, handle};
}

////////////////////////////////////////////////////////////////////////////////
// Allocation interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
allocate(descriptor_pool const& pool,
         descriptor_set_allocation_parameters params)
    -> std::vector<VkDescriptorSet> {
    // Do nothing if allocation size is zero.
    if(params.descriptor_set_layouts.empty()) {
        return {};
    }

    // Allocation fails if the given parameters are invalid.
    if((params.variable_descriptor_counts.size() != 0) &&
       (params.variable_descriptor_counts.size() !=
        params.descriptor_set_layouts.size())) {
        return {};
    }

    // Initialize a container which will hold allocated descriptor sets.
    auto result = std::vector<VkDescriptorSet>{
        params.descriptor_set_layouts.size(), nullptr};

#define structure_type_ \
    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO

    // Initialize a chain of allocation info structures.
    auto info_next = VkDescriptorSetVariableDescriptorCountAllocateInfo{
        .sType = structure_type_,
        .descriptorSetCount =
            static_cast<uint32_t>(params.variable_descriptor_counts.size()),
        .pDescriptorCounts = params.variable_descriptor_counts.data()};

    auto info = VkDescriptorSetAllocateInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = &info_next,
        .descriptorPool = pool.handle,
        .descriptorSetCount =
            static_cast<uint32_t>(params.descriptor_set_layouts.size()),
        .pSetLayouts = params.descriptor_set_layouts.data()};

#undef structure_type_

    // Try allocating descriptor sets.
    if(vkAllocateDescriptorSets(pool.parent, &info, result.data()) !=
       VK_SUCCESS) {
        result.clear();
    }

    return result;
}

void
reset(descriptor_pool const& pool) {
    // Note: Even though this Vulkan function returns a result, it always
    // succeeds.
    vkResetDescriptorPool(pool.parent, pool.handle, 0);
}

} // namespace sgc::vk
