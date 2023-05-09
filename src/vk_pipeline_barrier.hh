// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_A86904C38F714285823C3945FA813960
#define H_A86904C38F714285823C3945FA813960

#include "vk_resource.hh"
#include "vk_utility.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan pipeline barrier definition.
////////////////////////////////////////////////////////////////////////////////

struct pipeline_barrier {
    VkPipelineStageFlags src_stage_mask;
    VkPipelineStageFlags dst_stage_mask;
    VkDependencyFlags dependency_flags;

    std::span<VkMemoryBarrier const> memory_barriers;
    std::span<VkBufferMemoryBarrier const> buffer_memory_barriers;
    std::span<VkImageMemoryBarrier const> image_memory_barriers;
};

////////////////////////////////////////////////////////////////////////////////
// Barrier insertion interface.
////////////////////////////////////////////////////////////////////////////////

void
barrier(VkCommandBuffer command_buffer, pipeline_barrier pipeline_barrier);

} // namespace sgc::vk

#endif // H_A86904C38F714285823C3945FA813960
