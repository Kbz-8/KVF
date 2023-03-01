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
 */

#ifndef KBZ_8_VULKAN_FRAMEWORK_H
#define KBZ_8_VULKAN_FRAMEWORK_H

#ifndef KVF_IMPL_VK_NO_PROTOTYPES
	#include <vulkan/vulkan.h>
#endif // KVF_IMPL_VK_NO_PROTOTYPES

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	bool enableValidationLayers;
} KvfInstanceDesc;

VkInstance kvfCreateInstance(KvfInstanceDesc description);

#ifdef __cplusplus
}
#endif

#ifdef KVF_IMPLEMENTATION

#endif // KVF_IMPLEMENTATION

#endif // KBZ_8_VULKAN_FRAMEWORK_H
