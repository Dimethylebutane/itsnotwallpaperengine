#include <vulkan/vulkan.h>
#include <iostream>
#include <vulkan/vulkan_wayland.h>
#include <wayland-client.h>

#include "External.hpp"

#define CHECK_VK_RESULT(_expr) \
result = _expr; \
if (result != VK_SUCCESS) \
{ \
    printf("Error executing %s: %i\n", #_expr, result); \
}

#define GET_EXTENSION_FUNCTION(_id) ((PFN_##_id)(vkGetInstanceProcAddr(instance, #_id)))

#define CHECK_WL_RESULT(_expr) \
if (!(_expr)) \
{ \
    printf("Error executing %s.", #_expr); \
}

struct SwapchainElement
{
    VkCommandBuffer commandBuffer;
    VkImage image;
    VkImageView imageView;
    VkFramebuffer framebuffer;
    VkSemaphore startSemaphore;
    VkSemaphore endSemaphore;
    VkFence fence;
    VkFence lastFence;
};

__attribute__((visibility("default"))) void fun(ExternalRendererCom* Com);
__attribute__((visibility("default"))) ExternalRendererThrd_pfun renderer = fun;

//TODO: handle surface change

//ugly but should do the work
thread_local static const char* const appName = "Wayland Vulkan Example";
thread_local static VkInstance instance = VK_NULL_HANDLE;
thread_local static VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
thread_local static VkSurfaceKHR vulkanSurface = VK_NULL_HANDLE;
thread_local static VkPhysicalDevice physDevice = VK_NULL_HANDLE;
thread_local static VkDevice device = VK_NULL_HANDLE;
thread_local static uint32_t queueFamilyIndex = 0;
thread_local static VkQueue queue = VK_NULL_HANDLE;
thread_local static VkCommandPool commandPool = VK_NULL_HANDLE;
thread_local static VkSwapchainKHR swapchain = VK_NULL_HANDLE;
thread_local static VkRenderPass renderPass = VK_NULL_HANDLE;
thread_local static struct SwapchainElement* elements = NULL;
thread_local static struct wl_display* display = NULL;
thread_local static struct wl_surface* surface = NULL;
thread_local static int resize = 0;
thread_local static int newWidth = 0;
thread_local static int newHeight = 0;
thread_local static wl_surface* newSurface;
thread_local static bool newSurfaceToUse;
thread_local static VkFormat format = VK_FORMAT_UNDEFINED;
thread_local static uint32_t width = 1600;
thread_local static uint32_t height = 900;
thread_local static uint32_t currentFrame = 0;
thread_local static uint32_t imageIndex = 0;
thread_local static uint32_t imageCount = 0;

/*static void handleRegistry(
    void* data,
	struct wl_registry* registry,
	uint32_t name,
    const char* interface,
    uint32_t version
);

static const struct wl_registry_listener registryListener = {
    .global = handleRegistry
};

static void handleShellPing(void* data, struct xdg_wm_base* shell, uint32_t serial)
{
    xdg_wm_base_pong(shell, serial);
}

static const struct xdg_wm_base_listener shellListener = {
    .ping = handleShellPing
};

static void handleShellSurfaceConfigure(void* data, struct xdg_surface* shellSurface, uint32_t serial)
{
    xdg_surface_ack_configure(shellSurface, serial);

    if (resize)
    {
        readyToResize = 1;
    }
}

static const struct xdg_surface_listener shellSurfaceListener = {
    .configure = handleShellSurfaceConfigure
};

static void handleToplevelConfigure(
    void* data,
    struct xdg_toplevel* toplevel,
    int32_t width,
    int32_t height,
    struct wl_array* states
)
{
    if (width != 0 && height != 0)
    {
        resize = 1;
        newWidth = width;
        newHeight = height;
    }
}

static void handleToplevelClose(void* data, struct xdg_toplevel* toplevel)
{
    quit = 1;
}

static const struct xdg_toplevel_listener toplevelListener = {
    .configure = handleToplevelConfigure,
    .close = handleToplevelClose
};

static void handleRegistry(
    void* data,
	struct wl_registry* registry,
	uint32_t name,
    const char* interface,
    uint32_t version
)
{
    if (strcmp(interface, wl_compositor_interface.name) == 0)
    {
        CHECK_WL_RESULT(compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 1));
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        CHECK_WL_RESULT(shell = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
        xdg_wm_base_add_listener(shell, &shellListener, NULL);
    }
}*/

static VkBool32 onError(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData
)
{
    printf("Vulkan ");

    switch (type)
    {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT :
        printf("general ");
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT :
        printf("validation ");
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT :
        printf("performance ");
        break;
    }

    switch (severity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT :
        printf("(verbose): ");
        break;
    default :
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT :
        printf("(info): ");
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT :
        printf("(warning): ");
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT :
        printf("(error): ");
        break;
    }

    printf("%s\n", callbackData->pMessage);

    return 0;
}

static void createSwapchain()
{
    VkResult result;

    {
        VkSurfaceCapabilitiesKHR capabilities;
        CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, vulkanSurface, &capabilities));

        uint32_t formatCount;
        CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, vulkanSurface, &formatCount, NULL));

        VkSurfaceFormatKHR formats[formatCount];
        CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, vulkanSurface, &formatCount, formats));

        VkSurfaceFormatKHR chosenFormat = formats[0];

        for (uint32_t i = 0; i < formatCount; i++)
        {
            if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM)
            {
                chosenFormat = formats[i];
                break;
            }
        }

        format = chosenFormat.format;

        imageCount = capabilities.minImageCount + 1 < capabilities.maxImageCount
            ? capabilities.minImageCount + 1
            : capabilities.minImageCount;

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = vulkanSurface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = chosenFormat.format;
        createInfo.imageColorSpace = chosenFormat.colorSpace;
        createInfo.imageExtent.width = width;
        createInfo.imageExtent.height = height;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        createInfo.clipped = 1;

        CHECK_VK_RESULT(vkCreateSwapchainKHR(device, &createInfo, NULL, &swapchain));
    }

    {
        VkAttachmentDescription attachment = {0};
        attachment.format = format;
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference attachmentRef = {0};
        attachmentRef.attachment = 0;
        attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {0};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &attachmentRef;

        VkRenderPassCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = &attachment;
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &subpass;

        CHECK_VK_RESULT(vkCreateRenderPass(device, &createInfo, NULL, &renderPass));
    }

    CHECK_VK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, NULL));

    VkImage images[imageCount];
    CHECK_VK_RESULT(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images));

    elements = (SwapchainElement*)malloc(imageCount * sizeof(struct SwapchainElement));

    for (uint32_t i = 0; i < imageCount; i++)
    {
        {
            VkCommandBufferAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = commandPool;
            allocInfo.commandBufferCount = 1;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            vkAllocateCommandBuffers(device, &allocInfo, &elements[i].commandBuffer);
        }

        elements[i].image = images[i];

        {
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            createInfo.image = elements[i].image;
            createInfo.format = format;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

            CHECK_VK_RESULT(vkCreateImageView(device, &createInfo, NULL, &elements[i].imageView));
        }

        {
            VkFramebufferCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.renderPass = renderPass;
            createInfo.attachmentCount = 1;
            createInfo.pAttachments = &elements[i].imageView;
            createInfo.width = width;
            createInfo.height = height;
            createInfo.layers = 1;

            CHECK_VK_RESULT(vkCreateFramebuffer(device, &createInfo, NULL, &elements[i].framebuffer));
        }

        {
            VkSemaphoreCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            CHECK_VK_RESULT(vkCreateSemaphore(device, &createInfo, NULL, &elements[i].startSemaphore));
        }

        {
            VkSemaphoreCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            CHECK_VK_RESULT(vkCreateSemaphore(device, &createInfo, NULL, &elements[i].endSemaphore));
        }

        {
            VkFenceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            CHECK_VK_RESULT(vkCreateFence(device, &createInfo, NULL, &elements[i].fence));
        }

        elements[i].lastFence = VK_NULL_HANDLE;
    }
    resize = false;
}

static void destroySwapchain()
{
    for (uint32_t i = 0; i < imageCount; i++)
    {
        vkDestroyFence(device, elements[i].fence, NULL);
        vkDestroySemaphore(device, elements[i].endSemaphore, NULL);
        vkDestroySemaphore(device, elements[i].startSemaphore, NULL);
        vkDestroyFramebuffer(device, elements[i].framebuffer, NULL);
        vkDestroyImageView(device, elements[i].imageView, NULL);
        vkFreeCommandBuffers(device, commandPool, 1, &elements[i].commandBuffer);
    }

    free(elements);

    vkDestroyRenderPass(device, renderPass, NULL);

    vkDestroySwapchainKHR(device, swapchain, NULL);
}

void getComData(ExternalRendererCom* pdata) {
   newWidth = pdata->updtData.width;
   newHeight = pdata->updtData.height;
   newSurface = pdata->updtData.surface;
   newSurfaceToUse = (newSurface != surface);
   resize = (width != newWidth) || (height != newHeight) || newSurfaceToUse;

   std::cout << "Extrnl updt: " << newWidth << " " << newHeight << "\n";
   pdata->status.store(true, std::memory_order_release);
}

bool handleUpdate(ExternalRendererCom* Com) {
   switch(Com->event.load(std::memory_order_acquire)){
      case ExternalRendererCom::Event::none:
         return true;
         break;
      case ExternalRendererCom::Event::update:
         std::atomic_thread_fence(std::memory_order_acquire); //get data
         getComData(Com);
         resize = true;
         return true;
         break;
      case ExternalRendererCom::Event::terminate:
         return false;
         break;
   }
   Com->event.store(ExternalRendererCom::Event::none, std::memory_order_acquire);
   return false; //???
}


void createVkstuff() { //also create swapchain
    VkResult result;
    {
        VkWaylandSurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        createInfo.display = display;
        createInfo.surface = surface;

        CHECK_VK_RESULT(vkCreateWaylandSurfaceKHR(instance, &createInfo, NULL, &vulkanSurface));
    }

    
    {
        const char* const extensionNames[] = { "VK_KHR_swapchain" };

        const char* const layerNames[] = { "VK_LAYER_KHRONOS_validation" };

        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, NULL);

        VkQueueFamilyProperties queueFamilies[queueFamilyCount];
        vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, queueFamilies);

        for (uint32_t i = 0; i < queueFamilyCount; i++)
        {
            VkBool32 present = 0;

            CHECK_VK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(physDevice, i, vulkanSurface, &present));

            if (present && (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
            {
                queueFamilyIndex = i;
                break;
            }
        }
        
        float priority = 1;

        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &priority;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.enabledExtensionCount = sizeof(extensionNames) / sizeof(const char*);
        createInfo.ppEnabledExtensionNames = extensionNames;
        createInfo.enabledLayerCount = sizeof(layerNames) / sizeof(const char*);
        createInfo.ppEnabledLayerNames = layerNames;

        CHECK_VK_RESULT(vkCreateDevice(physDevice, &createInfo, NULL, &device));

        vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);
    }

    {
        VkCommandPoolCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.queueFamilyIndex = queueFamilyIndex;
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        CHECK_VK_RESULT(vkCreateCommandPool(device, &createInfo, NULL, &commandPool));
    }

    createSwapchain();
   newSurfaceToUse = false;
}

void cleanVkstuff() {
    VkResult result;
    CHECK_VK_RESULT(vkDeviceWaitIdle(device));

    destroySwapchain();

    vkDestroyCommandPool(device, commandPool, NULL);
    vkDestroyDevice(device, NULL);
    vkDestroySurfaceKHR(instance, vulkanSurface, NULL);
}

__attribute__((visibility("default"))) void fun(ExternalRendererCom* Com)
{

   std::cout << "Hello from SO\n";

    std::atomic_thread_fence(std::memory_order_acquire); //get data
    getComData(Com);
    newSurfaceToUse = false;
    display = Com->display;
    surface = newSurface;
    resize = false;
    width = newWidth;
    height = newHeight;

    //make sure configuration by compositor
    wl_surface_commit(surface);
    handleUpdate(Com);
    wl_display_roundtrip(display);
    handleUpdate(Com);
    wl_surface_commit(surface);
    handleUpdate(Com);

    newSurfaceToUse = false;
    resize = false;
    width = newWidth;
    height = newHeight;

    VkResult result;

    {
        const char* const extensionNames[] = {
            "VK_EXT_debug_utils",
            "VK_KHR_surface",
            "VK_KHR_wayland_surface"
        };

        const char* const layerNames[] = {
            "VK_LAYER_KHRONOS_validation"
        };

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = appName;
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        appInfo.pEngineName = appName;
        appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = sizeof(extensionNames) / sizeof(const char*);
        createInfo.ppEnabledExtensionNames = extensionNames;
        createInfo.enabledLayerCount = sizeof(layerNames) / sizeof(const char*);
        createInfo.ppEnabledLayerNames = layerNames;

        CHECK_VK_RESULT(vkCreateInstance(&createInfo, NULL, &instance));
    }
    //Physical Device
    uint32_t physDeviceCount;
    vkEnumeratePhysicalDevices(instance, &physDeviceCount, NULL);

    VkPhysicalDevice physDevices[physDeviceCount];
    vkEnumeratePhysicalDevices(instance, &physDeviceCount, physDevices);

    uint32_t bestScore = 0;

    for (uint32_t i = 0; i < physDeviceCount; i++)
    {
        VkPhysicalDevice device = physDevices[i];

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);

        uint32_t score;

        switch (properties.deviceType)
        {
        default :
            continue;
        case VK_PHYSICAL_DEVICE_TYPE_OTHER :
            score = 1;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU :
            score = 4;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU :
            score = 5;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU :
            score = 3;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU :
            score = 2;
            break;
        }

        if (score > bestScore)
        {
            physDevice = device;
            bestScore = score;
        }
    }


    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = onError;

        CHECK_VK_RESULT(GET_EXTENSION_FUNCTION(vkCreateDebugUtilsMessengerEXT)(instance, &createInfo, NULL, &debugMessenger));
    }

    //compositor dependant:
    createVkstuff();

    while (handleUpdate(Com))
    {
        if (newSurfaceToUse) {
           cleanVkstuff();
           surface = newSurface;
           createVkstuff(); //also recreate swapchain
           continue;
        } else if (resize)
        {
            width = newWidth;
            height = newHeight;

            CHECK_VK_RESULT(vkDeviceWaitIdle(device));

            destroySwapchain();
            createSwapchain();

            currentFrame = 0;
            imageIndex = 0;

            //wl_surface_commit(surface);
        }

        struct SwapchainElement* currentElement = &elements[currentFrame];

        CHECK_VK_RESULT(vkWaitForFences(device, 1, &currentElement->fence, 1, UINT64_MAX));
        result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, currentElement->startSemaphore, NULL, &imageIndex);

        if (result != VK_SUBOPTIMAL_KHR)
        {
            CHECK_VK_RESULT(result);
        }

        struct SwapchainElement* element = &elements[imageIndex];

        if (element->lastFence)
        {
            CHECK_VK_RESULT(vkWaitForFences(device, 1, &element->lastFence, 1, UINT64_MAX));
        }

        element->lastFence = currentElement->fence;

        CHECK_VK_RESULT(vkResetFences(device, 1, &currentElement->fence));

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        CHECK_VK_RESULT(vkBeginCommandBuffer(element->commandBuffer, &beginInfo));

        {
           static float f = 0;
           static float d = 0.1;
           f += d;
           if (f >= 1) {
              f = 1;
              d = -d;
           } else if (f <= 0) {
              f = 0;
              d = -d;
           }

           //Ugly as fuck TODO
            VkClearValue clearValue = {{
                1.0f,
                0.0f,
                1.0f,
                1.0f
            }};

            VkRenderPassBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            beginInfo.renderPass = renderPass;
            beginInfo.framebuffer = element->framebuffer;
            beginInfo.renderArea.offset.x = 0;
            beginInfo.renderArea.offset.y = 0;
            beginInfo.renderArea.extent.width = width;
            beginInfo.renderArea.extent.height = height;
            beginInfo.clearValueCount = 1;
            beginInfo.pClearValues = &clearValue;

            vkCmdBeginRenderPass(element->commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdEndRenderPass(element->commandBuffer);
        }

        CHECK_VK_RESULT(vkEndCommandBuffer(element->commandBuffer));

        const VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &currentElement->startSemaphore;
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &element->commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &currentElement->endSemaphore;

        CHECK_VK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, currentElement->fence));

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &currentElement->endSemaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain;
        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(queue, &presentInfo);

        if (result != VK_SUBOPTIMAL_KHR)
        {
            CHECK_VK_RESULT(result);
        }

        currentFrame = (currentFrame + 1) % imageCount;

        //wl_display_roundtrip(display);
    }

    cleanVkstuff();
    GET_EXTENSION_FUNCTION(vkDestroyDebugUtilsMessengerEXT)(instance, debugMessenger, NULL);
    vkDestroyInstance(instance, NULL);

    Com->status.store(false, std::memory_order_release);

    return;
}

