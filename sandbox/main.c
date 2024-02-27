#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <stdlib.h>
#define KVF_IMPLEMENTATION
#define KVF_ENABLE_VALIDATION_LAYERS
#include "../kvf.h"

int main(void)
{
	SDL_Window* win = SDL_CreateWindow("KVF Sandbox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_VULKAN);
	uint32_t ext_count;
	SDL_Vulkan_GetInstanceExtensions(win, &ext_count, NULL);
	const char** exts = (const char**)malloc(sizeof(char*) * (ext_count + 1));
	exts[0] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
	SDL_Vulkan_GetInstanceExtensions(win, &ext_count, exts + 1);

	VkInstance instance = kvfCreateInstance(exts, ext_count + 1);
	free(exts);

	VkSurfaceKHR surface = VK_NULL_HANDLE;
	SDL_Vulkan_CreateSurface(win, instance, &surface);

	VkPhysicalDevice ph_device = kvfPickGoodDefaultPhysicalDevice(instance, surface);
	VkDevice device = kvfCreateDefaultDevice(ph_device);

	VkQueue graphics_queue = kvfGetDeviceQueue(device, KVF_GRAPHICS_QUEUE);
	VkQueue present_queue = kvfGetDeviceQueue(device, KVF_PRESENT_QUEUE);

	int width, height;
	SDL_Vulkan_GetDrawableSize(win, &width, &height);
	VkExtent2D actualExtent = { (uint32_t)width, (uint32_t)height };
	VkSwapchainKHR swapchain = kvfCreateSwapchainKHR(device, ph_device, surface, actualExtent, true);
	size_t image_views_number;
	VkImageView* swapchain_image_views = kvfCreateSwapChainImageViewsKHR(device, swapchain, &image_views_number);

	SDL_Delay(2000);

	kvfDestroySwapChainImageViewsKHR(device, swapchain_image_views, image_views_number);
	kvfDestroySwapchainKHR(device, swapchain);
	vkDestroySurfaceKHR(instance, surface, NULL);
	kvfDestroyDevice(device);
	kvfDestroyInstance(instance);

	return 0;
}
