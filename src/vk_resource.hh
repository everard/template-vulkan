// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#ifndef H_3CE2314C208B4EF68E271071FDB7860E
#define H_3CE2314C208B4EF68E271071FDB7860E

#include <utility>
#include <vulkan/vulkan.h>

namespace sgc::vk {

////////////////////////////////////////////////////////////////////////////////
// Vulkan object deleter definition.
////////////////////////////////////////////////////////////////////////////////

template <typename H>
using object_deleter = void(H, VkAllocationCallbacks const*);

////////////////////////////////////////////////////////////////////////////////
// Vulkan object definition.
////////////////////////////////////////////////////////////////////////////////

template <typename H, object_deleter<H> deleter>
struct object {
    ////////////////////////////////////////////////////////////////////////////
    // Construction/destruction.
    ////////////////////////////////////////////////////////////////////////////

    object(H handle = nullptr) noexcept {
        this->handle = handle;
    }

    ~object() {
        if(this->handle != nullptr) {
            deleter(this->handle, nullptr);
        }
    }

    object(object const&) = delete;
    object(object&& other) noexcept
        : handle{std::exchange(other.handle, nullptr)} {
    }

    ////////////////////////////////////////////////////////////////////////////
    // Assignment operator.
    ////////////////////////////////////////////////////////////////////////////

    auto
    operator=(object other) noexcept -> object& {
        std::swap(this->handle, other.handle);
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Conversion operator.
    ////////////////////////////////////////////////////////////////////////////

    operator H() const noexcept {
        return this->handle;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Data members.
    ////////////////////////////////////////////////////////////////////////////

    H handle;
};

////////////////////////////////////////////////////////////////////////////////
// Vulkan resource deleter definition.
////////////////////////////////////////////////////////////////////////////////

template <typename P, typename H>
using resource_deleter = void(P, H, VkAllocationCallbacks const*);

////////////////////////////////////////////////////////////////////////////////
// Vulkan resource definition.
////////////////////////////////////////////////////////////////////////////////

namespace generic {

// Define generic version of resource.
template <typename P, typename H, resource_deleter<P, H> deleter>
struct resource {
    ////////////////////////////////////////////////////////////////////////////
    // Construction/destruction.
    ////////////////////////////////////////////////////////////////////////////

    resource(P parent = nullptr, H handle = nullptr) noexcept {
        this->parent = parent;
        this->handle = handle;
    }

    ~resource() {
        if(this->handle != nullptr) {
            deleter(this->parent, this->handle, nullptr);
        }
    }

    resource(resource const&) = delete;
    resource(resource&& other) noexcept
        : parent{std::exchange(other.parent, nullptr)}
        , handle{std::exchange(other.handle, nullptr)} {
    }

    ////////////////////////////////////////////////////////////////////////////
    // Assignment operator.
    ////////////////////////////////////////////////////////////////////////////

    auto
    operator=(resource other) noexcept -> resource& {
        std::swap(this->parent, other.parent);
        std::swap(this->handle, other.handle);
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Conversion operator.
    ////////////////////////////////////////////////////////////////////////////

    operator H() const noexcept {
        return this->handle;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Data members.
    ////////////////////////////////////////////////////////////////////////////

    P parent;
    H handle;
};

} // namespace generic

// Define version of resource with Vulkan device as parent.
template <typename H, resource_deleter<VkDevice, H> deleter>
struct resource : generic::resource<VkDevice, H, deleter> {
    using generic::resource<VkDevice, H, deleter>::resource;
};

} // namespace sgc::vk

#endif // H_3CE2314C208B4EF68E271071FDB7860E
