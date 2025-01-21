# KVF

KVF (kbz_8 vulkan framework) is a lightweight single-header Vulkan framework written in C99 to simplify the creation of Vulkan applications.
It is designed to be a drop in framework inside any existing Vulkan renderer as it does not use any custom types of structs (except for the graphics pipeline builder).

Here's a simple C example of a hello world triangle using KVF and SDL2 :
```C
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_vulkan.h>
#include <stdint.h>
#include <stdlib.h>
#define KVF_IMPLEMENTATION
#define KVF_ENABLE_VALIDATION_LAYERS
#include "kvf.h"

static const uint32_t vertex_shader[] = {
    /* Byte code */
};

static const uint32_t fragment_shader[] = {
    /* Byte code */
};

int main(void)
{
	// Window creation
	SDL_Window* win = SDL_CreateWindow("KVF Sandbox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_VULKAN);

	// Instance creation
	uint32_t ext_count;
	SDL_Vulkan_GetInstanceExtensions(win, &ext_count, NULL);
	const char** exts = (const char**)malloc(sizeof(char*) * ext_count);
	SDL_Vulkan_GetInstanceExtensions(win, &ext_count, exts);
	VkInstance instance = kvfCreateInstance(exts, ext_count);
	free(exts);

	// Surface creation
	VkSurfaceKHR surface; SDL_Vulkan_CreateSurface(win, instance, &surface);

	// Logical device creation
	VkPhysicalDevice ph_device = kvfPickGoodDefaultPhysicalDevice(instance, surface);
	VkDevice device = kvfCreateDefaultDevice(ph_device);

	// Swapchain creation
	VkExtent2D extent;
	SDL_Vulkan_GetDrawableSize(win, (int*)&extent.width, (int*)&extent.height);
	VkSwapchainKHR swapchain = kvfCreateSwapchainKHR(device, ph_device, surface, extent, VK_NULL_HANDLE, true, true);

	// Swapchain images acquisition
	uint32_t swapchain_images_count = kvfGetSwapchainImagesCount(swapchain);
	VkImage* swapchain_images = (VkImage*)calloc(swapchain_images_count, sizeof(VkImage));
	VkImageView* swapchain_images_views = (VkImageView*)calloc(swapchain_images_count, sizeof(VkImageView));
	vkGetSwapchainImagesKHR(device, swapchain, &swapchain_images_count /* useless */, swapchain_images);
	for(uint32_t i = 0; i < swapchain_images_count; i++)
	{
		VkCommandBuffer cmd = kvfCreateCommandBuffer(device);
		kvfTransitionImageLayout(device, swapchain_images[i], KVF_IMAGE_COLOR, cmd, kvfGetSwapchainImagesFormat(swapchain), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, true);
		swapchain_images_views[i] = kvfCreateImageView(device, swapchain_images[i], kvfGetSwapchainImagesFormat(swapchain), VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	// Sync objects creation
	VkFence fence = kvfCreateFence(device);
	VkSemaphore wait = kvfCreateSemaphore(device);
	VkSemaphore signal = kvfCreateSemaphore(device);

	// RenderPass creation
	VkAttachmentDescription attachment = kvfBuildSwapchainAttachmentDescription(swapchain, true);
	VkRenderPass renderpass = kvfCreateRenderPass(device, &attachment, 1, VK_PIPELINE_BIND_POINT_GRAPHICS);

	// Framebuffers creation
	VkFramebuffer* framebuffers = (VkFramebuffer*)calloc(sizeof(VkFramebuffer), swapchain_images_count);
	for(uint32_t i = 0; i < swapchain_images_count; i++)
		framebuffers[i] = kvfCreateFramebuffer(device, renderpass, &swapchain_images_views[i], 1, kvfGetSwapchainImagesSize(swapchain));

	// Pipeline creation
	VkShaderModule vertex_shader_module = kvfCreateShaderModule(device, (uint32_t*)vertex_shader, sizeof(vertex_shader) / sizeof(uint32_t));
	VkShaderModule fragment_shader_module = kvfCreateShaderModule(device, (uint32_t*)fragment_shader, sizeof(fragment_shader) / sizeof(uint32_t));

	VkPipelineLayout pipeline_layout = kvfCreatePipelineLayout(device, NULL, 0, NULL, 0);

	KvfGraphicsPipelineBuilder* builder = kvfCreateGPipelineBuilder();
	kvfGPipelineBuilderSetInputTopology(builder, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	kvfGPipelineBuilderSetPolygonMode(builder, VK_POLYGON_MODE_FILL, 1.0f);
	kvfGPipelineBuilderSetCullMode(builder, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
	kvfGPipelineBuilderAddShaderStage(builder, VK_SHADER_STAGE_VERTEX_BIT, vertex_shader_module, "main");
	kvfGPipelineBuilderAddShaderStage(builder, VK_SHADER_STAGE_FRAGMENT_BIT, fragment_shader_module, "main");
    	kvfGPipelineBuilderSetMultisampling(builder , VK_SAMPLE_COUNT_1_BIT);
	kvfGPipelineBuilderDisableDepthTest(builder);
	kvfGPipelineBuilderDisableBlending(builder);

	VkPipeline pipeline = kvfCreateGraphicsPipeline(device, VK_NULL_HANDLE, pipeline_layout, builder, renderpass);

	kvfDestroyGPipelineBuilder(builder);
	kvfDestroyShaderModule(device, vertex_shader_module);
	kvfDestroyShaderModule(device, fragment_shader_module);

	// Main command buffer creation
	VkCommandBuffer cmd = kvfCreateCommandBuffer(device);

	// Rendering loop
	for(size_t i = 0; i < 300; i++)
	{
		kvfWaitForFence(device, fence);
		uint32_t image_index;
		vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, wait, VK_NULL_HANDLE, &image_index);

		vkResetCommandBuffer(cmd, 0);
		kvfBeginCommandBuffer(cmd, 0);
			vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
			VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
			kvfBeginRenderPass(renderpass, cmd, framebuffers[image_index], kvfGetSwapchainImagesSize(swapchain), &clear_color, 1);
			VkViewport viewport = { 0 };
			viewport.width = kvfGetSwapchainImagesSize(swapchain).width;
			viewport.height = kvfGetSwapchainImagesSize(swapchain).height;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(cmd, 0, 1, &viewport);
			VkRect2D scissor = { 0 };
			scissor.extent = kvfGetSwapchainImagesSize(swapchain);
			vkCmdSetScissor(cmd, 0, 1, &scissor);
			vkCmdDraw(cmd, 3, 1, 0, 0);
			vkCmdEndRenderPass(cmd);
		kvfEndCommandBuffer(cmd);

		VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		kvfSubmitCommandBuffer(device, cmd, KVF_GRAPHICS_QUEUE, signal, wait, fence, wait_stages);
		kvfQueuePresentKHR(device, signal, swapchain, image_index);

		SDL_Delay(15);
	}

	// Cleanup
	vkDeviceWaitIdle(device);
	kvfDestroyPipelineLayout(device, pipeline_layout);
	kvfDestroyPipeline(device, pipeline);
	kvfDestroyRenderPass(device, renderpass);
	for(uint32_t i = 0; i < swapchain_images_count; i++)
	{
		kvfDestroyFramebuffer(device, framebuffers[i]);
		kvfDestroyImageView(device, swapchain_images_views[i]);
	}
	free(framebuffers);
	free(swapchain_images);
	free(swapchain_images_views);
	kvfDestroyFence(device, fence);
	kvfDestroySemaphore(device, wait);
	kvfDestroySemaphore(device, signal);
	kvfDestroySwapchainKHR(device, swapchain);
	vkDestroySurfaceKHR(instance, surface, NULL);
	kvfDestroyDevice(device);
	kvfDestroyInstance(instance);
	return 0;
}

```

<p align="center">
    <img src="https://github.com/Kbz-8/KVF/blob/46aa29be4facef398aeab0b7e82bff84aab683ce/sandbox/screenshot.png" alt="drawing" width="500"/>
</p>
