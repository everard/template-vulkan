// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_pipeline_barrier.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Barrier insertion interface implementation.
////////////////////////////////////////////////////////////////////////////////

void
barrier(VkCommandBuffer command_buffer, pipeline_barrier pipeline_barrier) {
    vkCmdPipelineBarrier(
        command_buffer,
        //
        // Stage masks, dependency flags.
        pipeline_barrier.src_stage_mask, //
        pipeline_barrier.dst_stage_mask, //
        pipeline_barrier.dependency_flags,
        //
        // Global memory barriers.
        static_cast<uint32_t>(pipeline_barrier.memory_barriers.size()),
        pipeline_barrier.memory_barriers.data(),
        // Buffer memory barriers.
        static_cast<uint32_t>(pipeline_barrier.buffer_memory_barriers.size()),
        pipeline_barrier.buffer_memory_barriers.data(),
        // Image memory barriers.
        static_cast<uint32_t>(pipeline_barrier.image_memory_barriers.size()),
        pipeline_barrier.image_memory_barriers.data());
}

} // namespace sgc::vk
