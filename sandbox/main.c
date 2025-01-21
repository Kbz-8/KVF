#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_vulkan.h>
#include <stdint.h>
#include <stdlib.h>
#define KVF_IMPLEMENTATION
#define KVF_ENABLE_VALIDATION_LAYERS
#include "../kvf.h"

static const uint32_t vertex_shader[] = {
	0x07230203,0x00010000,0x000d000b,0x00000036,0x00000000,0x00020011,0x00000001,0x0006000b,0x00000001,0x4c534c47,0x6474732e,0x3035342e,
	0x00000000,0x0003000e,0x00000000,0x00000001,0x0008000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x00000022,0x00000026,0x00000031,
	0x00030003,0x00000002,0x000001c2,0x000a0004,0x475f4c47,0x4c474f4f,0x70635f45,0x74735f70,0x5f656c79,0x656e696c,0x7269645f,0x69746365,
	0x00006576,0x00080004,0x475f4c47,0x4c474f4f,0x6e695f45,0x64756c63,0x69645f65,0x74636572,0x00657669,0x00040005,0x00000004,0x6e69616d,
	0x00000000,0x00050005,0x0000000c,0x69736f70,0x6e6f6974,0x00000073,0x00040005,0x00000017,0x6f6c6f63,0x00007372,0x00060005,0x00000020,
	0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000020,0x00000000,0x505f6c67,0x7469736f,0x006e6f69,0x00070006,0x00000020,
	0x00000001,0x505f6c67,0x746e696f,0x657a6953,0x00000000,0x00070006,0x00000020,0x00000002,0x435f6c67,0x4470696c,0x61747369,0x0065636e,
	0x00070006,0x00000020,0x00000003,0x435f6c67,0x446c6c75,0x61747369,0x0065636e,0x00030005,0x00000022,0x00000000,0x00060005,0x00000026,
	0x565f6c67,0x65747265,0x646e4978,0x00007865,0x00050005,0x00000031,0x67617266,0x6f6c6f43,0x00000072,0x00050048,0x00000020,0x00000000,
	0x0000000b,0x00000000,0x00050048,0x00000020,0x00000001,0x0000000b,0x00000001,0x00050048,0x00000020,0x00000002,0x0000000b,0x00000003,
	0x00050048,0x00000020,0x00000003,0x0000000b,0x00000004,0x00030047,0x00000020,0x00000002,0x00040047,0x00000026,0x0000000b,0x0000002a,
	0x00040047,0x00000031,0x0000001e,0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,0x00000020,
	0x00040017,0x00000007,0x00000006,0x00000002,0x00040015,0x00000008,0x00000020,0x00000000,0x0004002b,0x00000008,0x00000009,0x00000003,
	0x0004001c,0x0000000a,0x00000007,0x00000009,0x00040020,0x0000000b,0x00000006,0x0000000a,0x0004003b,0x0000000b,0x0000000c,0x00000006,
	0x0004002b,0x00000006,0x0000000d,0x00000000,0x0004002b,0x00000006,0x0000000e,0xbf000000,0x0005002c,0x00000007,0x0000000f,0x0000000d,
	0x0000000e,0x0004002b,0x00000006,0x00000010,0x3f000000,0x0005002c,0x00000007,0x00000011,0x00000010,0x00000010,0x0005002c,0x00000007,
	0x00000012,0x0000000e,0x00000010,0x0006002c,0x0000000a,0x00000013,0x0000000f,0x00000011,0x00000012,0x00040017,0x00000014,0x00000006,
	0x00000003,0x0004001c,0x00000015,0x00000014,0x00000009,0x00040020,0x00000016,0x00000006,0x00000015,0x0004003b,0x00000016,0x00000017,
	0x00000006,0x0004002b,0x00000006,0x00000018,0x3f800000,0x0006002c,0x00000014,0x00000019,0x00000018,0x0000000d,0x0000000d,0x0006002c,
	0x00000014,0x0000001a,0x0000000d,0x00000018,0x0000000d,0x0006002c,0x00000014,0x0000001b,0x0000000d,0x0000000d,0x00000018,0x0006002c,
	0x00000015,0x0000001c,0x00000019,0x0000001a,0x0000001b,0x00040017,0x0000001d,0x00000006,0x00000004,0x0004002b,0x00000008,0x0000001e,
	0x00000001,0x0004001c,0x0000001f,0x00000006,0x0000001e,0x0006001e,0x00000020,0x0000001d,0x00000006,0x0000001f,0x0000001f,0x00040020,
	0x00000021,0x00000003,0x00000020,0x0004003b,0x00000021,0x00000022,0x00000003,0x00040015,0x00000023,0x00000020,0x00000001,0x0004002b,
	0x00000023,0x00000024,0x00000000,0x00040020,0x00000025,0x00000001,0x00000023,0x0004003b,0x00000025,0x00000026,0x00000001,0x00040020,
	0x00000028,0x00000006,0x00000007,0x00040020,0x0000002e,0x00000003,0x0000001d,0x00040020,0x00000030,0x00000003,0x00000014,0x0004003b,
	0x00000030,0x00000031,0x00000003,0x00040020,0x00000033,0x00000006,0x00000014,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,
	0x000200f8,0x00000005,0x0003003e,0x0000000c,0x00000013,0x0003003e,0x00000017,0x0000001c,0x0004003d,0x00000023,0x00000027,0x00000026,
	0x00050041,0x00000028,0x00000029,0x0000000c,0x00000027,0x0004003d,0x00000007,0x0000002a,0x00000029,0x00050051,0x00000006,0x0000002b,
	0x0000002a,0x00000000,0x00050051,0x00000006,0x0000002c,0x0000002a,0x00000001,0x00070050,0x0000001d,0x0000002d,0x0000002b,0x0000002c,
	0x0000000d,0x00000018,0x00050041,0x0000002e,0x0000002f,0x00000022,0x00000024,0x0003003e,0x0000002f,0x0000002d,0x0004003d,0x00000023,
	0x00000032,0x00000026,0x00050041,0x00000033,0x00000034,0x00000017,0x00000032,0x0004003d,0x00000014,0x00000035,0x00000034,0x0003003e,
	0x00000031,0x00000035,0x000100fd,0x00010038
};

static const uint32_t fragment_shader[] = {
	0x07230203,0x00010000,0x000d000b,0x00000013,0x00000000,0x00020011,0x00000001,0x0006000b,0x00000001,0x4c534c47,0x6474732e,0x3035342e,
	0x00000000,0x0003000e,0x00000000,0x00000001,0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000c,0x00030010,
	0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x000a0004,0x475f4c47,0x4c474f4f,0x70635f45,0x74735f70,0x5f656c79,0x656e696c,
	0x7269645f,0x69746365,0x00006576,0x00080004,0x475f4c47,0x4c474f4f,0x6e695f45,0x64756c63,0x69645f65,0x74636572,0x00657669,0x00040005,
	0x00000004,0x6e69616d,0x00000000,0x00050005,0x00000009,0x4374756f,0x726f6c6f,0x00000000,0x00050005,0x0000000c,0x67617266,0x6f6c6f43,
	0x00000072,0x00040047,0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000c,0x0000001e,0x00000000,0x00020013,0x00000002,0x00030021,
	0x00000003,0x00000002,0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
	0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040017,0x0000000a,0x00000006,0x00000003,0x00040020,0x0000000b,0x00000001,
	0x0000000a,0x0004003b,0x0000000b,0x0000000c,0x00000001,0x0004002b,0x00000006,0x0000000e,0x3f800000,0x00050036,0x00000002,0x00000004,
	0x00000000,0x00000003,0x000200f8,0x00000005,0x0004003d,0x0000000a,0x0000000d,0x0000000c,0x00050051,0x00000006,0x0000000f,0x0000000d,
	0x00000000,0x00050051,0x00000006,0x00000010,0x0000000d,0x00000001,0x00050051,0x00000006,0x00000011,0x0000000d,0x00000002,0x00070050,
	0x00000007,0x00000012,0x0000000f,0x00000010,0x00000011,0x0000000e,0x0003003e,0x00000009,0x00000012,0x000100fd,0x00010038
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
	kvfGPipelineBuilderSetMultisampling(builder, VK_SAMPLE_COUNT_1_BIT);
	kvfGPipelineBuilderAddShaderStage(builder, VK_SHADER_STAGE_VERTEX_BIT, vertex_shader_module, "main");
	kvfGPipelineBuilderAddShaderStage(builder, VK_SHADER_STAGE_FRAGMENT_BIT, fragment_shader_module, "main");
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
