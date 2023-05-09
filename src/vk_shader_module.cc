// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_shader_module.hh"

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Initialization interface implementation.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(VkDevice device, shader_module_parameters params)
    -> std::expected<shader_module, error> {
    // If the specified shader code is empty, then initialization fails.
    if(params.code.empty()) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, 0}};
    }

    // Initialize shader module creation info.
    auto info = VkShaderModuleCreateInfo{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .flags = params.flags,
        .codeSize = static_cast<uint32_t>(params.code.size_bytes()),
        .pCode = params.code.data()};

    // Initialize a null handle.
    auto handle = VkShaderModule{};

    // Try creating a new shader module.
    if(auto code = vkCreateShaderModule(device, &info, nullptr, &handle);
       code != VK_SUCCESS) {
        // Initialization failed: return error description.
        return std::unexpected{error{__LINE__, code}};
    }

    // Return created shader module.
    return shader_module{device, handle};
}

} // namespace sgc::vk
