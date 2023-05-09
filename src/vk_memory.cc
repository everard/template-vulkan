// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_memory.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Allocation interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
allocate(device const& device, memory_allocation_parameters params)
    -> std::expected<memory, error> {
    // Obtain memory properties.
    auto const& memory_properties = device.parent.memory_properties;

    // Initialize an empty memory object.
    auto result = memory{device};

    // Search for a suitable memory type, then allocate memory from it.
    for(auto i = uint32_t{}; i != memory_properties.memoryTypeCount; ++i) {
        // Skip unsuitable memory types.
        if((params.requirements.memoryTypeBits & (1 << i)) == 0) {
            continue;
        }

        if(params.property_flags !=
           (params.property_flags &
            memory_properties.memoryTypes[i].propertyFlags)) {
            continue;
        }

        // Initialize memory allocation info.
        auto info = VkMemoryAllocateInfo{
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = params.requirements.size,
            .memoryTypeIndex = i};

        // Try allocating memory.
        if(auto code =
               vkAllocateMemory(device, &info, nullptr, &(result.handle));
           code != VK_SUCCESS) {
            return std::unexpected{error{__LINE__, code}};
        }

        // Return allocated memory.
        return result;
    }

    // Allocation failed: no suitable memory type has been found.
    return std::unexpected{error{__LINE__, 0}};
}

////////////////////////////////////////////////////////////////////////////////
// Data transmission interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
read(memory const& memory, memory_read_operation_parameters params)
    -> std::expected<void, error> {
    // Obtain the size of the data.
    auto size = static_cast<VkDeviceSize>(params.bytes.size());
    if(size != params.bytes.size()) {
        return std::unexpected{error{__LINE__, 0}};
    }

    // Map the memory.
    void* src = nullptr;
    if(auto code = vkMapMemory(
           memory.parent, memory.handle, params.offset, size, 0, &src);
       code != VK_SUCCESS) {
        return std::unexpected{error{__LINE__, code}};
    }

    // Make sure the memory is unmapped upon return from this function.
    struct memory_guard {
        ~memory_guard() {
            vkUnmapMemory(memory.parent, memory.handle);
        }

        struct memory const& memory;
    } _{memory};

    // Invalidate the mapped memory range.
    auto range =
        VkMappedMemoryRange{.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
                            .memory = memory.handle,
                            .offset = params.offset,
                            .size = size};

    if(auto code = vkInvalidateMappedMemoryRanges(memory.parent, 1, &range);
       code != VK_SUCCESS) {
        return std::unexpected{error{__LINE__, code}};
    }

    // Read the data.
    std::ranges::copy(std::span{static_cast<std::byte const*>(src), size},
                      std::begin(params.bytes));

    return {};
}

auto
write(memory const& memory, memory_write_operation_parameters params)
    -> std::expected<void, error> {
    // Obtain the size of the data.
    auto size = static_cast<VkDeviceSize>(params.bytes.size());
    if(size != params.bytes.size()) {
        return std::unexpected{error{__LINE__, 0}};
    }

    // Map the memory.
    void* dst = nullptr;
    if(auto code = vkMapMemory(
           memory.parent, memory.handle, params.offset, size, 0, &dst);
       code != VK_SUCCESS) {
        return std::unexpected{error{__LINE__, code}};
    }

    // Write the data.
    std::ranges::copy(params.bytes, static_cast<std::byte*>(dst));

    // Make sure the memory is unmapped upon return from this function.
    struct memory_guard {
        ~memory_guard() {
            vkUnmapMemory(memory.parent, memory.handle);
        }

        struct memory const& memory;
    } _{memory};

    // Flush the written memory range.
    auto range =
        VkMappedMemoryRange{.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
                            .memory = memory.handle,
                            .offset = params.offset,
                            .size = size};

    if(auto code = vkFlushMappedMemoryRanges(memory.parent, 1, &range);
       code != VK_SUCCESS) {
        return std::unexpected{error{__LINE__, code}};
    }

    return {};
}

} // namespace sgc::vk
