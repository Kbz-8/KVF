/***
 *                              MIT License
 *
 *                      Copyright (c) 2023-2024 kbz_8
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * Do this:
 * #define KVF_IMPLEMENTATION
 * before you include this file in *one* C or C++ file to create the implementation.
 *
 * // i.e. it should look like this:
 * #include ...
 * #include ...
 * #include ...
 * #define KVF_IMPLEMENTATION
 * #include "kvf.h"
 *
 * You can #define KVF_ASSERT(x) before the #include to avoid using assert.h.
 * And #define KVF_MALLOC, KVF_REALLOC, and KVF_FREE to avoid using malloc, realloc, free.
 *
 * By default KVF exits the program if a call to the Vulkan API fails. You can avoid that
 * by using #define KVF_NO_EXIT_ON_FAILURE
 *
 * If you are using Volk or any other meta loader you must define KVF_IMPL_VK_NO_PROTOTYPES
 * or VK_NO_PROTOTYPES before including this file to avoid conflicts with Vulkan prototypes.
 *
 * You can also #define KVF_ENABLE_VALIDATION_LAYERS to enable validation layers.
 */

#ifndef KBZ_8_VULKAN_FRAMEWORK_H
#define KBZ_8_VULKAN_FRAMEWORK_H

#ifdef KVF_IMPL_VK_NO_PROTOTYPES
	#define VK_NO_PROTOTYPES
#endif

#include <vulkan/vulkan.h>

#include <stdint.h>
#include <stdbool.h>

/* ============================================= Prototypes ============================================= */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	KVF_GRAPHICS_QUEUE = 0,
	KVF_PRESENT_QUEUE = 1,
	KVF_COMPUTE_QUEUE = 2
} KvfQueueType;

typedef enum
{
	KVF_IMAGE_COLOR = 0,
	KVF_IMAGE_DEPTH = 1,
	KVF_IMAGE_DEPTH_ARRAY = 2,
	KVF_IMAGE_CUBE = 3,
	KVF_IMAGE_OTHER = 4,
} KvfImageType;

typedef void (*KvfErrorCallback)(const char* message);

typedef struct KvfGraphicsPipelineBuilder KvfGraphicsPipelineBuilder;

void kvfSetErrorCallback(KvfErrorCallback callback);
void kvfSetValidationErrorCallback(KvfErrorCallback callback);
void kvfSetValidationWarningCallback(KvfErrorCallback callback);

void kvfAddLayer(const char* layer);

VkInstance kvfCreateInstance(const char** extensionsEnabled, uint32_t extensionsCount);
void kvfDestroyInstance(VkInstance instance);

VkPhysicalDevice kvfPickFirstPhysicalDevice(VkInstance instance);
VkPhysicalDevice kvfPickGoodDefaultPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
VkPhysicalDevice kvfPickGoodPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const char** deviceExtensions, uint32_t deviceExtensionsCount);

VkQueue kvfGetDeviceQueue(VkDevice device, KvfQueueType queue);
uint32_t kvfGetDeviceQueueFamily(VkDevice device, KvfQueueType queue);
bool kvfQueuePresentKHR(VkDevice device, VkSemaphore wait, VkSwapchainKHR swapchain, uint32_t image_index); // return false when the swapchain must be recreated

VkDevice kvfCreateDefaultDevice(VkPhysicalDevice physical);
VkDevice kvfCreateDevice(VkPhysicalDevice physical, const char** extensions, uint32_t extensions_count);
void kvfDestroyDevice(VkDevice device);

VkFence kvfCreateFence(VkDevice device);
void kvfWaitForFence(VkDevice device, VkFence fence);
void kvfDestroyFence(VkDevice device, VkFence fence);

VkSemaphore kvfCreateSemaphore(VkDevice device);
void kvfDestroySemaphore(VkDevice device, VkSemaphore semaphore);

VkSwapchainKHR kvfCreateSwapchainKHR(VkDevice device, VkPhysicalDevice physical, VkSurfaceKHR surface, VkExtent2D extent, bool tryVsync);
VkFormat kvfGetSwapchainImagesFormat(VkSwapchainKHR swapchain);
uint32_t kvfGetSwapchainImagesCount(VkSwapchainKHR swapchain);
uint32_t kvfGetSwapchainMinImagesCount(VkSwapchainKHR swapchain);
VkExtent2D kvfGetSwapchainImagesSize(VkSwapchainKHR swapchain);
void kvfDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain);

VkImage kvfCreateImage(VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);
void kvfImageBufferToBuffer(VkCommandBuffer cmd, VkBuffer dst, VkImage src, size_t size);
void kvfDestroyImage(VkDevice device, VkImage image);
VkImageView kvfCreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageViewType type, VkImageAspectFlags aspect);
void kvfDestroyImageView(VkDevice device, VkImageView image_view);
void kvfTransitionImageLayout(VkDevice device, VkImage image, VkCommandBuffer cmd, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout, bool is_single_time_cmd_buffer);
VkSampler kvfCreateSampler(VkDevice device, VkFilter filters, VkSamplerAddressMode address_modes, VkSamplerMipmapMode mipmap_mode);
void kvfDestroySampler(VkDevice device, VkSampler sampler);

VkBuffer kvfCreateBuffer(VkDevice device, VkBufferUsageFlags usage, VkDeviceSize size);
void kvfCopyBufferToBuffer(VkCommandBuffer cmd, VkBuffer dst, VkBuffer src, size_t size);
void kvfCopyBufferToImage(VkCommandBuffer cmd, VkImage dst, VkBuffer src, size_t buffer_offset, VkImageAspectFlagBits aspect, VkExtent3D extent);
void kvfDestroyBuffer(VkDevice device, VkBuffer buffer);

VkFramebuffer kvfCreateFramebuffer(VkDevice device, VkRenderPass renderpass, VkImageView* image_views, size_t image_views_count, VkExtent2D extent);
VkExtent2D kvfGetFramebufferSize(VkFramebuffer buffer);
void kvfDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer);

VkCommandBuffer kvfCreateCommandBuffer(VkDevice device);
VkCommandBuffer kvfCreateCommandBufferLeveled(VkDevice device, VkCommandBufferLevel level);
void kvfBeginCommandBuffer(VkCommandBuffer buffer, VkCommandBufferUsageFlags flags);
void kvfEndCommandBuffer(VkCommandBuffer buffer);
void kvfSubmitCommandBuffer(VkDevice device, VkCommandBuffer buffer, KvfQueueType queue, VkSemaphore signal, VkSemaphore wait, VkFence fence, VkPipelineStageFlags* stages);
void kvfSubmitSingleTimeCommandBuffer(VkDevice device, VkCommandBuffer buffer, KvfQueueType queue, VkFence fence);

VkAttachmentDescription kvfBuildAttachmentDescription(KvfImageType type, VkFormat format, VkImageLayout initial, VkImageLayout final, bool clear);
VkAttachmentDescription kvfBuildSwapchainAttachmentDescription(VkSwapchainKHR swapchain, bool clear);

VkRenderPass kvfCreateRenderPass(VkDevice device, VkAttachmentDescription* attachments, size_t attachments_count, VkPipelineBindPoint bind_point);
void kvfDestroyRenderPass(VkDevice device, VkRenderPass renderpass);
void kvfBeginRenderPass(VkRenderPass pass, VkCommandBuffer cmd, VkFramebuffer framebuffer, VkExtent2D framebuffer_extent, VkClearValue* clears, size_t clears_count);

VkShaderModule kvfCreateShaderModule(VkDevice device, uint32_t* code, size_t size);
void kvfDestroyShaderModule(VkDevice device, VkShaderModule shader);

const char* kvfVerbaliseVkResult(VkResult result);

bool kvfIsStencilFormat(VkFormat format);
bool kvfIsDepthFormat(VkFormat format);
uint32_t kvfFormatSize(VkFormat format);
VkPipelineStageFlags kvfLayoutToAccessMask(VkImageLayout layout, bool is_destination);
VkPipelineStageFlags kvfAccessFlagsToPipelineStage(VkAccessFlags access_flags, VkPipelineStageFlags stage_flags);
VkFormat kvfFindSupportFormatInCandidates(VkDevice device, VkFormat* candidates, size_t candidates_count, VkImageTiling tiling, VkFormatFeatureFlags flags);

VkDescriptorSetLayout kvfCreateDescriptorSetLayout(VkDevice device, VkDescriptorSetLayoutBinding* bindings, size_t bindings_count);
void kvfDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout layout);

VkDescriptorSet kvfAllocateDescriptorSet(VkDevice device, VkDescriptorSetLayout layout);
void kvfUpdateStorageBufferToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorBufferInfo* info, uint32_t binding);
void kvfUpdateUniformBufferToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorBufferInfo* info, uint32_t binding);
void kvfUpdateImageToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorImageInfo* info, uint32_t binding);
VkWriteDescriptorSet kvfWriteStorageBufferToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorBufferInfo* info, uint32_t binding);
VkWriteDescriptorSet kvfWriteUniformBufferToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorBufferInfo* info, uint32_t binding);
VkWriteDescriptorSet kvfWriteImageToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorImageInfo* info, uint32_t binding);

void kvfResetDeviceDescriptorPools(VkDevice device);

VkPipelineLayout kvfCreatePipelineLayout(VkDevice device, VkDescriptorSetLayout* set_layouts, size_t set_layouts_count, VkPushConstantRange* pc, size_t pc_count);
void kvfDestroyPipelineLayout(VkDevice device, VkPipelineLayout layout);

KvfGraphicsPipelineBuilder* kvfCreateGPipelineBuilder();
void kvfDestroyGPipelineBuilder(KvfGraphicsPipelineBuilder* builder);

void kvfGPipelineBuilderReset(KvfGraphicsPipelineBuilder* builder);
void kvfGPipelineBuilderSetInputTopology(KvfGraphicsPipelineBuilder* builder, VkPrimitiveTopology topology);
void kvfGPipelineBuilderSetPolygonMode(KvfGraphicsPipelineBuilder* builder, VkPolygonMode polygon, float line_width);
void kvfGPipelineBuilderSetCullMode(KvfGraphicsPipelineBuilder* builder, VkCullModeFlags cull, VkFrontFace face);
void kvfGPipelineBuilderDisableBlending(KvfGraphicsPipelineBuilder* builder);
void kvfGPipelineBuilderEnableAdditiveBlending(KvfGraphicsPipelineBuilder* builder);
void kvfGPipelineBuilderEnableAlphaBlending(KvfGraphicsPipelineBuilder* builder);
void kvfGPipelineBuilderEnableDepthTest(KvfGraphicsPipelineBuilder* builder, VkCompareOp op, bool write_enabled);
void kvfGPipelineBuilderDisableDepthTest(KvfGraphicsPipelineBuilder* builder);
void kvfGPipelineBuilderSetVertexInputs(KvfGraphicsPipelineBuilder* builder, VkVertexInputBindingDescription binds, VkVertexInputAttributeDescription* attributes, size_t attributes_count);
void kvfGPipelineBuilderAddShaderStage(KvfGraphicsPipelineBuilder* builder, VkShaderStageFlagBits stage, VkShaderModule module, const char* entry);
void kvfGPipelineBuilderResetShaderStages(KvfGraphicsPipelineBuilder* builder);

VkPipeline kvfCreateGraphicsPipeline(VkDevice device, VkPipelineLayout layout, KvfGraphicsPipelineBuilder* builder, VkRenderPass pass);
void kvfDestroyPipeline(VkDevice device, VkPipeline pipeline);

#ifdef __cplusplus
}
#endif

#endif // KBZ_8_VULKAN_FRAMEWORK_H

/* ========================================== Implementation =========================================== */

#ifdef KVF_IMPLEMENTATION

#ifndef KVF_MALLOC
	#define KVF_MALLOC(x) malloc(x)
#endif
#ifndef KVF_REALLOC
	#define KVF_REALLOC(x, s) realloc(x, s)
#endif
#ifndef KVF_FREE
	#define KVF_FREE(x) free(x)
#endif
#ifndef KVF_ASSERT
	#include <assert.h>
	#define KVF_ASSERT(x) assert(x)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef KVF_DESCRIPTOR_POOL_CAPACITY
	#undef KVF_DESCRIPTOR_POOL_CAPACITY
#endif
#define KVF_DESCRIPTOR_POOL_CAPACITY 512

typedef struct
{
	int32_t graphics;
	int32_t present;
	int32_t compute;
} __KvfQueueFamilies;

typedef struct
{
	VkDescriptorPool pool;
	size_t capacity;
	size_t size;
} __KvfDescriptorPool;

typedef struct
{
	VkDevice device;
	VkPhysicalDevice physical;
	VkCommandPool cmd_pool;
	__KvfQueueFamilies queues;
	__KvfDescriptorPool* sets_pools;
	size_t sets_pools_size;
} __KvfDevice;

typedef struct
{
	VkSurfaceCapabilitiesKHR capabilities;
	VkSurfaceFormatKHR* formats;
	VkPresentModeKHR* presentModes;
	uint32_t formatsCount;
	uint32_t presentModesCount;
} __KvfSwapchainSupportInternal;

typedef struct
{
	__KvfSwapchainSupportInternal support;
	VkSwapchainKHR swapchain;
	VkExtent2D images_extent;
	VkFormat images_format;
	uint32_t images_count;
} __KvfSwapchain;

typedef struct
{
	VkFramebuffer framebuffer;
	VkExtent2D extent;
} __KvfFramebuffer;

struct KvfGraphicsPipelineBuilder
{
	VkPipelineShaderStageCreateInfo* shader_stages;
	VkPipelineVertexInputStateCreateInfo vertex_input_state;
	VkPipelineInputAssemblyStateCreateInfo input_assembly_state;
	VkPipelineTessellationStateCreateInfo tessellation_state;
	VkPipelineRasterizationStateCreateInfo rasterization_state;
	VkPipelineDepthStencilStateCreateInfo depth_stencil_state;
	VkPipelineColorBlendAttachmentState color_blend_attachment_state;
	size_t shader_stages_count;
};

// Dynamic arrays
__KvfDevice* __kvf_internal_devices = NULL;
size_t __kvf_internal_devices_size = 0;
size_t __kvf_internal_devices_capacity = 0;

__KvfSwapchain* __kvf_internal_swapchains = NULL;
size_t __kvf_internal_swapchains_size = 0;
size_t __kvf_internal_swapchains_capacity = 0;

__KvfFramebuffer* __kvf_internal_framebuffers = NULL;
size_t __kvf_internal_framebuffers_size = 0;
size_t __kvf_internal_framebuffers_capacity = 0;

#ifdef KVF_ENABLE_VALIDATION_LAYERS
	VkDebugUtilsMessengerEXT __kvf_debug_messenger = VK_NULL_HANDLE;
	char** __kvf_extra_layers = NULL;
	size_t __kvf_extra_layers_count = 0;
#endif

KvfErrorCallback __kvf_error_callback = NULL;
KvfErrorCallback __kvf_validation_error_callback = NULL;
KvfErrorCallback __kvf_validation_warning_callback = NULL;

void __kvfCheckVk(VkResult result, const char* function)
{
	if(result != VK_SUCCESS)
	{
		if(__kvf_error_callback != NULL)
		{
			char buffer[1024];
			snprintf(buffer, 1024, "KVF Vulkan error in '%s': %s", function, kvfVerbaliseVkResult(result));
			__kvf_error_callback(buffer);
			return;
		}
		fprintf(stderr, "KVF Vulkan error in '%s': %s\n", function, kvfVerbaliseVkResult(result));
		#ifndef KVF_NO_EXIT_ON_FAILURE
			exit(EXIT_FAILURE);
		#endif
	}
}

#undef __kvfCheckVk
#define __kvfCheckVk(res) __kvfCheckVk(res, __FUNCTION__)

void __kvfAddDeviceToArray(VkPhysicalDevice device, int32_t graphics_queue, int32_t present_queue)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	if(__kvf_internal_devices_size == __kvf_internal_devices_capacity)
	{
		// Resize the dynamic array if necessary
		__kvf_internal_devices_capacity += 2;
		__kvf_internal_devices = (__KvfDevice*)KVF_REALLOC(__kvf_internal_devices, __kvf_internal_devices_capacity * sizeof(__KvfDevice));
	}

	__kvf_internal_devices[__kvf_internal_devices_size].physical = device;
	__kvf_internal_devices[__kvf_internal_devices_size].queues.graphics = graphics_queue;
	__kvf_internal_devices[__kvf_internal_devices_size].queues.present = present_queue;
	__kvf_internal_devices_size++;
}

void __kvfCompleteDevice(VkPhysicalDevice physical, VkDevice device)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	KVF_ASSERT(physical != VK_NULL_HANDLE);

	__KvfDevice* kvf_device = NULL;

	for(size_t i = 0; i < __kvf_internal_devices_size; i++)
	{
		if(__kvf_internal_devices[i].physical == physical)
			kvf_device = &__kvf_internal_devices[i];
	}

	KVF_ASSERT(kvf_device != NULL);

	VkCommandPool pool;
	VkCommandPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	pool_info.queueFamilyIndex = kvf_device->queues.graphics;
	__kvfCheckVk(vkCreateCommandPool(device, &pool_info, NULL, &pool));

	kvf_device->device = device;
	kvf_device->cmd_pool = pool;
	kvf_device->sets_pools = NULL;
	kvf_device->sets_pools_size = 0;
}

void __kvfDestroyDescriptorPools(VkDevice device);

void __kvfDestroyDevice(VkDevice device)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	for(size_t i = 0; i < __kvf_internal_devices_size; i++)
	{
		if(__kvf_internal_devices[i].device == device)
		{
			vkDestroyCommandPool(device, __kvf_internal_devices[i].cmd_pool, NULL);
			__kvfDestroyDescriptorPools(device);
			vkDestroyDevice(device, NULL);
			// Shift the elements to fill the gap
			for(size_t j = i; j < __kvf_internal_devices_size - 1; j++)
				__kvf_internal_devices[j] = __kvf_internal_devices[j + 1];
			__kvf_internal_devices_size--;
			if(__kvf_internal_devices_size == 0)
			{
				KVF_FREE(__kvf_internal_devices);
				__kvf_internal_devices_capacity = 0;
			}
			return;
		}
    }
}

__KvfDevice* __kvfGetKvfDeviceFromVkPhysicalDevice(VkPhysicalDevice device)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	for(size_t i = 0; i < __kvf_internal_devices_size; i++)
	{
		if(__kvf_internal_devices[i].physical == device)
			return &__kvf_internal_devices[i];
	}
	return NULL;
}

__KvfDevice* __kvfGetKvfDeviceFromVkDevice(VkDevice device)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	for(size_t i = 0; i < __kvf_internal_devices_size; i++)
	{
		if(__kvf_internal_devices[i].device == device)
			return &__kvf_internal_devices[i];
	}
	return NULL;
}

void __kvfAddSwapchainToArray(VkSwapchainKHR swapchain, __KvfSwapchainSupportInternal support, VkFormat format, uint32_t images_count, VkExtent2D extent)
{
	KVF_ASSERT(swapchain != VK_NULL_HANDLE);
	if(__kvf_internal_swapchains_size == __kvf_internal_swapchains_capacity)
	{
		// Resize the dynamic array if necessary
		__kvf_internal_swapchains_capacity += 2;
		__kvf_internal_swapchains = (__KvfSwapchain*)KVF_REALLOC(__kvf_internal_swapchains, __kvf_internal_swapchains_capacity * sizeof(__KvfSwapchain));
	}

	__kvf_internal_swapchains[__kvf_internal_swapchains_size].swapchain = swapchain;
	__kvf_internal_swapchains[__kvf_internal_swapchains_size].support = support;
	__kvf_internal_swapchains[__kvf_internal_swapchains_size].images_format = format;
	__kvf_internal_swapchains[__kvf_internal_swapchains_size].images_count = images_count;
	__kvf_internal_swapchains[__kvf_internal_swapchains_size].images_extent = extent;
	__kvf_internal_swapchains_size++;
}

void __kvfDestroySwapchain(VkDevice device, VkSwapchainKHR swapchain)
{
	KVF_ASSERT(swapchain != VK_NULL_HANDLE);
	KVF_ASSERT(device != VK_NULL_HANDLE);

	for(size_t i = 0; i < __kvf_internal_swapchains_size; i++)
	{
		if(__kvf_internal_swapchains[i].swapchain == swapchain)
		{
			vkDestroySwapchainKHR(device, swapchain, NULL);
			// Shift the elements to fill the gap
			for(size_t j = i; j < __kvf_internal_swapchains_size - 1; j++)
				__kvf_internal_swapchains[j] = __kvf_internal_swapchains[j + 1];
			__kvf_internal_swapchains_size--;
			if(__kvf_internal_swapchains_size == 0)
			{
				KVF_FREE(__kvf_internal_swapchains);
				__kvf_internal_swapchains_capacity = 0;
			}
			return;
		}
    }
}

__KvfSwapchain* __kvfGetKvfSwapchainFromVkSwapchainKHR(VkSwapchainKHR swapchain)
{
	KVF_ASSERT(swapchain != VK_NULL_HANDLE);
	for(size_t i = 0; i < __kvf_internal_swapchains_size; i++)
	{
		if(__kvf_internal_swapchains[i].swapchain == swapchain)
			return &__kvf_internal_swapchains[i];
	}
	return NULL;
}

void __kvfAddFramebufferToArray(VkFramebuffer framebuffer, VkExtent2D extent)
{
	KVF_ASSERT(framebuffer != VK_NULL_HANDLE);
	if(__kvf_internal_framebuffers_size == __kvf_internal_framebuffers_capacity)
	{
		// Resize the dynamic array if necessary
		__kvf_internal_framebuffers_capacity += 2;
		__kvf_internal_framebuffers = (__KvfFramebuffer*)KVF_REALLOC(__kvf_internal_framebuffers, __kvf_internal_framebuffers_capacity * sizeof(__KvfFramebuffer));
	}

	__kvf_internal_framebuffers[__kvf_internal_framebuffers_size].framebuffer = framebuffer;
	__kvf_internal_framebuffers[__kvf_internal_framebuffers_size].extent = extent;
	__kvf_internal_framebuffers_size++;
}

void __kvfDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer)
{
	KVF_ASSERT(framebuffer != VK_NULL_HANDLE);
	KVF_ASSERT(device != VK_NULL_HANDLE);

	for(size_t i = 0; i < __kvf_internal_framebuffers_size; i++)
	{
		if(__kvf_internal_framebuffers[i].framebuffer == framebuffer)
		{
			vkDestroyFramebuffer(device, framebuffer, NULL);
			// Shift the elements to fill the gap
			for(size_t j = i; j < __kvf_internal_framebuffers_size - 1; j++)
				__kvf_internal_framebuffers[j] = __kvf_internal_framebuffers[j + 1];
			__kvf_internal_framebuffers_size--;
			if(__kvf_internal_framebuffers_size == 0)
			{
				KVF_FREE(__kvf_internal_framebuffers);
				__kvf_internal_framebuffers_capacity = 0;
			}
			return;
		}
    }
}

__KvfFramebuffer* __kvfGetKvfSwapchainFromVkFramebuffer(VkFramebuffer framebuffer)
{
	KVF_ASSERT(framebuffer != VK_NULL_HANDLE);
	for(size_t i = 0; i < __kvf_internal_framebuffers_size; i++)
	{
		if(__kvf_internal_framebuffers[i].framebuffer == framebuffer)
			return &__kvf_internal_framebuffers[i];
	}
	return NULL;
}

VkDescriptorPool __kvfDeviceCreateDescriptorPool(VkDevice device)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	__KvfDevice* kvf_device = __kvfGetKvfDeviceFromVkDevice(device);
	KVF_ASSERT(kvf_device != NULL);
	kvf_device->sets_pools_size++;
	kvf_device->sets_pools = (__KvfDescriptorPool*)KVF_REALLOC(kvf_device->sets_pools, kvf_device->sets_pools_size * sizeof(__KvfDescriptorPool));
	memset(&kvf_device->sets_pools[kvf_device->sets_pools_size - 1], 0, sizeof(__KvfDescriptorPool));

	VkDescriptorPoolSize pool_sizes[] = {
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1024 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1024 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1024 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1024 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1024 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1024 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1024 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1024 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1024 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1024 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.poolSizeCount = sizeof(pool_sizes) / sizeof(VkDescriptorPoolSize);
	pool_info.pPoolSizes = pool_sizes;
	pool_info.maxSets = KVF_DESCRIPTOR_POOL_CAPACITY;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

	__kvfCheckVk(vkCreateDescriptorPool(device, &pool_info, NULL, &kvf_device->sets_pools[kvf_device->sets_pools_size - 1].pool));
	kvf_device->sets_pools[kvf_device->sets_pools_size - 1].capacity = KVF_DESCRIPTOR_POOL_CAPACITY;
	return kvf_device->sets_pools[kvf_device->sets_pools_size - 1].pool;
}

void __kvfDestroyDescriptorPools(VkDevice device)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	__KvfDevice* kvf_device = __kvfGetKvfDeviceFromVkDevice(device);
	KVF_ASSERT(kvf_device != NULL);

	for(size_t i = 0; i < kvf_device->sets_pools_size; i++)
		vkDestroyDescriptorPool(device, kvf_device->sets_pools[i].pool, NULL);
	KVF_FREE(kvf_device->sets_pools);
	kvf_device->sets_pools_size = 0;
}

void kvfSetErrorCallback(KvfErrorCallback callback)
{
	__kvf_error_callback = callback;
}

void kvfSetValidationErrorCallback(KvfErrorCallback callback)
{
	__kvf_validation_error_callback = callback;
}

void kvfSetValidationWarningCallback(KvfErrorCallback callback)
{
	__kvf_validation_warning_callback = callback;
}

bool kvfIsStencilFormat(VkFormat format)
{
	switch(format)
	{
		case VK_FORMAT_D32_SFLOAT_S8_UINT:
		case VK_FORMAT_D24_UNORM_S8_UINT:
			return true;

		default: return false;
	}
}

bool kvfIsDepthFormat(VkFormat format)
{
	switch(format)
	{
		case VK_FORMAT_D16_UNORM:
		case VK_FORMAT_D32_SFLOAT:
		case VK_FORMAT_D32_SFLOAT_S8_UINT:
		case VK_FORMAT_D24_UNORM_S8_UINT:
		case VK_FORMAT_D16_UNORM_S8_UINT:
			return true;

		default: return false;
	}
}

VkPipelineStageFlags kvfLayoutToAccessMask(VkImageLayout layout, bool is_destination)
{
	VkPipelineStageFlags access_mask = 0;

	switch(layout)
	{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			if(is_destination)
				KVF_ASSERT(false && "Vulkan : the new layout used in a transition must not be VK_IMAGE_LAYOUT_UNDEFINED");
		break;
		case VK_IMAGE_LAYOUT_GENERAL: access_mask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT; break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: access_mask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT; break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
			access_mask = VK_ACCESS_SHADER_READ_BIT; // VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: access_mask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT; break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: access_mask = VK_ACCESS_TRANSFER_READ_BIT; break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: access_mask = VK_ACCESS_TRANSFER_WRITE_BIT; break;
		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			if(!is_destination)
				access_mask = VK_ACCESS_HOST_WRITE_BIT;
			else
				KVF_ASSERT(false && "Vulkan : the new layout used in a transition must not be VK_IMAGE_LAYOUT_PREINITIALIZED");
		break;
		case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL: access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT; break;
		case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL: access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT; break;
		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: access_mask = VK_ACCESS_MEMORY_READ_BIT; break;

		default: KVF_ASSERT(false && "Vulkan : unexpected image layout"); break;
	}

	return access_mask;
}

VkPipelineStageFlags kvfAccessFlagsToPipelineStage(VkAccessFlags access_flags, VkPipelineStageFlags stage_flags)
{
	VkPipelineStageFlags stages = 0;

	while(access_flags != 0)
	{
		VkAccessFlagBits _access_flag = (VkAccessFlagBits)(access_flags & (~(access_flags - 1)));
		if(_access_flag == 0 || (_access_flag & (_access_flag - 1)) != 0)
			KVF_ASSERT(false && "Vulkan : an error has been caught during access flag to pipeline stage operation");
		access_flags &= ~_access_flag;

		switch(_access_flag)
		{
			case VK_ACCESS_INDIRECT_COMMAND_READ_BIT: stages |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT; break;
			case VK_ACCESS_INDEX_READ_BIT: stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; break;
			case VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT: stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; break;
			case VK_ACCESS_UNIFORM_READ_BIT: stages |= stage_flags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
			case VK_ACCESS_INPUT_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT; break;
			case VK_ACCESS_SHADER_READ_BIT: stages |= stage_flags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
			case VK_ACCESS_SHADER_WRITE_BIT: stages |= stage_flags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
			case VK_ACCESS_COLOR_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; break;
			case VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT: stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; break;
			case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT; break;
			case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT: stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT; break;
			case VK_ACCESS_TRANSFER_READ_BIT: stages |= VK_PIPELINE_STAGE_TRANSFER_BIT; break;
			case VK_ACCESS_TRANSFER_WRITE_BIT: stages |= VK_PIPELINE_STAGE_TRANSFER_BIT; break;
			case VK_ACCESS_HOST_READ_BIT: stages |= VK_PIPELINE_STAGE_HOST_BIT; break;
			case VK_ACCESS_HOST_WRITE_BIT: stages |= VK_PIPELINE_STAGE_HOST_BIT; break;
			case VK_ACCESS_MEMORY_READ_BIT: break;
			case VK_ACCESS_MEMORY_WRITE_BIT: break;

			default: KVF_ASSERT(false && "Vulkan : unknown access flag"); break;
		}
	}
	return stages;
}

VkFormat kvfFindSupportFormatInCandidates(VkDevice device, VkFormat* candidates, size_t candidates_count, VkImageTiling tiling, VkFormatFeatureFlags flags)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	__KvfDevice* kvf_device = __kvfGetKvfDeviceFromVkDevice(device);
	KVF_ASSERT(kvf_device != NULL);
	for(size_t i = 0; i < candidates_count; i++)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(kvf_device->physical, candidates[i], &props);
		if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & flags) == flags)
			return candidates[i];
		else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & flags) == flags)
			return candidates[i];
	}

	KVF_ASSERT(false && "Vulkan : failed to find image format");
	return VK_FORMAT_R8G8B8A8_SRGB; // just to avoir warning
}

uint32_t kvfFormatSize(VkFormat format)
{
	switch(format)
	{
		case VK_FORMAT_UNDEFINED: return 0;
		case VK_FORMAT_R4G4_UNORM_PACK8: return 1;
		case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return 2;
		case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return 2;
		case VK_FORMAT_R5G6B5_UNORM_PACK16: return 2;
		case VK_FORMAT_B5G6R5_UNORM_PACK16: return 2;
		case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return 2;
		case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return 2;
		case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return 2;
		case VK_FORMAT_R8_UNORM: return 1;
		case VK_FORMAT_R8_SNORM: return 1;
		case VK_FORMAT_R8_USCALED: return 1;
		case VK_FORMAT_R8_SSCALED: return 1;
		case VK_FORMAT_R8_UINT: return 1;
		case VK_FORMAT_R8_SINT: return 1;
		case VK_FORMAT_R8_SRGB: return 1;
		case VK_FORMAT_R8G8_UNORM: return 2;
		case VK_FORMAT_R8G8_SNORM: return 2;
		case VK_FORMAT_R8G8_USCALED: return 2;
		case VK_FORMAT_R8G8_SSCALED: return 2;
		case VK_FORMAT_R8G8_UINT: return 2;
		case VK_FORMAT_R8G8_SINT: return 2;
		case VK_FORMAT_R8G8_SRGB: return 2;
		case VK_FORMAT_R8G8B8_UNORM: return 3;
		case VK_FORMAT_R8G8B8_SNORM: return 3;
		case VK_FORMAT_R8G8B8_USCALED: return 3;
		case VK_FORMAT_R8G8B8_SSCALED: return 3;
		case VK_FORMAT_R8G8B8_UINT: return 3;
		case VK_FORMAT_R8G8B8_SINT: return 3;
		case VK_FORMAT_R8G8B8_SRGB: return 3;
		case VK_FORMAT_B8G8R8_UNORM: return 3;
		case VK_FORMAT_B8G8R8_SNORM: return 3;
		case VK_FORMAT_B8G8R8_USCALED: return 3;
		case VK_FORMAT_B8G8R8_SSCALED: return 3;
		case VK_FORMAT_B8G8R8_UINT: return 3;
		case VK_FORMAT_B8G8R8_SINT: return 3;
		case VK_FORMAT_B8G8R8_SRGB: return 3;
		case VK_FORMAT_R8G8B8A8_UNORM: return 4;
		case VK_FORMAT_R8G8B8A8_SNORM: return 4;
		case VK_FORMAT_R8G8B8A8_USCALED: return 4;
		case VK_FORMAT_R8G8B8A8_SSCALED: return 4;
		case VK_FORMAT_R8G8B8A8_UINT: return 4;
		case VK_FORMAT_R8G8B8A8_SINT: return 4;
		case VK_FORMAT_R8G8B8A8_SRGB: return 4;
		case VK_FORMAT_B8G8R8A8_UNORM: return 4;
		case VK_FORMAT_B8G8R8A8_SNORM: return 4;
		case VK_FORMAT_B8G8R8A8_USCALED: return 4;
		case VK_FORMAT_B8G8R8A8_SSCALED: return 4;
		case VK_FORMAT_B8G8R8A8_UINT: return 4;
		case VK_FORMAT_B8G8R8A8_SINT: return 4;
		case VK_FORMAT_B8G8R8A8_SRGB: return 4;
		case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return 4;
		case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return 4;
		case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return 4;
		case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return 4;
		case VK_FORMAT_A8B8G8R8_UINT_PACK32: return 4;
		case VK_FORMAT_A8B8G8R8_SINT_PACK32: return 4;
		case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return 4;
		case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return 4;
		case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return 4;
		case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return 4;
		case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return 4;
		case VK_FORMAT_A2R10G10B10_UINT_PACK32: return 4;
		case VK_FORMAT_A2R10G10B10_SINT_PACK32: return 4;
		case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return 4;
		case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return 4;
		case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return 4;
		case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return 4;
		case VK_FORMAT_A2B10G10R10_UINT_PACK32: return 4;
		case VK_FORMAT_A2B10G10R10_SINT_PACK32: return 4;
		case VK_FORMAT_R16_UNORM: return 2;
		case VK_FORMAT_R16_SNORM: return 2;
		case VK_FORMAT_R16_USCALED: return 2;
		case VK_FORMAT_R16_SSCALED: return 2;
		case VK_FORMAT_R16_UINT: return 2;
		case VK_FORMAT_R16_SINT: return 2;
		case VK_FORMAT_R16_SFLOAT: return 2;
		case VK_FORMAT_R16G16_UNORM: return 4;
		case VK_FORMAT_R16G16_SNORM: return 4;
		case VK_FORMAT_R16G16_USCALED: return 4;
		case VK_FORMAT_R16G16_SSCALED: return 4;
		case VK_FORMAT_R16G16_UINT: return 4;
		case VK_FORMAT_R16G16_SINT: return 4;
		case VK_FORMAT_R16G16_SFLOAT: return 4;
		case VK_FORMAT_R16G16B16_UNORM: return 6;
		case VK_FORMAT_R16G16B16_SNORM: return 6;
		case VK_FORMAT_R16G16B16_USCALED: return 6;
		case VK_FORMAT_R16G16B16_SSCALED: return 6;
		case VK_FORMAT_R16G16B16_UINT: return 6;
		case VK_FORMAT_R16G16B16_SINT: return 6;
		case VK_FORMAT_R16G16B16_SFLOAT: return 6;
		case VK_FORMAT_R16G16B16A16_UNORM: return 8;
		case VK_FORMAT_R16G16B16A16_SNORM: return 8;
		case VK_FORMAT_R16G16B16A16_USCALED: return 8;
		case VK_FORMAT_R16G16B16A16_SSCALED: return 8;
		case VK_FORMAT_R16G16B16A16_UINT: return 8;
		case VK_FORMAT_R16G16B16A16_SINT: return 8;
		case VK_FORMAT_R16G16B16A16_SFLOAT: return 8;
		case VK_FORMAT_R32_UINT: return 4;
		case VK_FORMAT_R32_SINT: return 4;
		case VK_FORMAT_R32_SFLOAT: return 4;
		case VK_FORMAT_R32G32_UINT: return 8;
		case VK_FORMAT_R32G32_SINT: return 8;
		case VK_FORMAT_R32G32_SFLOAT: return 8;
		case VK_FORMAT_R32G32B32_UINT: return 12;
		case VK_FORMAT_R32G32B32_SINT: return 12;
		case VK_FORMAT_R32G32B32_SFLOAT: return 12;
		case VK_FORMAT_R32G32B32A32_UINT: return 16;
		case VK_FORMAT_R32G32B32A32_SINT: return 16;
		case VK_FORMAT_R32G32B32A32_SFLOAT: return 16;
		case VK_FORMAT_R64_UINT: return 8;
		case VK_FORMAT_R64_SINT: return 8;
		case VK_FORMAT_R64_SFLOAT: return 8;
		case VK_FORMAT_R64G64_UINT: return 16;
		case VK_FORMAT_R64G64_SINT: return 16;
		case VK_FORMAT_R64G64_SFLOAT: return 16;
		case VK_FORMAT_R64G64B64_UINT: return 24;
		case VK_FORMAT_R64G64B64_SINT: return 24;
		case VK_FORMAT_R64G64B64_SFLOAT: return 24;
		case VK_FORMAT_R64G64B64A64_UINT: return 32;
		case VK_FORMAT_R64G64B64A64_SINT: return 32;
		case VK_FORMAT_R64G64B64A64_SFLOAT: return 32;
		case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return 4;
		case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return 4;

		default: return 0;
	}
}

const char* kvfVerbaliseVkResult(VkResult result)
{
	switch(result)
	{
		case VK_SUCCESS: return "Success";
		case VK_NOT_READY: return "A fence or query has not yet completed";
		case VK_TIMEOUT: return "A wait operation has not completed in the specified time";
		case VK_EVENT_SET: return "An event is signaled";
		case VK_EVENT_RESET: return "An event is unsignaled";
		case VK_INCOMPLETE: return "A return array was too small for the result";
		case VK_ERROR_OUT_OF_HOST_MEMORY: return "A host memory allocation has failed";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "A device memory allocation has failed";
		case VK_ERROR_INITIALIZATION_FAILED: return "Initialization of an object could not be completed for implementation-specific reasons";
		case VK_ERROR_DEVICE_LOST: return "The logical or physical device has been lost";
		case VK_ERROR_MEMORY_MAP_FAILED: return "Mapping of a memory object has failed";
		case VK_ERROR_LAYER_NOT_PRESENT: return "A requested layer is not present or could not be loaded";
		case VK_ERROR_EXTENSION_NOT_PRESENT: return "A requested extension is not supported";
		case VK_ERROR_FEATURE_NOT_PRESENT: return "A requested feature is not supported";
		case VK_ERROR_INCOMPATIBLE_DRIVER: return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
		case VK_ERROR_TOO_MANY_OBJECTS: return "Too many objects of the type have already been created";
		case VK_ERROR_FORMAT_NOT_SUPPORTED: return "A requested format is not supported on this device";
		case VK_ERROR_SURFACE_LOST_KHR: return "A surface is no longer available";
		case VK_SUBOPTIMAL_KHR: return "A swapchain no longer matches the surface properties exactly, but can still be used";
		case VK_ERROR_OUT_OF_DATE_KHR: return "A surface has changed in such a way that it is no longer compatible with the swapchain";
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "The display used by a swapchain does not use the same presentable image layout";
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
		case VK_ERROR_VALIDATION_FAILED_EXT: return "A validation layer found an error";

		default: return "Unknown Vulkan error";
	}
	return NULL; // just to avoid warnings
}

#ifdef KVF_ENABLE_VALIDATION_LAYERS
	bool __kvfCheckValidationLayerSupport()
	{
		uint32_t layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, NULL);
		VkLayerProperties* available_layers = (VkLayerProperties*)KVF_MALLOC(sizeof(VkLayerProperties) * layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers);
		for(size_t i = 0; i < __kvf_extra_layers_count; i++)
		{
			bool found = false;
			for(size_t j = 0; j < layer_count; j++)
			{
				if(strcmp(available_layers[j].layerName, __kvf_extra_layers[i]) == 0)
				{
					found = true;
					break;
				}
			}
			if(!found)
			{
				KVF_FREE(available_layers);
				return false;
			}
		}
		KVF_FREE(available_layers);
		return true;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL __kvfDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			if(__kvf_validation_error_callback != NULL)
			{
				char buffer[4096];
				snprintf(buffer, 4096, "KVF Vulkan validation error : %s", pCallbackData->pMessage);
				__kvf_validation_error_callback(buffer);
				return VK_FALSE;
			}
			fprintf(stderr, "\nKVF Vulkan validation error : %s\n", pCallbackData->pMessage);
		}
		else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			if(__kvf_validation_warning_callback != NULL)
			{
				char buffer[4096];
				snprintf(buffer, 4096, "KVF Vulkan validation warning : %s", pCallbackData->pMessage);
				__kvf_validation_warning_callback(buffer);
				return VK_FALSE;
			}
			fprintf(stderr, "\nKVF Vulkan validation warning : %s\n", pCallbackData->pMessage);
		}
		return VK_FALSE;
	}

	void __kvfPopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* create_info)
	{
		create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		create_info->pfnUserCallback = __kvfDebugCallback;
	}

	VkResult __kvfCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, VkDebugUtilsMessengerEXT* messenger)
	{
		PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		return func ? func(instance, create_info, NULL, messenger) : VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void __kvfInitValidationLayers(VkInstance instance)
	{
		uint32_t extension_count;
		vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);
		VkExtensionProperties* extensions = (VkExtensionProperties*)KVF_MALLOC(extension_count * sizeof(VkExtensionProperties));
		vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);
		bool extension_found = false;
		for(uint32_t i = 0; i < extension_count; i++)
		{
			if(strcmp(extensions[i].extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
			{
				extension_found = true;
				break;
			}
		}
		if(!extension_found)
		{
			if(__kvf_validation_warning_callback != NULL)
			{
				char buffer[1024];
				snprintf(buffer, 1024, "KVF Vulkan warning: %s is not present; cannot enable validation layers", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
				__kvf_validation_warning_callback(buffer);
				return;
			}
			printf("KVF Vulkan warning: %s is not present; cannot enable validation layers", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			KVF_FREE(extensions);
			return;
		}
		VkDebugUtilsMessengerCreateInfoEXT create_info = {};
		__kvfPopulateDebugMessengerCreateInfo(&create_info);
		__kvfCheckVk(__kvfCreateDebugUtilsMessengerEXT(instance, &create_info, &__kvf_debug_messenger));
	}

	void __kvfDestroyDebugUtilsMessengerEXT(VkInstance instance)
	{
		PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if(func)
			func(instance, __kvf_debug_messenger, NULL);
	}
#endif // KVF_ENABLE_VALIDATION_LAYERS

void kvfAddLayer(const char* layer)
{
	#ifdef KVF_ENABLE_VALIDATION_LAYERS
		__kvf_extra_layers = (char**)KVF_REALLOC(__kvf_extra_layers, sizeof(char*) * (__kvf_extra_layers_count + 1));
		KVF_ASSERT(__kvf_extra_layers != NULL);
		__kvf_extra_layers[__kvf_extra_layers_count] = (char*)KVF_MALLOC(strlen(layer) + 1);
		KVF_ASSERT(__kvf_extra_layers[__kvf_extra_layers_count] != NULL);
		strcpy(__kvf_extra_layers[__kvf_extra_layers_count], layer);
		__kvf_extra_layers_count++;
	#else
		if(__kvf_validation_error_callback != NULL)
		{
			char buffer[4096];
			snprintf(buffer, 4096, "KVF Vulkan validation error : cannot add extra layers, validation layers are not enabled. Try adding #define KVF_ENABLE_VALIDATION_LAYERS");
			__kvf_validation_error_callback(buffer);
			return;
		}
		fprintf(stderr, "KVF Vulkan validation error : cannot add extra layers, validation layers are not enabled. Try adding #define KVF_ENABLE_VALIDATION_LAYERS");
	#endif
}

VkInstance kvfCreateInstance(const char** extensions_enabled, uint32_t extensions_count)
{
	VkInstance instance = VK_NULL_HANDLE;

	VkInstanceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = NULL;
	create_info.flags = 0;
	create_info.enabledExtensionCount = extensions_count;
	create_info.ppEnabledExtensionNames = extensions_enabled;
	create_info.enabledLayerCount = 0;
	create_info.ppEnabledLayerNames = NULL;
	create_info.pNext = NULL;

#ifdef KVF_ENABLE_VALIDATION_LAYERS
	kvfAddLayer("VK_LAYER_KHRONOS_validation");
	const char** new_extension_set = NULL;
	if(__kvfCheckValidationLayerSupport())
	{
		VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};
		__kvfPopulateDebugMessengerCreateInfo(&debug_create_info);
		new_extension_set = (const char**)KVF_MALLOC(sizeof(char*) * (extensions_count + 1));
		memcpy(new_extension_set, extensions_enabled, sizeof(char*) * extensions_count);
		new_extension_set[extensions_count] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

		create_info.enabledExtensionCount = extensions_count + 1;
		create_info.ppEnabledExtensionNames = new_extension_set;
		create_info.enabledLayerCount = __kvf_extra_layers_count;
		create_info.ppEnabledLayerNames = (const char* const*)__kvf_extra_layers;
		create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
	}
#endif

	__kvfCheckVk(vkCreateInstance(&create_info, NULL, &instance));
#ifdef KVF_ENABLE_VALIDATION_LAYERS
	KVF_FREE(new_extension_set);
	__kvfInitValidationLayers(instance);
#endif
	return instance;
}

void kvfDestroyInstance(VkInstance instance)
{
	if(instance == VK_NULL_HANDLE)
		return;
#ifdef KVF_ENABLE_VALIDATION_LAYERS
	__kvfDestroyDebugUtilsMessengerEXT(instance);
	for(size_t i = 0; i < __kvf_extra_layers_count; i++)
		KVF_FREE(__kvf_extra_layers[i]);
	KVF_FREE(__kvf_extra_layers);
	__kvf_extra_layers_count = 0;
#endif
	vkDestroyInstance(instance, NULL);
}

VkPhysicalDevice kvfPickFirstPhysicalDevice(VkInstance instance)
{
	uint32_t device_count;
	VkPhysicalDevice* devices = NULL;
	VkPhysicalDevice chosen_one = VK_NULL_HANDLE;

	KVF_ASSERT(instance != VK_NULL_HANDLE);
	
	vkEnumeratePhysicalDevices(instance, &device_count, NULL);
	devices = (VkPhysicalDevice*)KVF_MALLOC(sizeof(VkPhysicalDevice) * device_count + 1);
	vkEnumeratePhysicalDevices(instance, &device_count, devices);
	chosen_one = devices[0];
	KVF_FREE(devices);
	return chosen_one;
}

__KvfQueueFamilies __kvfFindQueueFamilies(VkPhysicalDevice physical, VkSurfaceKHR surface)
{
	__KvfQueueFamilies queues = { -1, -1, -1 };
	uint32_t queue_family_count;
	vkGetPhysicalDeviceQueueFamilyProperties(physical, &queue_family_count, NULL);
	VkQueueFamilyProperties* queue_families = (VkQueueFamilyProperties*)KVF_MALLOC(sizeof(VkQueueFamilyProperties) * queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(physical, &queue_family_count, queue_families);

	for(int i = 0; i < queue_family_count; i++)
	{
		// try to find a queue family index that supports compute but not graphics
		if(queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT && (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)
			queues.compute = i;
		else if(queues.compute != -1 && queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) // else just find a compute queue
			queues.compute = i;
		if(queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			queues.graphics = i;
		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physical, i, surface, &present_support);
		if(present_support)
			queues.present = i;

		if(queues.graphics != -1 && queues.present != -1 && queues.compute != -1)
			break;
	}
	KVF_FREE(queue_families);
	return queues;
}

VkPhysicalDevice kvfPickGoodDefaultPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
	const char* extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	return kvfPickGoodPhysicalDevice(instance, surface, extensions, sizeof(extensions) / sizeof(extensions[0]));
}

int32_t __kvfScorePhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface, const char** deviceExtensions, uint32_t deviceExtensionsCount)
{
	/* Check Extensions Support */
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, NULL);
	VkExtensionProperties* props = (VkExtensionProperties*)KVF_MALLOC(sizeof(VkExtensionProperties) * extension_count + 1);
	vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, props);

	bool are_there_required_device_extensions = true;
	for(int j = 0; j < deviceExtensionsCount; j++)
	{
		bool is_there_extension = false;
		for(int k = 0; k < extension_count; k++)
		{
			if(strcmp(deviceExtensions[j], props[k].extensionName) == 0)
			{
				is_there_extension = true;
				break;
			}
		}
		if(is_there_extension == false)
		{
			are_there_required_device_extensions = false;
			break;
		}
	}
	KVF_FREE(props);
	if(are_there_required_device_extensions == false)
		return -1;

	/* Check Queue Families Support */
	__KvfQueueFamilies queues = __kvfFindQueueFamilies(device, surface);
	if(queues.graphics == -1 || queues.present == -1)
		return -1;

	/* Check Surface Formats Counts */
	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, NULL);
	if(format_count == 0)
		return -1;

	VkPhysicalDeviceProperties device_props;
	vkGetPhysicalDeviceProperties(device, &device_props);

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures(device, &device_features);

	int32_t score = -1;
	if(device_props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	if(!device_features.geometryShader)
		return -1;

	score += device_props.limits.maxImageDimension2D;
	score += device_props.limits.maxBoundDescriptorSets;

	return score;
}

VkPhysicalDevice kvfPickGoodPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const char** deviceExtensions, uint32_t deviceExtensionsCount)
{
	VkPhysicalDevice* devices = NULL;
	VkPhysicalDevice chosen_one = VK_NULL_HANDLE;
	uint32_t device_count;
	int32_t best_device_score = -1;

	KVF_ASSERT(instance != VK_NULL_HANDLE);
	KVF_ASSERT(surface != VK_NULL_HANDLE);

	vkEnumeratePhysicalDevices(instance, &device_count, NULL);
	devices = (VkPhysicalDevice*)KVF_MALLOC(sizeof(VkPhysicalDevice) * device_count + 1);
	vkEnumeratePhysicalDevices(instance, &device_count, devices);

	for(int i = 0; i < device_count; i++)
	{
		int32_t current_device_score = __kvfScorePhysicalDevice(devices[i], surface, deviceExtensions, deviceExtensionsCount);
		if(current_device_score > best_device_score)
		{
			best_device_score = current_device_score;
			chosen_one = devices[i];
		}
	}
	KVF_FREE(devices);
	if(chosen_one != VK_NULL_HANDLE)
	{
		__KvfQueueFamilies queues = __kvfFindQueueFamilies(chosen_one, surface);
		__kvfAddDeviceToArray(chosen_one, queues.graphics, queues.present);
		return chosen_one;
	}
	return VK_NULL_HANDLE;
}

VkDevice kvfCreateDefaultDevice(VkPhysicalDevice physical)
{
	const char* extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	return kvfCreateDevice(physical, extensions, sizeof(extensions) / sizeof(extensions[0]));
}

VkDevice kvfCreateDevice(VkPhysicalDevice physical, const char** extensions, uint32_t extensions_count)
{
	const float queue_priority = 1.0f;

	__KvfDevice* kvfdevice = __kvfGetKvfDeviceFromVkPhysicalDevice(physical);

	KVF_ASSERT(kvfdevice != NULL);
	KVF_ASSERT(kvfdevice->queues.graphics != -1);
	KVF_ASSERT(kvfdevice->queues.present != -1);

	VkDeviceQueueCreateInfo queue_create_info[2];
	queue_create_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info[0].queueFamilyIndex = kvfdevice->queues.graphics;
	queue_create_info[0].queueCount = 1;
	queue_create_info[0].pQueuePriorities = &queue_priority;
	queue_create_info[0].flags = 0;
	queue_create_info[0].pNext = NULL;
	queue_create_info[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info[1].queueFamilyIndex = kvfdevice->queues.present;
	queue_create_info[1].queueCount = 1;
	queue_create_info[1].pQueuePriorities = &queue_priority;
	queue_create_info[1].flags = 0;
	queue_create_info[1].pNext = NULL;

	VkPhysicalDeviceFeatures device_features = { VK_FALSE };

	VkDeviceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = (kvfdevice->queues.graphics == kvfdevice->queues.present ? 1 : 2);
	createInfo.pQueueCreateInfos = queue_create_info;
	createInfo.pEnabledFeatures = &device_features;
	createInfo.enabledExtensionCount = extensions_count;
	createInfo.ppEnabledExtensionNames = extensions;
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = NULL;
	createInfo.flags = 0;
	createInfo.pNext = NULL;

	VkDevice device;
	__kvfCheckVk(vkCreateDevice(physical, &createInfo, NULL, &device));
	__kvfCompleteDevice(physical, device);

	return device;
}

void kvfDestroyDevice(VkDevice device)
{
	if(device == VK_NULL_HANDLE)
		return;
	__kvfDestroyDevice(device);
}

VkQueue kvfGetDeviceQueue(VkDevice device, KvfQueueType queue)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	__KvfDevice* kvfdevice = __kvfGetKvfDeviceFromVkDevice(device);
	KVF_ASSERT(kvfdevice != NULL);
	VkQueue vk_queue = VK_NULL_HANDLE;
	if(queue == KVF_GRAPHICS_QUEUE)
		vkGetDeviceQueue(device, kvfdevice->queues.graphics, 0, &vk_queue);
	else if(queue == KVF_PRESENT_QUEUE)
		vkGetDeviceQueue(device, kvfdevice->queues.present, 0, &vk_queue);
	else if(queue == KVF_COMPUTE_QUEUE)
		vkGetDeviceQueue(device, kvfdevice->queues.compute, 0, &vk_queue);
	return vk_queue;
}

uint32_t kvfGetDeviceQueueFamily(VkDevice device, KvfQueueType queue)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	__KvfDevice* kvfdevice = __kvfGetKvfDeviceFromVkDevice(device);
	KVF_ASSERT(kvfdevice != NULL);
	VkQueue vk_queue = VK_NULL_HANDLE;
	if(queue == KVF_GRAPHICS_QUEUE)
		return kvfdevice->queues.graphics;
	else if(queue == KVF_PRESENT_QUEUE)
		return kvfdevice->queues.present;
	else if(queue == KVF_COMPUTE_QUEUE)
		return kvfdevice->queues.compute;
	KVF_ASSERT(false && "invalid queue");
	return 0;
}

bool kvfQueuePresentKHR(VkDevice device, VkSemaphore wait, VkSwapchainKHR swapchain, uint32_t image_index)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = &wait;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &swapchain;
	present_info.pImageIndices = &image_index;
	VkResult result = vkQueuePresentKHR(kvfGetDeviceQueue(device, KVF_PRESENT_QUEUE), &present_info);
	if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		return false;
	else
		__kvfCheckVk(result);
	return true;
}

VkFence kvfCreateFence(VkDevice device)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	VkFenceCreateInfo fence_info = {};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	VkFence fence;
	__kvfCheckVk(vkCreateFence(device, &fence_info, NULL, &fence));
	return fence;
}

void kvfWaitForFence(VkDevice device, VkFence fence)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	KVF_ASSERT(fence != VK_NULL_HANDLE);
	vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
}

void kvfDestroyFence(VkDevice device, VkFence fence)
{
	if(fence == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroyFence(device, fence, NULL);
}

VkSemaphore kvfCreateSemaphore(VkDevice device)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	VkSemaphoreCreateInfo semaphore_info = {};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	VkSemaphore semaphore;
	__kvfCheckVk(vkCreateSemaphore(device, &semaphore_info, NULL, &semaphore));
	return semaphore;
}

void kvfDestroySemaphore(VkDevice device, VkSemaphore semaphore)
{
	if(semaphore == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroySemaphore(device, semaphore, NULL);
}

__KvfSwapchainSupportInternal __kvfQuerySwapchainSupport(VkPhysicalDevice physical, VkSurfaceKHR surface)
{
	__KvfSwapchainSupportInternal support;

	__kvfCheckVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical, surface, &support.capabilities));

	vkGetPhysicalDeviceSurfaceFormatsKHR(physical, surface, &support.formatsCount, NULL);
	if(support.formatsCount != 0)
	{
		support.formats = (VkSurfaceFormatKHR*)KVF_MALLOC(sizeof(VkSurfaceFormatKHR) * support.formatsCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physical, surface, &support.formatsCount, support.formats);
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(physical, surface, &support.presentModesCount, NULL);
	if(support.presentModesCount != 0)
	{
		support.presentModes = (VkPresentModeKHR*)KVF_MALLOC(sizeof(VkPresentModeKHR) * support.presentModesCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physical, surface, &support.presentModesCount, support.presentModes);
	}
	return support;
}

VkSurfaceFormatKHR __kvfChooseSwapSurfaceFormat(__KvfSwapchainSupportInternal* support)
{
	for(int i = 0; i < support->formatsCount; i++)
	{
		if(support->formats[i].format == VK_FORMAT_R8G8B8A8_SRGB && support->formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return support->formats[i];
	}
	return support->formats[0];
}

VkPresentModeKHR __kvfChooseSwapPresentMode(__KvfSwapchainSupportInternal* support, bool tryVsync)
{
	if(tryVsync == false)
		return VK_PRESENT_MODE_IMMEDIATE_KHR;
	for(int i = 0; i < support->presentModesCount; i++)
	{
		if(support->presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			return support->presentModes[i];
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

uint32_t __kvfClamp(uint32_t i, uint32_t min, uint32_t max)
{
	const uint32_t t = i < min ? min : i;
	return t > max ? max : t;
}

VkSwapchainKHR kvfCreateSwapchainKHR(VkDevice device, VkPhysicalDevice physical, VkSurfaceKHR surface, VkExtent2D extent, bool tryVsync)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	VkSwapchainKHR swapchain;
	__KvfSwapchainSupportInternal support = __kvfQuerySwapchainSupport(physical, surface);

	VkSurfaceFormatKHR surfaceFormat = __kvfChooseSwapSurfaceFormat(&support);
	VkPresentModeKHR presentMode = __kvfChooseSwapPresentMode(&support, tryVsync);

	uint32_t imageCount = support.capabilities.minImageCount + 1;
	if(support.capabilities.maxImageCount > 0 && imageCount > support.capabilities.maxImageCount)
		imageCount = support.capabilities.maxImageCount;

	__KvfDevice* kvfdevice = __kvfGetKvfDeviceFromVkDevice(device);
	KVF_ASSERT(kvfdevice != NULL);

	uint32_t queueFamilyIndices[] = { (uint32_t)kvfdevice->queues.graphics, (uint32_t)kvfdevice->queues.present };

	if(support.capabilities.currentExtent.width != UINT32_MAX)
		extent = support.capabilities.currentExtent;
	else
	{
		extent.width = __kvfClamp(extent.width, support.capabilities.minImageExtent.width, support.capabilities.maxImageExtent.width);
		extent.height = __kvfClamp(extent.height, support.capabilities.minImageExtent.height, support.capabilities.maxImageExtent.height);
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = support.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if(kvfdevice->queues.graphics != kvfdevice->queues.present)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

	__kvfCheckVk(vkCreateSwapchainKHR(device, &createInfo, NULL, &swapchain));

	uint32_t images_count;
	vkGetSwapchainImagesKHR(device, swapchain, (uint32_t*)&images_count, NULL);

	__kvfAddSwapchainToArray(swapchain, support, surfaceFormat.format, images_count, extent);

	return swapchain;
}

VkFormat kvfGetSwapchainImagesFormat(VkSwapchainKHR swapchain)
{
	__KvfSwapchain* kvf_swapchain = __kvfGetKvfSwapchainFromVkSwapchainKHR(swapchain);
	KVF_ASSERT(kvf_swapchain != NULL);
	return kvf_swapchain->images_format;
}

uint32_t kvfGetSwapchainImagesCount(VkSwapchainKHR swapchain)
{
	__KvfSwapchain* kvf_swapchain = __kvfGetKvfSwapchainFromVkSwapchainKHR(swapchain);
	KVF_ASSERT(kvf_swapchain != NULL);
	return kvf_swapchain->images_count;
}

uint32_t kvfGetSwapchainMinImagesCount(VkSwapchainKHR swapchain)
{
	__KvfSwapchain* kvf_swapchain = __kvfGetKvfSwapchainFromVkSwapchainKHR(swapchain);
	KVF_ASSERT(kvf_swapchain != NULL);
	return kvf_swapchain->support.capabilities.minImageCount;
}

VkExtent2D kvfGetSwapchainImagesSize(VkSwapchainKHR swapchain)
{
	__KvfSwapchain* kvf_swapchain = __kvfGetKvfSwapchainFromVkSwapchainKHR(swapchain);
	KVF_ASSERT(kvf_swapchain != NULL);
	return kvf_swapchain->images_extent;
}

void kvfDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain)
{
	if(swapchain == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	__kvfDestroySwapchain(device, swapchain);
}

VkImage kvfCreateImage(VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	VkImageCreateInfo image_info = {};
	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.imageType = VK_IMAGE_TYPE_2D;
	image_info.extent.width = width;
	image_info.extent.height = height;
	image_info.extent.depth = 1;
	image_info.mipLevels = 1;
	image_info.arrayLayers = 1;
	image_info.format = format;
	image_info.tiling = tiling;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage = usage;
	image_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VkImage image;
	__kvfCheckVk(vkCreateImage(device, &image_info, NULL, &image));
	return image;
}

void kvfDestroyImage(VkDevice device, VkImage image)
{
	if(image == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroyImage(device, image, NULL);
}

VkImageView kvfCreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageViewType type, VkImageAspectFlags aspect)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	VkImageViewCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	create_info.image = image;
	create_info.viewType = type;
	create_info.format = format;
	create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.subresourceRange.aspectMask = aspect;
	create_info.subresourceRange.baseMipLevel = 0;
	create_info.subresourceRange.levelCount = 1;
	create_info.subresourceRange.baseArrayLayer = 0;
	create_info.subresourceRange.layerCount = 1;
	VkImageView view;
	__kvfCheckVk(vkCreateImageView(device, &create_info, NULL, &view));
	return view;
}

void kvfDestroyImageView(VkDevice device, VkImageView image_view)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	KVF_ASSERT(image_view != VK_NULL_HANDLE);
	vkDestroyImageView(device, image_view, NULL);
}

void kvfTransitionImageLayout(VkDevice device, VkImage image, VkCommandBuffer cmd, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout, bool is_single_time_cmd_buffer)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);

	if(new_layout == old_layout)
		return;

	if(is_single_time_cmd_buffer)
		kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = old_layout;
	barrier.newLayout = new_layout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = kvfIsDepthFormat(format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = kvfLayoutToAccessMask(old_layout, false);
	barrier.dstAccessMask = kvfLayoutToAccessMask(new_layout, true);
	if(kvfIsStencilFormat(format))
		barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

	VkPipelineStageFlags source_stage = 0;
	if(barrier.oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		source_stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	else if(barrier.srcAccessMask != 0)
		source_stage = kvfAccessFlagsToPipelineStage(barrier.srcAccessMask, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
	else
		source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

	VkPipelineStageFlags destination_stage = 0;
	if(barrier.newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		destination_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	else if(barrier.dstAccessMask != 0)
		destination_stage = kvfAccessFlagsToPipelineStage(barrier.dstAccessMask, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
	else
		destination_stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

	vkCmdPipelineBarrier(cmd, source_stage, destination_stage, 0, 0, NULL, 0, NULL, 1, &barrier);

	if(is_single_time_cmd_buffer)
	{
		kvfEndCommandBuffer(cmd);
		VkFence fence = kvfCreateFence(device);
		kvfSubmitSingleTimeCommandBuffer(device, cmd, KVF_GRAPHICS_QUEUE, fence);
		kvfDestroyFence(device, fence);
	}
}

VkSampler kvfCreateSampler(VkDevice device, VkFilter filters, VkSamplerAddressMode address_modes, VkSamplerMipmapMode mipmap_mode)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	VkSamplerCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	info.magFilter = filters;
	info.minFilter = filters;
	info.mipmapMode = mipmap_mode;
	info.addressModeU = address_modes;
	info.addressModeV = address_modes;
	info.addressModeW = address_modes;
	info.minLod = -1000;
	info.maxLod = 1000;
	info.anisotropyEnable = VK_FALSE;
	info.maxAnisotropy = 1.0f;
	VkSampler sampler;
	__kvfCheckVk(vkCreateSampler(device, &info, NULL, &sampler));
	return sampler;
}

void kvfDestroySampler(VkDevice device, VkSampler sampler)
{
	if(sampler == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroySampler(device, sampler, NULL);
}

VkBuffer kvfCreateBuffer(VkDevice device, VkBufferUsageFlags usage, VkDeviceSize size)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	VkBufferCreateInfo buffer_info = {};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.size = size;
	buffer_info.usage = usage;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VkBuffer buffer;
	__kvfCheckVk(vkCreateBuffer(device, &buffer_info, NULL, &buffer));
	return buffer;
}

void kvfCopyBufferToBuffer(VkCommandBuffer cmd, VkBuffer dst, VkBuffer src, size_t size)
{
	KVF_ASSERT(cmd != VK_NULL_HANDLE);
	KVF_ASSERT(dst != VK_NULL_HANDLE);
	KVF_ASSERT(src != VK_NULL_HANDLE);
	VkBufferCopy copy_region = {};
	copy_region.size = size;
	vkCmdCopyBuffer(cmd, src, dst, 1, &copy_region);
}

void kvfCopyBufferToImage(VkCommandBuffer cmd, VkImage dst, VkBuffer src, size_t buffer_offset, VkImageAspectFlagBits aspect, VkExtent3D extent)
{
	KVF_ASSERT(cmd != VK_NULL_HANDLE);
	KVF_ASSERT(dst != VK_NULL_HANDLE);
	KVF_ASSERT(src != VK_NULL_HANDLE);
	VkOffset3D offset = { 0, 0, 0 };
	VkBufferImageCopy region = {};
	region.bufferOffset = buffer_offset;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = aspect;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = offset;
	region.imageExtent = extent;
	vkCmdCopyBufferToImage(cmd, src, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void kvfDestroyBuffer(VkDevice device, VkBuffer buffer)
{
	if(buffer != VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroyBuffer(device, buffer, NULL);
}

VkFramebuffer kvfCreateFramebuffer(VkDevice device, VkRenderPass render_pass, VkImageView* image_views, size_t image_views_count, VkExtent2D extent)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	KVF_ASSERT(image_views != NULL);

	VkFramebufferCreateInfo framebuffer_info = {};
	framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebuffer_info.renderPass = render_pass;
	framebuffer_info.attachmentCount = image_views_count;
	framebuffer_info.pAttachments = image_views;
	framebuffer_info.width = extent.width;
	framebuffer_info.height = extent.height;
	framebuffer_info.layers = 1;
	VkFramebuffer framebuffer = VK_NULL_HANDLE;
	__kvfCheckVk(vkCreateFramebuffer(device, &framebuffer_info, NULL, &framebuffer));
	__kvfAddFramebufferToArray(framebuffer, extent);
	return framebuffer;
}

VkExtent2D kvfGetFramebufferSize(VkFramebuffer buffer)
{
	__KvfFramebuffer* kvf_framebuffer = __kvfGetKvfSwapchainFromVkFramebuffer(buffer);
	KVF_ASSERT(kvf_framebuffer != NULL);
	return kvf_framebuffer->extent;
}

void kvfDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer)
{
	if(framebuffer == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	__kvfDestroyFramebuffer(device, framebuffer);
}

VkCommandBuffer kvfCreateCommandBuffer(VkDevice device)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	return kvfCreateCommandBufferLeveled(device, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
}

VkCommandBuffer kvfCreateCommandBufferLeveled(VkDevice device, VkCommandBufferLevel level)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	__KvfDevice* kvfdevice = __kvfGetKvfDeviceFromVkDevice(device);
	KVF_ASSERT(kvfdevice != NULL);

	VkCommandPool pool = kvfdevice->cmd_pool;
	VkCommandBuffer buffer;
	VkCommandBufferAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = pool;
	alloc_info.level = level;
	alloc_info.commandBufferCount = 1;
	__kvfCheckVk(vkAllocateCommandBuffers(device, &alloc_info, &buffer));
	return buffer;
}

void kvfBeginCommandBuffer(VkCommandBuffer buffer, VkCommandBufferUsageFlags usage)
{
	KVF_ASSERT(buffer != VK_NULL_HANDLE);
	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = usage;
	__kvfCheckVk(vkBeginCommandBuffer(buffer, &begin_info));
}

void kvfEndCommandBuffer(VkCommandBuffer buffer)
{
	KVF_ASSERT(buffer != VK_NULL_HANDLE);
	__kvfCheckVk(vkEndCommandBuffer(buffer));
}

void kvfSubmitCommandBuffer(VkDevice device, VkCommandBuffer buffer, KvfQueueType queue, VkSemaphore signal, VkSemaphore wait, VkFence fence, VkPipelineStageFlags* stages)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);

	VkSemaphore signal_semaphores[1];
	VkSemaphore wait_semaphores[1];
	signal_semaphores[0] = signal;
	wait_semaphores[0] = wait;

	if(fence != VK_NULL_HANDLE)
		vkResetFences(device, 1, &fence);

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount = (wait == VK_NULL_HANDLE ? 0 : 1);
	submit_info.pWaitSemaphores = wait_semaphores;
	submit_info.pWaitDstStageMask = stages;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &buffer;
	submit_info.signalSemaphoreCount = (signal == VK_NULL_HANDLE ? 0 : 1);
	submit_info.pSignalSemaphores = signal_semaphores;
	__kvfCheckVk(vkQueueSubmit(kvfGetDeviceQueue(device, queue), 1, &submit_info, fence));
}

void kvfSubmitSingleTimeCommandBuffer(VkDevice device, VkCommandBuffer buffer, KvfQueueType queue, VkFence fence)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);

	if(fence != VK_NULL_HANDLE)
		vkResetFences(device, 1, &fence);

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &buffer;
	__kvfCheckVk(vkQueueSubmit(kvfGetDeviceQueue(device, queue), 1, &submit_info, fence));
	if(fence != VK_NULL_HANDLE)
		vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
}

VkAttachmentDescription kvfBuildAttachmentDescription(KvfImageType type, VkFormat format, VkImageLayout initial, VkImageLayout final, bool clear)
{
	VkAttachmentDescription attachment = {};

	switch(type)
	{
		case KVF_IMAGE_CUBE:
		case KVF_IMAGE_DEPTH_ARRAY:
		case KVF_IMAGE_COLOR:
		case KVF_IMAGE_DEPTH:
		{
			attachment.format = format;
			attachment.initialLayout = initial;
			attachment.finalLayout = final;
			break;
		}

		default: KVF_ASSERT(false && "KVF Attachment Description builder : unsupported image type"); break;
	}

	if(clear)
	{
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	}
	else
	{
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	}

	attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.flags = 0;

	return attachment;
}

VkAttachmentDescription kvfBuildSwapchainAttachmentDescription(VkSwapchainKHR swapchain, bool clear)
{
	__KvfSwapchain* kvf_swapchain = __kvfGetKvfSwapchainFromVkSwapchainKHR(swapchain);
	KVF_ASSERT(kvf_swapchain != NULL);
	KVF_ASSERT(kvf_swapchain->images_count != 0);
	return kvfBuildAttachmentDescription(KVF_IMAGE_COLOR, kvf_swapchain->images_format, VK_IMAGE_LAYOUT_UNDEFINED,VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, clear);
}

VkRenderPass kvfCreateRenderPass(VkDevice device, VkAttachmentDescription* attachments, size_t attachments_count, VkPipelineBindPoint bind_point)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);

	size_t color_attachment_count = 0;
	size_t depth_attachment_count = 0;

	for(size_t i = 0; i < attachments_count; i++)
	{
		if(kvfIsDepthFormat(attachments[i].format))
			depth_attachment_count++;
		else
			color_attachment_count++;
	}

	VkAttachmentReference* color_references = NULL;
	VkAttachmentReference* depth_references = NULL;

	if(color_attachment_count != 0)
	{
		color_references = (VkAttachmentReference*)KVF_MALLOC(color_attachment_count * sizeof(VkAttachmentReference));
		KVF_ASSERT(color_references != NULL);
	}
	if(depth_attachment_count != 0)
	{
		depth_references = (VkAttachmentReference*)KVF_MALLOC(depth_attachment_count * sizeof(VkAttachmentReference));
		KVF_ASSERT(depth_references != NULL);
	}

	for(size_t i = 0, c = 0, d = 0; i < attachments_count; i++)
	{
		if(!kvfIsDepthFormat(attachments[i].format))
		{
			VkImageLayout layout = attachments[i].finalLayout;
			color_references[c].attachment = i;
			color_references[c].layout = layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : layout;
			c++;
		}
		else
		{
			depth_references[d].attachment = i;
			depth_references[d].layout = attachments[i].finalLayout;
			d++;
		}
	}

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = bind_point;
	subpass.colorAttachmentCount = color_attachment_count;
	subpass.pColorAttachments = color_references;
	subpass.pDepthStencilAttachment = depth_references;

	VkRenderPassCreateInfo renderpass_create_info = {};
	renderpass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderpass_create_info.attachmentCount = attachments_count;
	renderpass_create_info.pAttachments = attachments;
	renderpass_create_info.subpassCount = 1;
	renderpass_create_info.pSubpasses = &subpass;
	renderpass_create_info.dependencyCount = 0;
	renderpass_create_info.pDependencies = NULL;

	VkRenderPass render_pass = VK_NULL_HANDLE;
	__kvfCheckVk(vkCreateRenderPass(device, &renderpass_create_info, NULL, &render_pass));
	KVF_FREE(color_references);
	KVF_FREE(depth_references);
	return render_pass;
}

void kvfDestroyRenderPass(VkDevice device, VkRenderPass renderPass)
{
	if(renderPass == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroyRenderPass(device, renderPass, NULL);
}

void kvfBeginRenderPass(VkRenderPass pass, VkCommandBuffer cmd, VkFramebuffer framebuffer, VkExtent2D framebuffer_extent, VkClearValue* clears, size_t clears_count)
{
	KVF_ASSERT(pass != VK_NULL_HANDLE);
	KVF_ASSERT(framebuffer != VK_NULL_HANDLE);

	VkOffset2D offset = { 0, 0 };
	VkRenderPassBeginInfo renderpass_info = {};
	renderpass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderpass_info.renderPass = pass;
	renderpass_info.framebuffer = framebuffer;
	renderpass_info.renderArea.offset = offset;
	renderpass_info.renderArea.extent = framebuffer_extent;
	renderpass_info.clearValueCount = clears_count;
	renderpass_info.pClearValues = clears;
	vkCmdBeginRenderPass(cmd, &renderpass_info, VK_SUBPASS_CONTENTS_INLINE);
}

VkShaderModule kvfCreateShaderModule(VkDevice device, uint32_t* code, size_t size)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = size * sizeof(uint32_t);
	createInfo.pCode = code;
	VkShaderModule shader = VK_NULL_HANDLE;
	__kvfCheckVk(vkCreateShaderModule(device, &createInfo, NULL, &shader));
	return shader;
}

void kvfDestroyShaderModule(VkDevice device, VkShaderModule shader)
{
	if(shader == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroyShaderModule(device, shader, NULL);
}

VkDescriptorSetLayout kvfCreateDescriptorSetLayout(VkDevice device, VkDescriptorSetLayoutBinding* bindings, size_t bindings_count)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	VkDescriptorSetLayoutCreateInfo layout_info = {};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = bindings_count;
	layout_info.pBindings = bindings;

	VkDescriptorSetLayout layout;
	__kvfCheckVk(vkCreateDescriptorSetLayout(device, &layout_info, NULL, &layout));
	return layout;
}

void kvfDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout layout)
{
	if(layout == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroyDescriptorSetLayout(device, layout, NULL);
}

VkDescriptorSet kvfAllocateDescriptorSet(VkDevice device, VkDescriptorSetLayout layout)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	__KvfDevice* kvf_device = __kvfGetKvfDeviceFromVkDevice(device);
	KVF_ASSERT(kvf_device != NULL);
	VkDescriptorPool pool = VK_NULL_HANDLE;
	for(int i = 0; i < kvf_device->sets_pools_size; i++)
	{
		if(kvf_device->sets_pools[i].size < kvf_device->sets_pools[i].capacity)
			pool = kvf_device->sets_pools[i].pool;
	}
	if(pool == VK_NULL_HANDLE)
		pool = __kvfDeviceCreateDescriptorPool(device);
	KVF_ASSERT(pool != VK_NULL_HANDLE);

	VkDescriptorSet set = VK_NULL_HANDLE;
	VkDescriptorSetAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	alloc_info.descriptorPool = pool;
	alloc_info.descriptorSetCount = 1;
	alloc_info.pSetLayouts = &layout;
	__kvfCheckVk(vkAllocateDescriptorSets(device, &alloc_info, &set));
	KVF_ASSERT(set != VK_NULL_HANDLE);
	return set;
}

void kvfUpdateStorageBufferToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorBufferInfo* info, uint32_t binding)
{
	VkWriteDescriptorSet write = kvfWriteStorageBufferToDescriptorSet(device, set, info, binding);
	vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
}

void kvfUpdateUniformBufferToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorBufferInfo* info, uint32_t binding)
{
	VkWriteDescriptorSet write = kvfWriteUniformBufferToDescriptorSet(device, set, info, binding);
	vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
}

void kvfUpdateImageToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorImageInfo* info, uint32_t binding)
{
	VkWriteDescriptorSet write = kvfWriteImageToDescriptorSet(device, set, info, binding);
	vkUpdateDescriptorSets(device, 1, &write, 0, NULL);
}

VkWriteDescriptorSet kvfWriteStorageBufferToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorBufferInfo* info, uint32_t binding)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	KVF_ASSERT(set != VK_NULL_HANDLE);
	VkWriteDescriptorSet descriptor_write = {};
	descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptor_write.dstSet = set;
	descriptor_write.dstBinding = binding;
	descriptor_write.dstArrayElement = 0;
	descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptor_write.descriptorCount = 1;
	descriptor_write.pBufferInfo = info;
	return descriptor_write;
}

VkWriteDescriptorSet kvfWriteUniformBufferToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorBufferInfo* info, uint32_t binding)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	KVF_ASSERT(set != VK_NULL_HANDLE);
	VkWriteDescriptorSet descriptor_write = {};
	descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptor_write.dstSet = set;
	descriptor_write.dstBinding = binding;
	descriptor_write.dstArrayElement = 0;
	descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptor_write.descriptorCount = 1;
	descriptor_write.pBufferInfo = info;
	return descriptor_write;
}

VkWriteDescriptorSet kvfWriteImageToDescriptorSet(VkDevice device, VkDescriptorSet set, const VkDescriptorImageInfo* info, uint32_t binding)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	KVF_ASSERT(set != VK_NULL_HANDLE);
	VkWriteDescriptorSet descriptor_write = {};
	descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptor_write.dstSet = set;
	descriptor_write.dstBinding = binding;
	descriptor_write.dstArrayElement = 0;
	descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptor_write.descriptorCount = 1;
	descriptor_write.pImageInfo = info;
	return descriptor_write;
}

VkPipelineLayout kvfCreatePipelineLayout(VkDevice device, VkDescriptorSetLayout* set_layouts, size_t set_layouts_count, VkPushConstantRange* pc, size_t pc_count)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	VkPipelineLayoutCreateInfo pipeline_layout_info = {};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = set_layouts_count;
	pipeline_layout_info.pSetLayouts = set_layouts;
	pipeline_layout_info.pushConstantRangeCount = pc_count;
	pipeline_layout_info.pPushConstantRanges = pc;

	VkPipelineLayout layout;
	__kvfCheckVk(vkCreatePipelineLayout(device, &pipeline_layout_info, NULL, &layout));
	return layout;
}

void kvfDestroyPipelineLayout(VkDevice device, VkPipelineLayout layout)
{
	if(layout == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroyPipelineLayout(device, layout, NULL);
}

void kvfResetDeviceDescriptorPools(VkDevice device)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	__KvfDevice* kvf_device = __kvfGetKvfDeviceFromVkDevice(device);
	KVF_ASSERT(kvf_device != NULL);
	for(int i = 0; i < kvf_device->sets_pools_size; i++)
	{
		vkResetDescriptorPool(device, kvf_device->sets_pools[i].pool, 0);
		kvf_device->sets_pools[i].size = 0;
	}
}

KvfGraphicsPipelineBuilder* kvfCreateGPipelineBuilder()
{
	KvfGraphicsPipelineBuilder* builder = (KvfGraphicsPipelineBuilder*)KVF_MALLOC(sizeof(KvfGraphicsPipelineBuilder));
	memset(builder, 0, sizeof(KvfGraphicsPipelineBuilder));
	kvfGPipelineBuilderReset(builder);
	return builder;
}

void kvfDestroyGPipelineBuilder(KvfGraphicsPipelineBuilder* builder)
{
	KVF_ASSERT(builder != NULL);
	KVF_FREE(builder->shader_stages);
	if(builder->vertex_input_state.pVertexAttributeDescriptions != NULL)
		KVF_FREE((VkVertexInputAttributeDescription*)builder->vertex_input_state.pVertexAttributeDescriptions);
	if(builder->vertex_input_state.pVertexBindingDescriptions != NULL)
		KVF_FREE((VkVertexInputBindingDescription*)builder->vertex_input_state.pVertexBindingDescriptions);
	KVF_FREE(builder);
}

void kvfGPipelineBuilderReset(KvfGraphicsPipelineBuilder* builder)
{
	KVF_ASSERT(builder != NULL);
	KVF_FREE(builder->shader_stages);
	if(builder->vertex_input_state.pVertexAttributeDescriptions != NULL)
		KVF_FREE((VkVertexInputAttributeDescription*)builder->vertex_input_state.pVertexAttributeDescriptions);
	if(builder->vertex_input_state.pVertexBindingDescriptions != NULL)
		KVF_FREE((VkVertexInputBindingDescription*)builder->vertex_input_state.pVertexBindingDescriptions);
	memset(builder, 0, sizeof(KvfGraphicsPipelineBuilder));
	builder->vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	builder->input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	builder->tessellation_state.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	builder->rasterization_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	builder->depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
}

void kvfGPipelineBuilderSetInputTopology(KvfGraphicsPipelineBuilder* builder, VkPrimitiveTopology topology)
{
	KVF_ASSERT(builder != NULL);
	builder->input_assembly_state.topology = topology;
	builder->input_assembly_state.primitiveRestartEnable = VK_FALSE;
}

void kvfGPipelineBuilderSetPolygonMode(KvfGraphicsPipelineBuilder* builder, VkPolygonMode polygon, float line_width)
{
	KVF_ASSERT(builder != NULL);
	builder->rasterization_state.polygonMode = polygon;
	builder->rasterization_state.lineWidth = line_width;
}

void kvfGPipelineBuilderSetCullMode(KvfGraphicsPipelineBuilder* builder, VkCullModeFlags cull, VkFrontFace face)
{
	KVF_ASSERT(builder != NULL);
	builder->rasterization_state.cullMode = cull;
	builder->rasterization_state.frontFace = face;
}

void kvfGPipelineBuilderDisableBlending(KvfGraphicsPipelineBuilder* builder)
{
	KVF_ASSERT(builder != NULL);
	builder->color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	builder->color_blend_attachment_state.blendEnable = VK_FALSE;
}

void kvfGPipelineBuilderEnableAdditiveBlending(KvfGraphicsPipelineBuilder* builder)
{
	KVF_ASSERT(builder != NULL);
	builder->color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	builder->color_blend_attachment_state.blendEnable = VK_TRUE;
	builder->color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	builder->color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
	builder->color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
	builder->color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	builder->color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	builder->color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
}

void kvfGPipelineBuilderEnableAlphaBlending(KvfGraphicsPipelineBuilder* builder)
{
	KVF_ASSERT(builder != NULL);
	builder->color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	builder->color_blend_attachment_state.blendEnable = VK_TRUE;
	builder->color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	builder->color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	builder->color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
	builder->color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	builder->color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	builder->color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
}

void kvfGPipelineBuilderEnableDepthTest(KvfGraphicsPipelineBuilder* builder, VkCompareOp op, bool write_enabled)
{
	KVF_ASSERT(builder != NULL);
	builder->depth_stencil_state.depthTestEnable = VK_TRUE;
	builder->depth_stencil_state.depthWriteEnable = write_enabled;
	builder->depth_stencil_state.depthCompareOp = op;
	builder->depth_stencil_state.depthBoundsTestEnable = VK_FALSE;
	builder->depth_stencil_state.stencilTestEnable = VK_FALSE;
	builder->depth_stencil_state.minDepthBounds = 0.f;
	builder->depth_stencil_state.maxDepthBounds = 1.f;
}

void kvfGPipelineBuilderDisableDepthTest(KvfGraphicsPipelineBuilder* builder)
{
	KVF_ASSERT(builder != NULL);
	builder->depth_stencil_state.depthTestEnable = VK_FALSE;
	builder->depth_stencil_state.depthWriteEnable = VK_FALSE;
	builder->depth_stencil_state.depthCompareOp = VK_COMPARE_OP_NEVER;
	builder->depth_stencil_state.depthBoundsTestEnable = VK_FALSE;
	builder->depth_stencil_state.stencilTestEnable = VK_FALSE;
	builder->depth_stencil_state.minDepthBounds = 0.f;
	builder->depth_stencil_state.maxDepthBounds = 1.f;
}

void kvfGPipelineBuilderSetVertexInputs(KvfGraphicsPipelineBuilder* builder, VkVertexInputBindingDescription binds, VkVertexInputAttributeDescription* attributes, size_t attributes_count)
{
	KVF_ASSERT(builder != NULL);
	KVF_ASSERT(attributes != NULL);
	VkVertexInputBindingDescription* binds_ptr = (VkVertexInputBindingDescription*)KVF_MALLOC(sizeof(VkVertexInputBindingDescription));
	KVF_ASSERT(binds_ptr != NULL);
	*binds_ptr = binds;
	VkVertexInputAttributeDescription* attributes_descriptions = (VkVertexInputAttributeDescription*)KVF_MALLOC(sizeof(VkVertexInputAttributeDescription) * attributes_count);
	KVF_ASSERT(attributes_descriptions != NULL);
	memcpy(attributes_descriptions, attributes, sizeof(VkVertexInputAttributeDescription) * attributes_count);
	builder->vertex_input_state.vertexBindingDescriptionCount = 1;
	builder->vertex_input_state.pVertexBindingDescriptions = binds_ptr;
	builder->vertex_input_state.vertexAttributeDescriptionCount = attributes_count;
	builder->vertex_input_state.pVertexAttributeDescriptions = attributes_descriptions;
}

void kvfGPipelineBuilderAddShaderStage(KvfGraphicsPipelineBuilder* builder, VkShaderStageFlagBits stage, VkShaderModule module, const char* entry)
{
	KVF_ASSERT(builder != NULL);
	builder->shader_stages = (VkPipelineShaderStageCreateInfo*)KVF_REALLOC(builder->shader_stages, sizeof(VkPipelineShaderStageCreateInfo) * (builder->shader_stages_count + 1));
	KVF_ASSERT(builder->shader_stages != NULL);
	memset(&builder->shader_stages[builder->shader_stages_count], 0, sizeof(VkPipelineShaderStageCreateInfo));
	char* entry_ptr = (char*)KVF_MALLOC(strlen(entry));
	KVF_ASSERT(entry_ptr != NULL);
	strcpy(entry_ptr, entry);
	builder->shader_stages[builder->shader_stages_count].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	builder->shader_stages[builder->shader_stages_count].stage = stage;
	builder->shader_stages[builder->shader_stages_count].module = module;
	builder->shader_stages[builder->shader_stages_count].pName = entry_ptr;
	builder->shader_stages_count++;
}

void kvfGPipelineBuilderResetShaderStages(KvfGraphicsPipelineBuilder* builder)
{
	KVF_ASSERT(builder != NULL);
	if(builder->shader_stages == NULL)
		return;

	for(size_t i = 0; builder->shader_stages_count; i++)
		KVF_FREE((char*)builder->shader_stages[i].pName);
	KVF_FREE(builder->shader_stages);
	builder->shader_stages_count = 0;
}

VkPipeline kvfCreateGraphicsPipeline(VkDevice device, VkPipelineLayout layout, KvfGraphicsPipelineBuilder* builder, VkRenderPass pass)
{
	KVF_ASSERT(builder != NULL);
	KVF_ASSERT(device != VK_NULL_HANDLE);

	VkPipelineColorBlendStateCreateInfo color_blending = {};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY;
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &builder->color_blend_attachment_state;
	color_blending.blendConstants[0] = 0.0f;
	color_blending.blendConstants[1] = 0.0f;
	color_blending.blendConstants[2] = 0.0f;
	color_blending.blendConstants[3] = 0.0f;

	VkDynamicState states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo dynamic_states = {};
	dynamic_states.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_states.dynamicStateCount = sizeof(states) / sizeof(VkDynamicState);
	dynamic_states.pDynamicStates = states;

	VkPipelineViewportStateCreateInfo viewport_state = {};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.pViewports = NULL;
	viewport_state.scissorCount = 1;
	viewport_state.pScissors = NULL;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkGraphicsPipelineCreateInfo pipeline_info = {};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = builder->shader_stages_count;
	pipeline_info.pStages = builder->shader_stages;
	pipeline_info.pVertexInputState = &builder->vertex_input_state;
	pipeline_info.pInputAssemblyState = &builder->input_assembly_state;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &builder->rasterization_state;
	pipeline_info.pMultisampleState = &multisampling;
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.pDynamicState = &dynamic_states;
	pipeline_info.layout = layout;
	pipeline_info.renderPass = pass;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
	pipeline_info.pDepthStencilState = &builder->depth_stencil_state;

	VkPipeline pipeline;
	__kvfCheckVk(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, NULL, &pipeline));
	return pipeline;
}

void kvfDestroyPipeline(VkDevice device, VkPipeline pipeline)
{
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroyPipeline(device, pipeline, NULL);
}

#endif // KVF_IMPLEMENTATION
