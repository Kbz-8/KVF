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
 *	You can also #define KVF_ENABLE_VALIDATION_LAYERS to enable validation layers when 
 *	compiling in release (they are always enabled in debug).
 */

#ifndef KBZ_8_VULKAN_FRAMEWORK_H
#define KBZ_8_VULKAN_FRAMEWORK_H

#ifndef KVF_IMPL_VK_NO_PROTOTYPES
	#include <vulkan/vulkan.h>
#endif // KVF_IMPL_VK_NO_PROTOTYPES

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	const char** extensionsEnabled;
	const uint32_t extensionsCount;
} KvfInstanceDesc;

VkInstance kvfCreateInstance(KvfInstanceDesc description);
void kvfDestroyInstance(VkInstance instance);

VkPhysicalDevice kvfPickFirstPhysicalDevice(VkInstance instance);
VkPhysicalDevice kvfPickGoodPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const char** deviceExtensions, uint32_t deviceExtensionsCount);

VkDevice kvfCreateDevice(VkPhysicalDevice physical);
void kvfDestroyDevice(VkDevice device);

#ifdef __cplusplus
}
#endif

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

VkInstance kvfCreateInstance(KvfInstanceDesc description)
{
	VkInstance vk_instance = VK_NULL_HANDLE;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = NULL;
	createInfo.enabledExtensionCount = description.extensionsCount;
	createInfo.ppEnabledExtensionNames = description.extensionsEnabled;

	checkVk(vkCreateInstance(&createInfo, NULL, &vk_instance));
	return vk_instance;
}

void kvfDestroyInstance(VkInstance instance)
{
	if(instance == VK_NULL_HANDLE)
		return;
	checkVk(vkDestroyInstance(instance, NULL));
}

VkPhysicalDevice kvfPickFirstPhysicalDevice(VkInstance instance)
{
	uint32_t device_count;
	vkphysicaldevice* devices = NULL;
	vkphysicaldevice chosen_one = VK_NULL_HANDLE;

	KVF_ASSERT(instance != VK_NULL_HANDLE);
	
	vkEnumeratePhysicalDevices(instance, &device_count, NULL);
	devices = KVF_MALLOC(sizeof(VkPhysicalDevice) * device_count + 1);
	vkEnumeratePhysicalDevices(instance, &device_count, devices);
	chosen_one = devices[0];
	KVF_FREE(devices);
	return chosen_one;
}

VkPhysicalDevice kvfPickGoodPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const char** deviceExtensions, uint32_t deviceExtensionsCount)
{
	uint32_t device_count;
	VkPhysicalDevice* devices = NULL;
	VkPhysicalDevice chosen_one = VK_NULL_HANDLE;

	KVF_ASSERT(instance != VK_NULL_HANDLE);
	KVF_ASSERT(surface != VK_NULL_HANDLE);
	
	vkEnumeratePhysicalDevices(instance, &device_count, NULL);
	devices = KVF_MALLOC(sizeof(VkPhysicaDevice) * device_count + 1);
	vkEnumeratePhysicalDvices(instance, &device_count, devices);

	for(int i = 0; i < device_count; i++)
	{
		/* Check Extensions Support */
		uint32_t extension_count;
		vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extension_count, NULL);
		VkExtensionProperties* props = KVF_MALLOC(sizeof(VkExtensionProperties) * extension_count + 1);
		vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extension_count, props);
	
		bool are_there_required_device_extensions = true;
		for(int j = 0; j < extensions_count; j++)
		{
			bool is_there_extension = false;
			for(int k = 0; k < deviceExtensionsCount; k++)
			{
				if(strcmp(deviceExtensions[k], props[j].extensionName) == 0)
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

		/* Check Queue Families */
		// TODO
		
		// If we get there, the device is good
		chosen_one = devices[i];
		break;
	}

	KVF_FREE(devices);
	return chosen_one;
}

VkDevice kvfCreateDevice(VkPhysicalDevice physical)
{

}

void kvfDestroyDevice(VkDevice device)
{
	if(device == VK_NULL_HANDLE)
		return;
	checkVk(vkDestroyDevice(device, NULL));
}

#endif // KVF_IMPLEMENTATION

#endif // KBZ_8_VULKAN_FRAMEWORK_H
