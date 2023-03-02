/**
 * 								 MIT License
 *
 *						  Copyright (c) 2023 kbz_8
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 *
 *
 *	Do this:
 *	#define KVF_IMPLEMENTATION
 *	before you include this file in *one* C or C++ file to create the implementation.
 *
 *	// i.e. it should look like this:
 *	#include ...
 *	#include ...
 *	#include ...
 *	#define KVF_IMPLEMENTATION
 *	#include "kvf.h"
 *	
 *	You can #define KVF_ASSERT(x) before the #include to avoid using assert.h.
 *	And #define KVF_MALLOC, KVF_REALLOC, and KVF_FREE to avoid using malloc, realloc, free.
 *
 *	If you are using Volk or any other meta loader you must define KVF_IMPL_VK_NO_PROTOTYPES
 *	before including this file to avoid conflicts with Vulkan prototypes.
 *
 *	You can also #define KVF_ENABLE_VALIDATION_LAYERS to enable validation layers.
 */

#ifndef KBZ_8_VULKAN_FRAMEWORK_H
#define KBZ_8_VULKAN_FRAMEWORK_H

#ifndef KVF_IMPL_VK_NO_PROTOTYPES
	#include <vulkan/vulkan.h>
#endif // KVF_IMPL_VK_NO_PROTOTYPES

#include <stdint.h>

/* ============================================= Prototypes ============================================= */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	KVF_GRAPHICS_QUEUE,
	KVF_PRESENT_QUEUE
} KvfQueueType;

VkInstance kvfCreateInstance(const char** extensionsEnabled, uint32_t extensionsCount);
void kvfDestroyInstance(VkInstance instance);

VkPhysicalDevice kvfPickFirstPhysicalDevice(VkInstance instance);
VkPhysicalDevice kvfPickGoodDefaultPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
VkPhysicalDevice kvfPickGoodPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const char** deviceExtensions, uint32_t deviceExtensionsCount);

void kvfFindQueueFamilies(VkPhysicalDevice physical, VkSurfaceKHR surface);
VkQueue kvfGetDeviceQueue(VkDevice device, KvfQueueType queue);

VkDevice kvfCreateDefaultDevice(VkPhysicalDevice physical);
VkDevice kvfCreateDevice(VkPhysicalDevice physical, const char** extensions, uint32_t extensions_count);
void kvfDestroyDevice(VkDevice device);

VkFence kvfCreateFence(VkDevice device);
void kvfDestroyFence(VkDevice device, VkFence fence);

VkSemaphore kvfCreateSemaphore(VkDevice device);
void kvfDestroySemaphore(VkDevice device, VkSemaphore semaphore);

VkSwapchainKHR kvfCreateSwapchain(VkDevice device, VkPhysicalDevice physical, VkSurfaceKHR surface);
void kvfDestroySwapchain(VkDevice device, VkSwapchainKHR swapchain);

#ifdef __cplusplus
}
#endif

/* ========================================== Implementation =========================================== */

#ifdef KVF_IMPLEMENTATION

#ifndef KVF_MALLOC
	#define KVF_MALLOC(x) malloc(x)
#endif
#ifndef KVF_REALLOC
	#define KVF_REALLOC(x) realloc(x)
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
#include <stdbool.h>

int32_t __kvf_graphics_queue_family = -1;
int32_t __kvf_present_queue_family = -1;

#ifdef KVF_ENABLE_VALIDATION_LAYERS
	VkDebugUtilsMessengerEXT __kvf_debug_messenger;
#endif

const char* verbaliseResultVk(VkResult result)
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
}

void checkVk(VkResult result)
{
	if(result != VK_SUCCESS)
		printf("KVF Vulkan error : %s\n", verbaliseResultVk(result));
}

#ifdef KVF_ENABLE_VALIDATION_LAYERS
	bool __kvfCheckValidationLayerSupport()
	{
		uint32_t layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, NULL);
		VkLayerProperties* available_layers = KVF_MALLOC(sizeof(VkLayerProperties) * layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

		for(int i = 0; i < layer_count; i++)
		{
			if(strcmp(available_layers[i].layerName, "VK_LAYER_KHRONOS_validation") == 0)
				return true;
		}
		return false;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL __kvfDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			printf("\nKVF Vulkan validation layer error : %s\n", pCallbackData->pMessage);
		else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			printf("\nKVF Vulkan validation layer warning : %s\n", pCallbackData->pMessage);
		return VK_FALSE;
	}

	void __kvfPopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo)
	{
		createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo->pfnUserCallback = __kvfDebugCallback;
	}
#endif // KVF_ENABLE_VALIDATION_LAYERS

VkInstance kvfCreateInstance(const char** extensionsEnabled, uint32_t extensionsCount)
{
	VkInstance vk_instance = VK_NULL_HANDLE;

	VkInstanceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = NULL;
	createInfo.flags = 0;
	createInfo.enabledExtensionCount = extensionsCount;
	createInfo.ppEnabledExtensionNames = extensionsEnabled;
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = NULL;
	createInfo.pNext = NULL;

#ifdef KVF_ENABLE_VALIDATION_LAYERS
	const char** new_extension_set = NULL;
	if(__kvfCheckValidationLayerSupport())
	{
		const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = { 0 };
		__kvfPopulateDebugMessengerCreateInfo(&debugCreateInfo);
		new_extension_set = KVF_MALLOC(sizeof(char*) * (extensionsCount + 1));
		memcpy(new_extension_set, extensionsEnabled, sizeof(char*) * extensionsCount);
		new_extension_set[extensionsCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

		createInfo.enabledExtensionCount = extensionsCount + 1;
		createInfo.ppEnabledExtensionNames = new_extension_set;
		createInfo.enabledLayerCount = 1;
		createInfo.ppEnabledLayerNames = layers;
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
#endif

	checkVk(vkCreateInstance(&createInfo, NULL, &vk_instance));
#ifdef KVF_ENABLE_VALIDATION_LAYERS
	KVF_FREE(new_extension_set);
#endif
	return vk_instance;
}

void kvfDestroyInstance(VkInstance instance)
{
	if(instance == VK_NULL_HANDLE)
		return;
#ifdef KVF_ENABLE_VALIDATION_LAYERS
	PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if(func)
		func(instance, __kvf_debug_messenger, NULL);
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
	devices = KVF_MALLOC(sizeof(VkPhysicalDevice) * device_count + 1);
	vkEnumeratePhysicalDevices(instance, &device_count, devices);
	chosen_one = devices[0];
	KVF_FREE(devices);
	return chosen_one;
}

void kvfFindQueueFamilies(VkPhysicalDevice physical, VkSurfaceKHR surface)
{
	if(__kvf_graphics_queue_family != -1 && __kvf_present_queue_family != -1)
		return;
	uint32_t queue_family_count;
	vkGetPhysicalDeviceQueueFamilyProperties(physical, &queue_family_count, NULL);
	VkQueueFamilyProperties* queue_families = KVF_MALLOC(sizeof(VkQueueFamilyProperties) * queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(physical, &queue_family_count, queue_families);
	
	for(int i = 0; i < queue_family_count; i++)
	{
		if(queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && __kvf_graphics_queue_family == -1)
		{
			__kvf_graphics_queue_family = i;
			continue;
		}
		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physical, i, surface, &present_support);
		if(present_support)
			__kvf_present_queue_family = i;

		if(__kvf_graphics_queue_family != -1 && __kvf_present_queue_family != -1)
			break;
	}
	KVF_FREE(queue_families);
}

VkPhysicalDevice kvfPickGoodDefaultPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
	const char* extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	return kvfPickGoodPhysicalDevice(instance, surface, extensions, sizeof(extensions) / sizeof(extensions[0]));
}

VkPhysicalDevice kvfPickGoodPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const char** deviceExtensions, uint32_t deviceExtensionsCount)
{
	uint32_t device_count;
	VkPhysicalDevice* devices = NULL;
	VkPhysicalDevice chosen_one = VK_NULL_HANDLE;

	KVF_ASSERT(instance != VK_NULL_HANDLE);
	KVF_ASSERT(surface != VK_NULL_HANDLE);
	
	vkEnumeratePhysicalDevices(instance, &device_count, NULL);
	devices = KVF_MALLOC(sizeof(VkPhysicalDevice) * device_count + 1);
	vkEnumeratePhysicalDevices(instance, &device_count, devices);

	for(int i = 0; i < device_count; i++)
	{
		/* Check Extensions Support */
		uint32_t extension_count;
		vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extension_count, NULL);
		VkExtensionProperties* props = KVF_MALLOC(sizeof(VkExtensionProperties) * extension_count + 1);
		vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extension_count, props);
	
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
			continue;

		/* Check Surface Formats Counts */
		uint32_t format_count;
		vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i], surface, &format_count, NULL);
		if(format_count == 0)
			continue;

		/* Check Queue Families Support */
		kvfFindQueueFamilies(devices[i], surface);
		if(__kvf_graphics_queue_family == -1 || __kvf_present_queue_family == -1)
			continue;
		
		// If we get there, the device is good
		chosen_one = devices[i];
		KVF_FREE(devices);
		return chosen_one;
	}
	KVF_FREE(devices);
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

	KVF_ASSERT(__kvf_graphics_queue_family != -1);
	KVF_ASSERT(__kvf_present_queue_family != -1);

	VkDeviceQueueCreateInfo queue_create_info[2];
	queue_create_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info[0].queueFamilyIndex = __kvf_graphics_queue_family;
	queue_create_info[0].queueCount = 1;
	queue_create_info[0].pQueuePriorities = &queue_priority;
	queue_create_info[0].flags = 0;
	queue_create_info[0].pNext = NULL;
	queue_create_info[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info[1].queueFamilyIndex = __kvf_present_queue_family;
	queue_create_info[1].queueCount = 1;
	queue_create_info[1].pQueuePriorities = &queue_priority;
	queue_create_info[1].flags = 0;
	queue_create_info[1].pNext = NULL;

	VkPhysicalDeviceFeatures device_features = { VK_FALSE };

	VkDeviceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = 2;
	createInfo.pQueueCreateInfos = queue_create_info;
	createInfo.pEnabledFeatures = &device_features;
	createInfo.enabledExtensionCount = extensions_count;
	createInfo.ppEnabledExtensionNames = extensions;
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = NULL;
	createInfo.flags = 0;
	createInfo.pNext = NULL;

	VkDevice device = VK_NULL_HANDLE;
	checkVk(vkCreateDevice(physical, &createInfo, NULL, &device));
	return device;
}

void kvfDestroyDevice(VkDevice device)
{
	if(device == VK_NULL_HANDLE)
		return;
	vkDestroyDevice(device, NULL);
}

VkQueue kvfGetDeviceQueue(VkDevice device, KvfQueueType queue)
{
	KVF_ASSERT(__kvf_graphics_queue_family != -1);
	KVF_ASSERT(__kvf_present_queue_family != -1);

	VkQueue vk_queue = VK_NULL_HANDLE;

	if(queue == KVF_GRAPHICS_QUEUE)
		vkGetDeviceQueue(device, __kvf_graphics_queue_family, 0, &vk_queue);
	else if(queue == KVF_GRAPHICS_QUEUE)
		vkGetDeviceQueue(device, __kvf_present_queue_family, 0, &vk_queue);
	return vk_queue;
}

VkFence kvfCreateFence(VkDevice device)
{
	VkFenceCreateInfo fenceInfo = { 0 };
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	
	VkFence fence = VK_NULL_HANDLE;
	checkVk(vkCreateFence(device, &fenceInfo, NULL, &fence));
	return fence;
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
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkSemaphore semaphore = VK_NULL_HANDLE;
	checkVk(vkCreateSemaphore(Render_Core::get().getDevice().get(), &semaphoreInfo, NULL, &semaphores));
	return semaphore;
}

void kvfDestroySemaphore(VkDevice device, VkSemaphore semaphore)
{
	if(semaphore == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroySemaphore(device, semaphores, NULL);
}

VkSwapchainKHR kvfCreateSwapchain(VkDevice device, VkPhysicalDevice physical, VkSurfaceKHR surface)
{

}

void kvfDestroySwapchain(VkDevice device, VkSwapchainKHR swapchain)
{
	if(swapchain == VK_NULL_HANDLE)
		return;
	KVF_ASSERT(device != VK_NULL_HANDLE);
	vkDestroySwapchainKHR(device, swapchain, NULL);
}

#endif // KVF_IMPLEMENTATION

#endif // KBZ_8_VULKAN_FRAMEWORK_H
