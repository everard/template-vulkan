// Copyright Nezametdinov E. Ildus 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
#include "vk_command_pool.hh"
#include "vk_device.hh"
#include "vk_image.hh"
#include "vk_image_view.hh"
#include "vk_instance.hh"
#include "vk_memory.hh"
#include "vk_pipeline_barrier.hh"
#include "vk_semaphore.hh"
#include "vk_swapchain.hh"

#include <chrono>
#include <thread>

namespace sgc {

////////////////////////////////////////////////////////////////////////////////
// Window definition.
////////////////////////////////////////////////////////////////////////////////

namespace detail {

using window_deleter = decltype([](SDL_Window* window) {
    if(window != nullptr) {
        SDL_DestroyWindow(window);
    }
});

} // namespace detail

using window = std::unique_ptr<SDL_Window, detail::window_deleter>;

////////////////////////////////////////////////////////////////////////////////
// Vulkan extension query utility functions.
////////////////////////////////////////////////////////////////////////////////

static auto
obtain_instance_extensions(SDL_Window* window) -> std::vector<vk::utf8_ntbs> {
    auto n = unsigned{};
    if(SDL_Vulkan_GetInstanceExtensions(window, &n, nullptr) != SDL_TRUE) {
        return {};
    }

    auto extensions = std::vector<vk::utf8_ntbs>{n, nullptr};
    if(SDL_Vulkan_GetInstanceExtensions(window, &n, extensions.data()) !=
       SDL_TRUE) {
        return {};
    }

    return extensions;
}

static auto
obtain_device_extensions() -> std::vector<vk::utf8_ntbs> {
    return {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
}

////////////////////////////////////////////////////////////////////////////////
// Texture definition.
////////////////////////////////////////////////////////////////////////////////

struct texture {
    vk::memory memory;
    vk::image image;
    vk::image_view image_view;
};

////////////////////////////////////////////////////////////////////////////////
// Texture initialization parameters definition.
////////////////////////////////////////////////////////////////////////////////

struct texture_parameters {
    // Texture image type.
    VkImageType image_type;

    // Format and extent.
    VkFormat format;
    VkExtent3D extent;

    // Flags.
    VkImageUsageFlags image_usage_flags;
    VkMemoryPropertyFlags memory_property_flags;
};

////////////////////////////////////////////////////////////////////////////////
// Texture initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(vk::device const& device, texture_parameters params)
    -> std::expected<texture, vk::error> {
    // Initialize image.
    auto image = vk::initialize(
        device, VkImageCreateInfo{.imageType = params.image_type,
                                  .format = params.format,
                                  .extent = params.extent,
                                  .mipLevels = 1,
                                  .arrayLayers = 1,
                                  .samples = VK_SAMPLE_COUNT_1_BIT,
                                  .tiling = VK_IMAGE_TILING_OPTIMAL,
                                  .usage = params.image_usage_flags});

    if(!image) {
        return std::unexpected{image.error()};
    }

    // Allocate memory.
    auto allocation_parameters = vk::memory_allocation_parameters{
        .property_flags = params.memory_property_flags};

    vkGetImageMemoryRequirements(
        device, *image, &(allocation_parameters.requirements));

    auto memory = allocate(device, allocation_parameters);
    if(!memory) {
        return std::unexpected{memory.error()};
    }

    // Bind allocated memory.
    if(auto code = vkBindImageMemory(device, *image, *memory, 0);
       code != VK_SUCCESS) {
        return std::unexpected{vk::error{__LINE__, code}};
    }

    // Initialize image view.
    auto aspect_mask = VkImageAspectFlags{VK_IMAGE_ASPECT_COLOR_BIT};
    if((params.format == VK_FORMAT_D16_UNORM) ||
       (params.format == VK_FORMAT_X8_D24_UNORM_PACK32) ||
       (params.format == VK_FORMAT_D32_SFLOAT)) {
        aspect_mask = VK_IMAGE_ASPECT_DEPTH_BIT;
    } else if(params.format == VK_FORMAT_S8_UINT) {
        aspect_mask = VK_IMAGE_ASPECT_STENCIL_BIT;
    } else if((params.format == VK_FORMAT_D16_UNORM_S8_UINT) ||
              (params.format == VK_FORMAT_D24_UNORM_S8_UINT) ||
              (params.format == VK_FORMAT_D32_SFLOAT_S8_UINT)) {
        aspect_mask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    auto image_view = initialize(
        device, VkImageViewCreateInfo{
                    .image = *image,
                    .viewType = static_cast<VkImageViewType>(params.image_type),
                    .format = params.format,
                    .subresourceRange = {.aspectMask = aspect_mask,
                                         .baseMipLevel = 0,
                                         .levelCount = 1,
                                         .baseArrayLayer = 0,
                                         .layerCount = 1}});

    if(!image_view) {
        return std::unexpected{image_view.error()};
    }

    // Return created texture.
    return texture{.memory = std::move(*memory),
                   .image = std::move(*image),
                   .image_view = std::move(*image_view)};
}

////////////////////////////////////////////////////////////////////////////////
// Main context definition.
////////////////////////////////////////////////////////////////////////////////

struct main_context {
    ~main_context() {
        if(this->device.handle != nullptr) {
            vkDeviceWaitIdle(this->device);
        }
    }

    // Vulkan instance.
    vk::instance instance;

    // Selected physical device.
    vk::physical_device physical_device;

    // Render surface and its properties.
    vk::surface surface;
    vk::surface_properties surface_properties;

    // Vulkan logical device.
    vk::device device;

    // Semaphores.
    struct {
        vk::semaphore rendering, swapchain, presentation;
    } semaphores;

    // Command pools.
    struct {
        vk::command_pool rendering, presentation;
    } command_pools;

    // Command buffers.
    struct {
        std::vector<VkCommandBuffer> rendering, presentation;
    } command_buffers;

    // Render target.
    texture render_target;

    // Vulkan swapchain and its initialization parameters.
    vk::swapchain swapchain;
    vk::swapchain_parameters swapchain_parameters;

    // Swapchain data.
    std::vector<VkImage> swapchain_images;
};

////////////////////////////////////////////////////////////////////////////////
// Main context initialization interface.
////////////////////////////////////////////////////////////////////////////////

auto
initialize(main_context& context, SDL_Window* window) -> bool {
#define print_error_(x)                                               \
    printf("error: %s: %s, %d, %lld, %lld\n", __func__, #x, __LINE__, \
           x.error().line, x.error().code)

    // Create Vulkan instance.
    if(true) {
        auto instance = initialize(vk::instance_parameters{
            .api_version = VK_API_VERSION_1_3,
            .extensions = obtain_instance_extensions(window)});

        if(!instance) {
            print_error_(instance);
            return false;
        } else {
            context.instance = std::move(*instance);
        }
    }

    // Select physical device.
    if(true) {
        auto physical_device =
            select(context.instance,
                   vk::physical_device_preference{
                       .api_version = VK_API_VERSION_1_3,
                       .type = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU});

        if(!physical_device) {
            print_error_(physical_device);
            return false;
        } else {
            context.physical_device = *physical_device;
        }
    }

    // Obtain physical device features.
    auto features = vk::physical_device_features{context.physical_device};
    if(true) {
        auto is_feature_complete =
            features.vulkan_1_2.descriptorIndexing &&
            features.vulkan_1_2.descriptorBindingVariableDescriptorCount &&
            features.vulkan_1_2.drawIndirectCount &&
            features.common.features.multiDrawIndirect &&
            features.common.features.drawIndirectFirstInstance;

        if(!is_feature_complete) {
            printf("error: selected physical device is missing features\n");
            return false;
        }
    }

    // Create Vulkan surface.
    if(true) {
        auto surface = initialize(
            context.instance, vk::surface_parameters{.window = window});

        if(!surface) {
            print_error_(surface);
            return false;
        } else {
            context.surface = std::move(*surface);
        }
    }

    // Obtain surface properties.
    if(true) {
        auto surface_properties =
            obtain_properties(context.surface, context.physical_device);

        if(!surface_properties) {
            print_error_(surface_properties);
            return false;
        } else {
            context.surface_properties = std::move(*surface_properties);
        }
    }

    // Check surface properties.
    if((context.surface_properties.capabilities.supportedUsageFlags &
        VK_IMAGE_USAGE_TRANSFER_DST_BIT) == 0) {
        printf("error: %s: %s, %d\n", __func__,
               "surface properties: surface does not support transfer",
               __LINE__);

        return false;
    }

    if(context.surface_properties.capabilities.minImageCount > 3) {
        printf("error: %s: %s, %d\n", __func__,
               "surface properties: surface requires too many images in its "
               "swapchain",
               __LINE__);

        return false;
    }

    // Initialize swapchain parameters.
    context.swapchain_parameters = vk::swapchain_parameters{
        .surface = context.surface,
        .surface_transform =
            context.surface_properties.capabilities.currentTransform,
        .image_usage_flags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                             VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .n_images = 3,
        .n_image_array_layers = 1};

    // Select the most suitable format.
    for(context.swapchain_parameters.surface_format =
            context.surface_properties.formats[0];
        auto x : context.surface_properties.formats) {
        if((x.format == VK_FORMAT_R8G8B8A8_UNORM) ||
           (x.format == VK_FORMAT_R8G8B8A8_SNORM)) {
            context.swapchain_parameters.surface_format = x;
            break;
        }
    }

    // Select the most suitable presentation mode.
    for(context.swapchain_parameters.present_mode = VK_PRESENT_MODE_FIFO_KHR;
        auto x : context.surface_properties.modes) {
        if(x == VK_PRESENT_MODE_MAILBOX_KHR) {
            context.swapchain_parameters.present_mode = x;
            break;
        }
    }

    // Create Vulkan device.
    if(true) {
        auto device = initialize(
            context.physical_device,
            vk::device_parameters{.features = features,
                                  .extensions = obtain_device_extensions(),
                                  .surface = context.surface});

        if(!device) {
            print_error_(device);
            return false;
        } else {
            context.device = std::move(*device);
        }
    }

    // Create semaphores.
    if(true) {
        auto semaphores = std::array{&(context.semaphores.rendering),
                                     &(context.semaphores.swapchain),
                                     &(context.semaphores.presentation)};

        for(auto x : semaphores) {
            auto semaphore =
                initialize(context.device, vk::semaphore_parameters{});

            if(!semaphore) {
                print_error_(semaphore);
                return false;
            } else {
                *x = std::move(*semaphore);
            }
        }
    }

    // Create command pools.
    //
    // Note: Command pool for presentation only prepares swapchain images for
    // presenting on surface. It uses graphics queue, NOT presentation queue.
    if(true) {
        auto command_pools = std::array{&(context.command_pools.rendering),
                                        &(context.command_pools.presentation)};

        for(auto x : command_pools) {
            auto command_pool = vk::initialize(
                context.device,
                VkCommandPoolCreateInfo{
                    .queueFamilyIndex =
                        context.device.queues.family_indices.graphics});

            if(!command_pool) {
                print_error_(command_pool);
                return false;
            } else {
                *x = std::move(*command_pool);
            }
        }
    }

    // Allocate command buffers for rendering.
    context.command_buffers.rendering = std::move(vk::allocate(
        context.command_pools.rendering,
        {.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .commandBufferCount = 1}));

    if(context.command_buffers.rendering.empty()) {
        printf("error: %s: %s, %d\n", __func__,
               "command buffers allocation: rendering", __LINE__);

        return false;
    }

    // Allocate command buffers for presentation.
    context.command_buffers.presentation = std::move(vk::allocate(
        context.command_pools.presentation,
        {.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .commandBufferCount = 16}));

    if(context.command_buffers.presentation.empty()) {
        printf("error: %s: %s, %d\n", __func__,
               "command buffers allocation: presentation", __LINE__);

        return false;
    }

    return true;
}

auto
initialize_swapchain(main_context& context, SDL_Window* window) -> bool {
    // Reset render target.
    context.render_target = texture{};

    // Clear array of swapchain images.
    context.swapchain_images.clear();

    // Obtain screen size.
    auto screen_size = VkExtent2D{};
    if(auto w = 0, h = 0; true) {
        if(SDL_GetWindowSize(window, &w, &h); (w <= 0) || (h <= 0)) {
            return false;
        }

        screen_size.width = static_cast<uint32_t>(w);
        screen_size.height = static_cast<uint32_t>(h);
    }

    // Initialize render target.
    if(true) {
        auto render_target = initialize(
            context.device,
            texture_parameters{
                .image_type = VK_IMAGE_TYPE_2D,
                .format = context.swapchain_parameters.surface_format.format,
                .extent = {screen_size.width, screen_size.height, 1},
                .image_usage_flags = VK_IMAGE_USAGE_STORAGE_BIT |
                                     VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                     VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                     VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .memory_property_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT});

        if(!render_target) {
            print_error_(render_target);
            return false;
        } else {
            context.render_target = std::move(*render_target);
        }
    }

    // Initialize swapchain.
    if(context.swapchain_parameters.image_extent = screen_size; true) {
        context.swapchain.parent = context.device;

        auto swapchain = initialize(
            std::move(context.swapchain), context.swapchain_parameters);

        if(!swapchain) {
            print_error_(swapchain);
            return false;
        } else {
            context.swapchain = std::move(*swapchain);
        }
    }

    // Reset command pools.
    if(!reset(context.command_pools.rendering) ||
       !reset(context.command_pools.presentation)) {
        return false;
    }

    // Obtain swapchain images.
    if(auto images = obtain_images(context.swapchain); !images) {
        print_error_(images);
        return false;
    } else {
        context.swapchain_images = std::move(*images);
    }

    // Obtain queue family index.
    auto queue_family_index = context.device.queues.family_indices;

    // Record rendering command buffers.
    if(auto command_buffer = context.command_buffers.rendering[0]; true) {
        // Initialize image sub-resource range.
        auto image_subresource_range =
            VkImageSubresourceRange{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                    .baseMipLevel = 0,
                                    .levelCount = 1,
                                    .baseArrayLayer = 0,
                                    .layerCount = 1};

        // Begin recording.
        if(true) {
            auto info = VkCommandBufferBeginInfo{
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

            if(vkBeginCommandBuffer(command_buffer, &info) != VK_SUCCESS) {
                return false;
            }
        }

        // Memory barrier for the render target.
        if(true) {
            VkImageMemoryBarrier barriers[] = {
                {.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                 .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
                 .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
                 .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                 .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                 .srcQueueFamilyIndex = queue_family_index.graphics,
                 .dstQueueFamilyIndex = queue_family_index.graphics,
                 .image = context.render_target.image,
                 .subresourceRange = image_subresource_range}};

            barrier(command_buffer,
                    vk::pipeline_barrier{
                        .src_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT,
                        .dst_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT,
                        .image_memory_barriers = barriers});
        }

        // Clear the image.
        if(auto color = VkClearColorValue{.float32 = {1.0f, 0.0f, 0.0f, 1.0f}};
           true) {
            vkCmdClearColorImage( //
                command_buffer, context.render_target.image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &color, 1,
                &image_subresource_range);
        }

        // End recording.
        if(vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
            return false;
        }
    }

    // Record presentation command buffers.
    for(auto i = 0z; auto image : context.swapchain_images) {
        auto command_buffer = context.command_buffers.presentation[i++];

        // Initialize image sub-resource range.
        auto image_subresource_range =
            VkImageSubresourceRange{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                    .baseMipLevel = 0,
                                    .levelCount = 1,
                                    .baseArrayLayer = 0,
                                    .layerCount = 1};

        // Begin recording.
        if(true) {
            auto info = VkCommandBufferBeginInfo{
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

            if(vkBeginCommandBuffer(command_buffer, &info) != VK_SUCCESS) {
                return false;
            }
        }

        // Memory barriers for images.
        if(true) {
            VkImageMemoryBarrier barriers[] = {
                {.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                 .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
                 .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
                 .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                 .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                 .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                 .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                 .image = image,
                 .subresourceRange = image_subresource_range},

                {.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                 .srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT,
                 .dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
                 .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                 .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                 .srcQueueFamilyIndex = queue_family_index.graphics,
                 .dstQueueFamilyIndex = queue_family_index.graphics,
                 .image = context.render_target.image,
                 .subresourceRange = image_subresource_range},
            };

            barrier(command_buffer,
                    vk::pipeline_barrier{
                        .src_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT,
                        .dst_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT,
                        .image_memory_barriers = barriers});
        }

        // Copy render target into swapchain image.
        if(true) {
            auto blit = VkImageBlit{
                .srcSubresource = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                   .mipLevel = 0,
                                   .baseArrayLayer = 0,
                                   .layerCount = 1},
                .srcOffsets = {{},
                               {static_cast<int32_t>(screen_size.width),
                                static_cast<int32_t>(screen_size.height), 0}},

                .dstSubresource = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                   .mipLevel = 0,
                                   .baseArrayLayer = 0,
                                   .layerCount = 1},
                .dstOffsets = {{},
                               {static_cast<int32_t>(screen_size.width),
                                static_cast<int32_t>(screen_size.height), 0}}};

            vkCmdBlitImage( //
                command_buffer, context.render_target.image,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit,
                VK_FILTER_NEAREST);
        }

        // Memory barrier for the swapchain image.
        if(true) {
            VkImageMemoryBarrier barriers[] = {
                {.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                 .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
                 .dstAccessMask = 0,
                 .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                 .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                 .srcQueueFamilyIndex = queue_family_index.graphics,
                 .dstQueueFamilyIndex = queue_family_index.presentation,
                 .image = image,
                 .subresourceRange = image_subresource_range}};

            barrier(command_buffer,
                    vk::pipeline_barrier{
                        .src_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT,
                        .dst_stage_mask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                        .image_memory_barriers = barriers});
        }

        // End recording.
        if(vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
            return false;
        }
    }

#undef print_error_

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Rendering interface.
////////////////////////////////////////////////////////////////////////////////

void
render(main_context& context) {
#define try_(expression)             \
    if((expression) != VK_SUCCESS) { \
        return;                      \
    }

    // Acquire the next image.
    auto image_idx = uint32_t{};
    try_(vkAcquireNextImageKHR( //
        context.device, context.swapchain, UINT64_MAX,
        context.semaphores.swapchain, nullptr, &image_idx));

    // Perform rendering.
    if(true) {
        VkPipelineStageFlags pipeline_stage_flags_presentation[] = {
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT};

        VkSemaphore wait_semaphores[] = {
            context.semaphores.swapchain, context.semaphores.rendering};

        VkSubmitInfo infos[] = {
            {.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
             .commandBufferCount = static_cast<uint32_t>(
                 context.command_buffers.rendering.size()),
             .pCommandBuffers = context.command_buffers.rendering.data(),
             .signalSemaphoreCount = 1,
             .pSignalSemaphores = &(context.semaphores.rendering.handle)},

            {.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
             .waitSemaphoreCount = std::size(wait_semaphores),
             .pWaitSemaphores = wait_semaphores,
             .pWaitDstStageMask = pipeline_stage_flags_presentation,
             .commandBufferCount = 1,
             .pCommandBuffers =
                 &(context.command_buffers.presentation[image_idx]),
             .signalSemaphoreCount = 1,
             .pSignalSemaphores = &(context.semaphores.presentation.handle)}};

        try_(vkQueueSubmit(
            context.device.queues.graphics, std::size(infos), infos, nullptr));
    }

    // Present rendered image.
    if(true) {
        auto info = VkPresentInfoKHR{
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &(context.semaphores.presentation.handle),
            .swapchainCount = 1,
            .pSwapchains = &(context.swapchain.handle),
            .pImageIndices = &image_idx};

        try_(vkQueuePresentKHR(context.device.queues.presentation, &info));
    }

#undef try_
}

} // namespace sgc

////////////////////////////////////////////////////////////////////////////////
// Application entry point.
////////////////////////////////////////////////////////////////////////////////

int
main() {
    // Initialize SDL library.
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        return EXIT_FAILURE;
    }

#if defined(_WIN32) || defined(_WINDOWS)
    if(SDL_Vulkan_LoadLibrary("libvulkan-1.dll") != 0) {
        return EXIT_FAILURE;
    }
#endif

    // Make sure the state is cleaned-up upon exit.
    struct exit_guard {
        ~exit_guard() {
            SDL_Quit();

#if defined(_WIN32) || defined(_WINDOWS)
            SDL_Vulkan_UnloadLibrary();
#endif
        }
    } exit_guard;

    // Create a new window.
    auto window = sgc::window{SDL_CreateWindow(
        "game", 0, 0, 1280, 720, SDL_WINDOW_BORDERLESS | SDL_WINDOW_VULKAN)};

    if(!window) {
        return EXIT_FAILURE;
    }

    // Create and initialize a context.
    auto context = sgc::main_context{};
    if(!initialize(context, window.get())) {
        return EXIT_FAILURE;
    }

    // Initialize a swapchain.
    if(!initialize_swapchain(context, window.get())) {
        return EXIT_FAILURE;
    }

    // Use relative pointer events.
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Enter the main loop.
    for(bool should_run = true; should_run;) {
        // Get current frame time.
        auto t0 = std::chrono::high_resolution_clock::now();

        // Process events.
        for(SDL_Event event; SDL_PollEvent(&event) != 0;) {
            switch(event.type) {
                case SDL_QUIT:
                    should_run = false;
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_q:
                            should_run = false;
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        // Render.
        render(context);

        // Compute elapsed time.
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t0);

        // Sleep between frames to reduce CPU load.
        if(delta.count() < 16) {
            auto dt = 16 - delta.count();
            std::this_thread::sleep_for(std::chrono::milliseconds{dt});
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds{2});
        }
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}
