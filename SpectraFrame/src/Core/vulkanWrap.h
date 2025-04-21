/**
* Quick and dirty vulkan wrapper library that is made with intention of 
* cutting typical vulkan initialization code like setting up sType and 
* other default values that must be set. Information about default values
* set in constructors are provided in class comments
* 
* There could be lack of documentation in some parts of library!
* 
* If something is missing feel free to add it to wrapper.
* 
* 
* CHANGELOG:
* - 03.04.2025
*	Library is usable and can do something
* 
* - 06.04.2025
*	RenderPass is now wrapped into usable class
* 
* Have fun, Piotr UjemnyGH Plombon ;)
*/

// TODO: Classes, as well as source code would benefit from documentation

#pragma once
#ifndef _VULKAN_WRAP_
#define _VULKAN_WRAP_

// To delete later when I write my own loader, that gonna load only needed functions based on selected vulkan version
// Baseline class for wrapper already exist
#include <vulkan/vulkan.h>
// For showing errors in default debug utils messenger option
#include <iostream>
// For vectors all over the code
#include <vector>
// For assert
#include <cassert>
// For arrays
#include <array>
#include <unordered_map>

#define VKWNS vkw

namespace VKWNS {
	enum VkWrapCoreCreateFlagBits {
		VKWRAP_CORE_CREATE_DEBUG_MESSENGER = 0x1,
		VKWRAP_CORE_GET_GRAPHICS_QUEUE = 0x2,
		VKWRAP_CORE_GET_TRANSFER_QUEUE = 0x4,
		VKWRAP_CORE_GET_COMPUTE_QUEUE = 0x8,
		VKWRAP_CORE_CHECK_SWAPCHAIN_EXTENSION_SUPPORT = 0x10,
		// Selects best device as baseline, left this bit as reserved
		//VKWRAP_CORE_SELECT_BEST_PHYSICAL_DEVICE = 0x20,
	};

	typedef uint32_t VkWrapCoreCreateFlags;

	VkResult vkwAssert(VkResult result);

#pragma region VkWrapCore class declaration
	/**
	 * @brief Class wrapping all tedious part of vulkan in short and simple way
	 * 
	 * @brief Default values
	 * @param applicationInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
	 * @param applicationInfo.apiVersion = VK_API_VERSION_1_0;
	 * @param applicationInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	 * @param applicationInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	 * @param applicationInfo.pApplicationName = "DefualtApplicationName";
	 * @param applicationInfo.pEngineName = "DefaultEngineName";
	 * 
	 * @param instanceInfo = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
	 * @param instanceInfo.pApplicationInfo = &applicationInfo;
	 * 
	 * @param debugUtilsMessengerInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
	 * @param debugUtilsMessengerInfo.messageSeverity =
	 * @param 	VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
	 * @param 	| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
	 * @param 	| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
	 * @param 	| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	 * @param debugUtilsMessengerInfo.messageType =
	 * @param 	VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
	 * @param 	| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
	 * @param 	| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	 * @param debugUtilsMessengerInfo.pfnUserCallback = _debugUtilsMessengerCallback;
	 * @param debugUtilsMessengerInfo.pUserData = nullptr;
	 *
	 * @param mSurfaceCapabilities = {};
	 * 
	 * @param deviceEnabledFeatures = {};
	 * @param deviceEnabledFeatures.samplerAnisotropy = VK_TRUE;
	 * 
	 * @param deviceInfo = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
	 * @param deviceInfo.pEnabledFeatures = &deviceEnabledFeatures;
	 *		
	 */
	class VkWrapCore {
	private:
		// Core creation flags to simplify this repeating part of creating apps in vulkan
		VkWrapCoreCreateFlags mCoreCreateFlags;

		// Needed vulkan instance data
		std::vector<const char*> mInstanceEnabledLayers;
		std::vector<const char*> mInstanceEnabledExtensions;
		VkInstance mInstance;

		// Debug messenger handle
		VkDebugUtilsMessengerEXT mDebugUtilsMessenger;

		// Needed surface data
		VkSurfaceFormatKHR mSelectedSurfaceFormat;
		VkPresentModeKHR mSelectedSurfacePresentMode;
		std::vector<VkSurfaceFormatKHR> mAvailableSurfaceFormats;
		std::vector<VkPresentModeKHR> mAvailableSurfacePresentModes;
		VkSurfaceCapabilitiesKHR mSurfaceCapabilities;
		VkSurfaceKHR mSurface;

		// Physical devices (GPUs) data, and selected device
		VkPhysicalDevice mSelectedPhysicalDevice;
		std::vector<VkPhysicalDevice> mAvailablePhysicalDevices;

		// Device and queues needed data
		std::vector<const char*> mDeviceEnabledLayers;
		std::vector<const char*> mDeviceEnabledExtensions;

		std::array<VkDeviceQueueCreateInfo, 3> mDeviceQueueInfos;
		uint32_t mDeviceQueuesAmount = 0;
		float mDeviceQueuePriority = 1.0f;

		int32_t mGraphicsQueueIndex;
		int32_t mTransferQueueIndex;
		int32_t mComputeQueueIndex;
		VkQueue mGraphicsQueue;
		VkQueue mTransferQueue;
		VkQueue mComputeQueue;
		VkDevice mDevice;

		/**
		 * @brief Just a default callback to handle vulkan validation layer messages
		 * @param messageSeverity 
		 * @param messageTypes 
		 * @param pCallbackData 
		 * @param pUserData 
		 * @return 
		 */
		static VkBool32 _debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

		/**
		 * @brief Checks instance enabled layers support
		 * @return 
		 */
		bool _checkInstanceLayersSupport();

		/**
		 * @brief Gets available surface data from selected physical device (such as capabilities, formats and present modes)
		 */
		void _getAvailableSurfaceData();

		/**
		 * @brief Finds supported format from provided desiredFormats, first format in desiredFormats is most wanted
		 * @param desiredFormats 
		 * @param tiling 
		 * @param features 
		 * @return 
		 */
		VkFormat _findSupportedFormat(const std::vector<VkFormat>& desiredFormats, const VkImageTiling tiling, const VkFormatFeatureFlags features);

	public:
		// Public access for those who didn`t find appropriate function wrapping what they need
		// All this structs are set to default values in constructor so set what you want and rest should work
		VkApplicationInfo applicationInfo;
		VkInstanceCreateInfo instanceInfo;
		VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo;
		VkDeviceCreateInfo deviceInfo;
		VkPhysicalDeviceFeatures deviceEnabledFeatures;

		/**
		 * @brief Ya`ll know what constructor does
		 */
		VkWrapCore();

		/**
		 * @brief Also ya`ll should know what destructor does, tho it also destroys surface if provided so you shouldn`t worry about it
		 */
		~VkWrapCore();

		// Getters

		/**
		 * @brief Gets instance handle I guess
		 * @return 
		 */
		VkInstance& getInstance();

		/**
		 * @brief Gets surface handle if provided I guess
		 * @return 
		 */
		VkSurfaceKHR& getSurface();

		/**
		 * @brief Propably gets surface capabilities
		 * @return 
		 */
		VkSurfaceCapabilitiesKHR& getSurfaceCapabilities();

		/**
		 * @brief Should get surface desired format
		 * @return 
		 */
		VkSurfaceFormatKHR& getSurfaceFormat();

		/**
		 * @brief Is it getting desired present mode?
		 * @return 
		 */
		VkPresentModeKHR& getPresentMode();
		
		/**
		 * @brief You should of guessed that this gets selected physical device (most likely selected GPU)
		 * @return 
		 */
		VkPhysicalDevice& getGPU();

		/**
		 * @brief Gets all available physical devices, wild, I know
		 * @return 
		 */
		std::vector<VkPhysicalDevice>& getAvailableGPUs();

		/**
		 * @brief Unforseen function for getting graphics queue index
		 * @return 
		 */
		uint32_t getGraphicsQueueIndex();

		/**
		 * @brief Magnificent method if getting transfer queue index
		 * @return 
		 */
		uint32_t getTransferQueueIndex();
		
		/**
		 * @brief Marvelous way to get compute queue index
		 * @return 
		 */
		uint32_t getComputeQueueIndex();

		/**
		 * @brief Ostentatious method of getting graphics queue handle
		 * @return 
		 */
		VkQueue& getGraphicsQueue();

		/**
		 * @brief Glorious way of getting transfer queue handle
		 * @return 
		 */
		VkQueue& getTransferQueue();

		/**
		 * @brief Miraculous piece of functionality to get compute queue handle
		 * @return 
		 */
		VkQueue& getComputeQueue();

		/**
		 * @brief Dignified way to get device handle
		 * @return 
		 */
		VkDevice& getDevice();

		/**
		 * @brief Gets best depth format
		 * @return 
		 */
		VkFormat getDepthFormat();

		// Setters

		/**
		 * @brief Lofty yet much needed for graphics, method of setting surface
		 * @param rSurface 
		 * @return 
		 */
		VkWrapCore& setSurface(VkSurfaceKHR& rSurface);

		/**
		 * @brief Faultless way to set flags, flags for graphics are usually set automaticly using setSurface(<surface>) and/or graphicsRequiredExtensions(<os_specific_extension_name>)
		 * @see setSurface()
		 * @see graphicsRequiredExtensions()
		 * @param flags 
		 * @return 
		 */
		VkWrapCore& setFlags(const VkWrapCoreCreateFlags flags);

		/**
		 * @brief Remarkable way of overriding instance enabled layers
		 * @param rLayers 
		 * @return 
		 */
		VkWrapCore& setInstanceLayers(const std::vector<const char*>& rLayers);

		/**
		 * @brief Delightful method of overriding instance enabled extensions, for graphics use graphicsRequiredExtensions(<os_specific_extension_name>)
		 * @see graphicsRequiredExtensions()
		 * @param rExtensions 
		 * @return 
		 */
		VkWrapCore& setInstanceExtensions(const std::vector<const char*>& rExtensions);

		/**
		 * @brief Exquisite function of overriding device enabled layers
		 * @param rLayers 
		 * @return 
		 */
		VkWrapCore& setDeviceLayers(const std::vector<const char*>& rLayers);

		/**
		 * @brief Splendid way to override device enabled extensions, graphicsRequiredExtensions(<os_specific_extension_name>) sets it automaticly
		 * @see graphicsRequiredExtensions()
		 * @param rExtensions 
		 * @return 
		 */
		VkWrapCore& setDeviceExtensions(const std::vector<const char*>& rExtensions);

		// NOTE: Here we are, I do not have more synonyms to describe word "great", I think >:)

		// Autosetup functions

		/**
		 * @brief Changes current Vulkan API version from 1.0 to whatever user desires
		 * @param apiVersion 
		 * @return 
		 */
		VkWrapCore& apiVersion(const uint32_t apiVersion);

		/**
		 * @brief Changes application name from default name
		 * @param appName 
		 * @return 
		 */
		VkWrapCore& applicationName(const char* appName);

		/**
		 * @brief Changers engine name from default engine name
		 * @param engineName 
		 * @return 
		 */
		VkWrapCore& engineName(const char* engineName);

		/**
		 * @brief  Changes application version from 1.0.0 to whatever is specified
		 * @param appVersion 
		 * @return 
		 */
		VkWrapCore& applicationVersion(const uint32_t appVersion);

		/**
		 * @brief Changes engine version from 1.0.0 to whatever is specified
		 * @param engineVersion 
		 * @return 
		 */
		VkWrapCore& engineVersion(const uint32_t engineVersion);

		/**
		 * @brief Enables basic validation layer debug messaging
		 * @return 
		 */
		VkWrapCore& enableDebugMessenger(bool withInfo = false);

		/**
		 * @brief Setups required extension for graphics, there is no check if extension is already added so do it on your side
		 * @param systemDependandVulkanExtension you need to specify your system extension name eg. VK_KHR_WIN32_SURFACE_EXTENSION_NAME
		 * @return 
		 */
		VkWrapCore& graphicsRequiredExtensions(const char* systemDependantVulkanExtension);

		/**
		 * @brief Selects desired format if possible, must be used after pickGPU() method, default desired selection is {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}
		 * @see pickGPU()
		 * @default {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}
		 * @param desiredFormat 
		 * @return 
		 */
		VkWrapCore& selectSurfaceFormat(const VkSurfaceFormatKHR desiredFormat);

		/**
		 * @brief Selects desired present mode if possible, must be used after pickGPU() method, default desired selection is VK_PRESENT_MODE_FIFO_KHR
		 * @see pickGPU()
		 * @param desiredPresentMode 
		 * @return 
		 */
		VkWrapCore& selectPresentMode(const VkPresentModeKHR desiredPresentMode);

		/**
		 * @brief Refreshes surface capabilities, available formats and present modes (calls _getAvailableSurfaceData())
		 * @return 
		 */
		VkWrapCore& refreshSurfaceCapabilities();

		// create/pick functions

		/**
		 * @brief Creates instance from data set before
		 * @return 
		 */
		VkWrapCore& createInstance();

		/**
		 * @brief Picks best physical device (most likely discrete GPU if you have one), instance must be created before this method, setSurface(<surface_handle>) must be set before if you want do swapchain
		 * @return 
		 */
		VkWrapCore& pickGPU();

		/**
		 * @brief Creates logical device, physical device (GPU) must be picked before this method
		 * @return 
		 */
		VkWrapCore& createDevice();

		// Control functions

		/**
		 * @brief vkDeviceWaitIdle
		 * @return 
		 */
		VkWrapCore& waitForDeviceIdle();

		/**
		 * @brief Destroys all objects in VkWrapCore including surface if one exist
		 */
		void destroy();
	};
#pragma endregion

#pragma region Swapchain class declaration
	/**
	 * @brief Swapchain wrapper class
	 * 
	 * @brief Default values
	 * @param swapchainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	 * @param mSwapchainExtent = { 0, 0 };
	 * 
	 */
	class Swapchain {
	private:
		VkSwapchainKHR mSwapchain;
		VkExtent2D mSwapchainExtent;

		VkWrapCore* mCorePointer;

	public:
		VkSwapchainCreateInfoKHR swapchainInfo;
		std::vector<VkImage> swapchainImages;

		/**
		 * @brief Pointless description
		 */
		Swapchain();

		/**
		 * @brief Another pointless description
		 */
		~Swapchain();

		/**
		 * @brief Gets swapchain as vulkan object
		 * @return 
		 */
		VkSwapchainKHR& getSwapchain();

		/**
		 * @brief Gets pointer set in setCorePtr()
		 * @return 
		 */
		VkWrapCore* getCorePtr();

		/**
		 * @brief Gets extent set on creation of swapchain
		 * @return 
		 */
		VkExtent2D getExtent();

		/**
		 * @brief Gets amount of images, same as swapchainInfo.minImageCount, literally returns swapchainImages.size()
		 * @return 
		 */
		uint32_t getImagesAmount();

		/**
		 * @brief Sets VkWrapCore pointer
		 * @param pCore 
		 * @return 
		 */
		Swapchain& setCorePtr(VkWrapCore *const pCore);

		/**
		 * @brief Creates swapchain with specified extent (size) of images
		 * @param swapchainExtent 
		 * @return 
		 */
		Swapchain& create(const VkExtent2D swapchainExtent);

		/**
		 * @brief Destroy swapchain
		 */
		void destroy();
	};
#pragma endregion

#pragma region ImageView class declaration
	/**
	 * @brief ImageView wrapper class
	 * 
	 * @brief Default values
	 * @param imageViewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
	 * @param imageViewInfo.image = VK_NULL_HANDLE;
	 * @param imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	 * @param imageViewInfo.format = VK_FORMAT_UNDEFINED;
	 * @param imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
	 * @param imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
	 * @param imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
	 * @param imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
	 * @param imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	 * @param imageViewInfo.subresourceRange.baseArrayLayer = 0;
	 * @param imageViewInfo.subresourceRange.baseMipLevel = 0;
	 * @param imageViewInfo.subresourceRange.layerCount = 1;
	 * @param imageViewInfo.subresourceRange.levelCount = 1;
	 */
	class ImageView {
	private:
		VkImageView mImageView;

		VkWrapCore* mCorePointer;

	public:
		VkImageViewCreateInfo imageViewInfo;

		ImageView();

		~ImageView();

		/**
		 * @brief Gets vulkan image view object
		 * @return 
		 */
		VkImageView& getImageView();

		/**
		 * @brief Pointer to VkWrapCore
		 * @return 
		 */
		VkWrapCore* getCorePtr();

		/**
		 * @brief Sets pointer to VkWrapCore
		 * @param pCore 
		 * @return 
		 */
		ImageView& setCorePtr(VkWrapCore* const pCore);

		/**
		 * @brief Sets view type of image view
		 * @param viewType 
		 * @return 
		 */
		ImageView& viewType(const VkImageViewType viewType);

		/**
		 * @brief Sets format of image view
		 * @param format 
		 * @return 
		 */
		ImageView& format(const VkFormat format);

		ImageView& subresource(const VkImageAspectFlags aspectMask, const uint32_t baseArrayLayer = 0, const uint32_t baseMipLevel = 0, const uint32_t layerCount = 1, const uint32_t levelCount = 1);

		/**
		 * @brief Creates image view from image
		 * @param image 
		 * @return 
		 */
		ImageView& create(const VkImage image);

		/**
		 * @brief Destroys image view object if one exist
		 */
		void destroy();
	};
#pragma endregion

#pragma region Framebuffer class declaration
	/**
	 * @brief Framebuffer wrapper class
	 * 
	 * @brief Default values
	 * @param framebufferInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
	 * @param framebufferInfo.attachmentCount = 0;
	 * @param framebufferInfo.pAttachments = nullptr;
	 * @param framebufferInfo.renderPass = VK_NULL_HANDLE;
	 * @param framebufferInfo.layers = 1;
	 * @param framebufferInfo.width = 0;
	 * @param framebufferInfo.height = 0;
	 */
	class Framebuffer {
	private:
		VkFramebuffer mFramebuffer;

		VkWrapCore* mCorePointer;

	public:
		VkFramebufferCreateInfo framebufferInfo;
		std::vector<VkImageView> framebufferAttachments;

		Framebuffer();

		~Framebuffer();

		/**
		 * @brief Gets framebuffer vulkan object
		 * @return 
		 */
		VkFramebuffer& getFramebuffer();

		/**
		 * @brief Gets pointer to core class
		 * @return 
		 */
		VkWrapCore* getCorePtr();

		/**
		 * @brief Sets pointer to wrapper core
		 * @param pCore 
		 * @return 
		 */
		Framebuffer& setCorePtr(VkWrapCore* const pCore);

		/**
		 * @brief Sets framebuffer extent (size)
		 * @param extent 
		 * @return 
		 */
		Framebuffer& extent(const VkExtent2D extent);

		/**
		 * @brief Sets framebuffer render pass
		 * @param renderPass 
		 * @return 
		 */
		Framebuffer& renderPass(const VkRenderPass renderPass);

		/**
		 * @brief Adds attachment (VkImageView) to framebuffer
		 * @param attachment 
		 * @return 
		 */
		Framebuffer& addAttachment(const VkImageView attachment);

		/**
		 * @brief Clears all attachments
		 * @return 
		 */
		Framebuffer& clearAttachments();

		/**
		 * @brief Sets amount of framebuffer layers
		 * @param layers 
		 * @return 
		 */
		Framebuffer& layers(const uint32_t layers);

		/**
		 * @brief Creates framebuffer
		 * @return 
		 */
		Framebuffer& create();

		/**
		 * @brief Destroys framebuffer
		 */
		void destroy();
	};
#pragma endregion

#pragma region CommandPool class declaration
	/**
	 * @brief CommandPool wrapper class
	 * 
	 * @brief Default values
	 * @param commandPoolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	 */
	class CommandPool {
	private:
		VkCommandPool mCommandPool;

		VkWrapCore* mCorePointer;

	public:
		VkCommandPoolCreateInfo commandPoolInfo;

		CommandPool();

		~CommandPool();

		VkCommandPool& getCommandPool();

		VkWrapCore* getCorePtr();

		CommandPool& setCorePtr(VkWrapCore* const pCore);

		CommandPool& resetCommandBuffersFlag();

		CommandPool& transientFlag();

		CommandPool& protectedFlag();

		CommandPool& clearFlags();

		CommandPool& create(const uint32_t queueIndex);

		void destroy();
	};
#pragma endregion

#pragma region Buffer class declaration
	/**
	 * @brief Buffer wrapper class
	 * 
	 * @brief Default values
	 * @param bufferAllocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
	 * @param bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	 * @param bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	 * @param bufferInfo.size = 0;
	 * @param bufferInfo.usage = 0;
	 */
	class Buffer {
	private:
		VkBuffer mBuffer;
		VkDeviceMemory mBufferMemory;
		VkMemoryPropertyFlags mMemoryProperty;

		VkWrapCore* mCorePointer;

		uint32_t _findMemoryType();

	public:
		VkBufferCreateInfo bufferInfo;
		VkMemoryAllocateInfo bufferAllocInfo;

		Buffer();

		~Buffer();

		VkBuffer& getBuffer();

		VkDeviceMemory& getMemory();

		VkWrapCore* getCorePtr();

		Buffer& setCorePtr(VkWrapCore* const pCore);

		Buffer& usage(const VkBufferUsageFlags usage);

		Buffer& properties(const VkMemoryPropertyFlags property);

		Buffer& map(void** ppData, const VkDeviceSize size, const VkDeviceSize offset = 0);

		Buffer& unmap();

		Buffer& create(const uint32_t size);

		void destroy();
	};
#pragma endregion

#pragma region ShaderModule class declaration
	/**
	 * @brief ShaderModule wrapper class
	 * 
	 * @brief Default values
	 * @param shaderModuleInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
	 */
	class ShaderModule {
	private:
		VkShaderModule mShaderModule;
		VkWrapCore* mCorePointer;

	public:
		VkShaderModuleCreateInfo shaderModuleInfo;

		ShaderModule();

		~ShaderModule();

		VkShaderModule& getShaderModule();

		VkWrapCore* getCorePtr();

		ShaderModule& setCorePtr(VkWrapCore* const pCore);

		ShaderModule& create(const void* const srcData, const uint32_t size);

		void destroy();
	};
#pragma endregion

#pragma region Pipeline class declaration
	/**
	 * @brief Pipeline wrapper class
	 * 
	 * @brief Default values
	 * @param vertexInputState = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
	 * @param inputAssemblyState = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, nullptr, 0, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE };
	 * @param tessellationState = { VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO };
	 * @param viewportState = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
	 * @param rasterizationState = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, nullptr, 0, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f };
	 * @param multisampleState = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, nullptr, 0, VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 0.0f, nullptr, VK_FALSE, VK_FALSE };
	 * @param depthStencilState = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
	 * @param colorBlendState = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, nullptr, 0, VK_FALSE, VK_LOGIC_OP_COPY, 0, nullptr, {0.0f, 0.0f, 0.0f, 0.0f} };
	 * @param dynamicState = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, nullptr, 0, 0, nullptr };
	 * 
	 * @param graphicsPipelineInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, nullptr, 0, 0, nullptr, &vertexInputState, &inputAssemblyState, &tessellationState, &viewportState, &rasterizationState, &multisampleState, &depthStencilState, &colorBlendState, &dynamicState, VK_NULL_HANDLE, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, -1 };
	 * @param computePipelineInfo = { VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO, nullptr, 0, { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_COMPUTE_BIT, VK_NULL_HANDLE, "main", nullptr }, VK_NULL_HANDLE, VK_NULL_HANDLE, -1 };
	 */
	class Pipeline {
	private:
		VkPipeline mPipeline;
		VkWrapCore* mCorePointer;

		VkViewport mViewport;
		VkRect2D mScissor;

	public:
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		std::vector<VkDynamicState> dynamicStates;
		std::vector<VkVertexInputAttributeDescription> vertexInputAttributes;
		std::vector<VkVertexInputBindingDescription> vertexInputBindings;
		std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachmentStates;

		VkPipelineVertexInputStateCreateInfo vertexInputState;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
		VkPipelineTessellationStateCreateInfo tessellationState;
		VkPipelineViewportStateCreateInfo viewportState;
		VkPipelineRasterizationStateCreateInfo rasterizationState;
		VkPipelineMultisampleStateCreateInfo multisampleState;
		VkPipelineDepthStencilStateCreateInfo depthStencilState;
		VkPipelineColorBlendStateCreateInfo colorBlendState;
		VkPipelineDynamicStateCreateInfo dynamicState;

		VkGraphicsPipelineCreateInfo graphicsPipelineInfo;
		VkComputePipelineCreateInfo computePipelineInfo;

		Pipeline();

		~Pipeline();

		VkPipeline& getPipeline();

		VkWrapCore* getCorePtr();

		Pipeline& setCorePtr(VkWrapCore* const pCore);

		// Shader stage must be 1 per stage
		Pipeline& addShaderStage(const VkShaderModule pModule, const VkShaderStageFlagBits shaderStage, const char* const name = "main");

		Pipeline& clearShaderStages();

		Pipeline& addDynamicState(const VkDynamicState state);

		Pipeline& clearDynamicStates();

		Pipeline& addAttribute(const uint32_t location, const uint32_t binding, const VkFormat format, const uint32_t offset);

		Pipeline& clearAttributes();

		Pipeline& addBinding(const uint32_t binding, const uint32_t stride, const VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);

		Pipeline& clearBindings();

		Pipeline& addColorBlendAttachment(const VkPipelineColorBlendAttachmentState colorBlendAttachment);

		Pipeline& clearColorBlendAttachments();

		Pipeline& topology(const VkPrimitiveTopology topology);

		Pipeline& viewport(const VkExtent2D size, const float x = 0.0f, const float y = 0.0f, const float minDepth = 0.0f, const float maxDepth = 1.0f);

		Pipeline& scissor(const VkExtent2D size, const VkOffset2D offset = {0, 0});

		Pipeline& polygonMode(const VkPolygonMode polygonMode);

		Pipeline& cull(const VkCullModeFlags cullMode);

		Pipeline& frontFace(const VkFrontFace frontFace);

		Pipeline& renderPass(const VkRenderPass renderPass);

		Pipeline& depthStencilEnable(const VkBool32 testEnable, const VkBool32 writeEnable, const VkBool32 boundTestEnable, const VkBool32 stencilTestEnable);

		Pipeline& depthBounds(const float minDepth, const float maxDepth);

		/**
		 * @brief Usually VK_COMPARE_OP_LESS
		 * @param op 
		 * @return 
		 */
		Pipeline& depthCompareOp(const VkCompareOp op);

		Pipeline& layout(const VkPipelineLayout layout);

		Pipeline& createGraphics();

		Pipeline& createCompute();

		void destroy();
	};
#pragma endregion

#pragma region CommandBuffer class declaration
	/**
	 * @brief CommandBuffer wrapper class. CommandBuffer does not contain all vkCmd..() functions, I add rest later, for now use default vkCmd funcs from vulkan.h
	 * 
	 * @brief Default values
	 * @param commandBufferAllocateInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, nullptr, VK_NULL_HANDLE, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1 };
	 * @param commandBufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	 */
	class CommandBuffer {
	private:
		VkCommandBuffer mCommandBuffer;

		VkWrapCore* mCorePointer;

	public:
		VkCommandBufferAllocateInfo commandBufferAllocateInfo;
		VkCommandBufferBeginInfo commandBufferBeginInfo;

		CommandBuffer();

		~CommandBuffer();

		VkCommandBuffer& getCommandBuffer();

		VkWrapCore* getCorePtr();

		CommandBuffer& setCorePtr(VkWrapCore* const pCore);

		CommandBuffer& begin();

		CommandBuffer& end();

		CommandBuffer& reset(bool releaseResources = false);

		// VULKAN 1.0 COMMANDS

		CommandBuffer& cmdBindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);

		CommandBuffer& cmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports);

		CommandBuffer& cmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors);

		CommandBuffer& cmdSetLineWidth(float lineWidth);

		CommandBuffer& cmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);

		CommandBuffer& cmdSetBlendConstants(const float blendConstants[4]);

		CommandBuffer& cmdSetDepthBounds(float minDepthBounds, float maxDepthBounds);

		CommandBuffer& cmdSetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask);

		CommandBuffer& cmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask);

		CommandBuffer& cmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference);

		CommandBuffer& cmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
		
		CommandBuffer& cmdBindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);

		CommandBuffer& cmdBindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);

		CommandBuffer& cmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

		CommandBuffer& cmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);

		CommandBuffer& cmdDrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);

		CommandBuffer& cmdDrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);

		CommandBuffer& cmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

		CommandBuffer& cmdDispatchIndirect(VkBuffer buffer, VkDeviceSize offset);

		CommandBuffer& cmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions);

		CommandBuffer& cmdCopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions);

		CommandBuffer& cmdBlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter);

		CommandBuffer& cmdCopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions);

		CommandBuffer& cmdCopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions);

		CommandBuffer& cmdUpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData);

		CommandBuffer& cmdFillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);

		CommandBuffer& cmdClearColorImage(VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);

		CommandBuffer& cmdClearDepthStencilImage(VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges);

		CommandBuffer& cmdClearAttachments(uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects);

		CommandBuffer& cmdResolveImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions);

		CommandBuffer& cmdSetEvent(VkEvent event, VkPipelineStageFlags stageMask);

		CommandBuffer& cmdResetEvent(VkEvent event, VkPipelineStageFlags stageMask);

		CommandBuffer& cmdWaitEvents(uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
		
		CommandBuffer& cmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
		
		CommandBuffer& cmdBeginQuery(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags);
		
		CommandBuffer& cmdEndQuery(VkQueryPool queryPool, uint32_t query);
		
		CommandBuffer& cmdResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount);
		
		CommandBuffer& cmdWriteTimestamp(VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query);
		
		CommandBuffer& cmdCopyQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags);
		
		CommandBuffer& cmdPushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
		
		CommandBuffer& cmdBeginRenderPass(const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);
		
		CommandBuffer& cmdNextSubpass(VkSubpassContents contents);
		
		CommandBuffer& cmdEndRenderPass();
		
		CommandBuffer& cmdExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);

		// VULKAN 1.1 COMMANDS

		CommandBuffer& cmdSetDeviceMask(uint32_t deviceMask);

		CommandBuffer& cmdDispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

		// VULKAN 1.2 COMMANDS

		CommandBuffer& cmdDrawIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);

		CommandBuffer& cmdDrawIndexedIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride);

		CommandBuffer& cmdBeginRenderPass2(const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo);

		CommandBuffer& cmdNextSubpass2(const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo);

		CommandBuffer& cmdEndRenderPass2(const VkSubpassEndInfo* pSubpassEndInfo);

		// VULKAN 1.3 COMMANDS

		CommandBuffer& cmdSetEvent2(VkEvent event, const VkDependencyInfo* pDependencyInfo);

		CommandBuffer& cmdResetEvent2(VkEvent event, VkPipelineStageFlags2 stageMask);

		CommandBuffer& cmdWaitEvents2(uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos);

		CommandBuffer& cmdPipelineBarrier2(const VkDependencyInfo* pDependencyInfo);

		CommandBuffer& cmdWriteTimestamp2(VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query);

		CommandBuffer& cmdCopyBuffer2(const VkCopyBufferInfo2* pCopyBufferInfo);

		CommandBuffer& cmdCopyImage2(const VkCopyImageInfo2* pCopyImageInfo);

		CommandBuffer& cmdCopyBufferToImage2(const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo);

		CommandBuffer& cmdCopyImageToBuffer2(const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo);

		CommandBuffer& cmdBlitImage2(const VkBlitImageInfo2* pBlitImageInfo);

		CommandBuffer& cmdResolveImage2(const VkResolveImageInfo2* pResolveImageInfo);

		CommandBuffer& cmdBeginRendering(const VkRenderingInfo* pRenderingInfo);

		CommandBuffer& cmdEndRendering();

		CommandBuffer& cmdSetCullMode(VkCullModeFlags cullMode);

		CommandBuffer& cmdSetFrontFace(VkFrontFace frontFace);

		CommandBuffer& cmdSetPrimitiveTopology(VkPrimitiveTopology primitiveTopology);

		CommandBuffer& cmdSetViewportWithCount(uint32_t viewportCount, const VkViewport* pViewports);

		CommandBuffer& cmdSetScissorWithCount(uint32_t scissorCount, const VkRect2D* pScissors);

		CommandBuffer& cmdBindVertexBuffers2(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides);

		CommandBuffer& cmdSetDepthTestEnable(VkBool32 depthTestEnable);

		CommandBuffer& cmdSetDepthWriteEnable(VkBool32 depthWriteEnable);

		CommandBuffer& cmdSetDepthCompareOp(VkCompareOp depthCompareOp);

		CommandBuffer& cmdSetDepthBoundsTestEnable(VkBool32 depthBoundsTestEnable);

		CommandBuffer& cmdSetStencilTestEnable(VkBool32 stencilTestEnable);

		CommandBuffer& cmdSetStencilOp(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp);

		CommandBuffer& cmdSetRasterizerDiscardEnable(VkBool32 rasterizerDiscardEnable);

		CommandBuffer& cmdSetDepthBiasEnable(VkBool32 depthBiasEnable);
		
		CommandBuffer& cmdSetPrimitiveRestartEnable(VkBool32 primitiveRestartEnable);

		// VULKAN 1.4 COMMANDS

		CommandBuffer& cmdSetLineStipple(uint32_t lineStippleFactor, uint16_t lineStipplePattern);
		
		CommandBuffer& cmdBindIndexBuffer2(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType);
		
		CommandBuffer& cmdPushDescriptorSet(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
		
		CommandBuffer& cmdPushDescriptorSetWithTemplate(VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData);
		
		CommandBuffer& cmdSetRenderingAttachmentLocations(const VkRenderingAttachmentLocationInfo* pLocationInfo);
		
		CommandBuffer& cmdSetRenderingInputAttachmentIndices(const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo);
		
		CommandBuffer& cmdBindDescriptorSets2(const VkBindDescriptorSetsInfo* pBindDescriptorSetsInfo);
		
		CommandBuffer& cmdPushConstants2(const VkPushConstantsInfo* pPushConstantsInfo);
		
		CommandBuffer& cmdPushDescriptorSet2(const VkPushDescriptorSetInfo* pPushDescriptorSetInfo);

		CommandBuffer& cmdPushDescriptorSetWithTemplate2(const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo);

		CommandBuffer& primaryLevel();

		CommandBuffer& secondaryLevel();

		CommandBuffer& beginFlag(const VkCommandBufferUsageFlags beginUsageFlags);

		CommandBuffer& allocate(const VkCommandPool	commandPool);
	
		void destroy();
	};
#pragma endregion

#pragma region Semaphore class declaration
	/**
	 * @brief Semaphore wrapper class
	 * 
	 * @brief Default values
	 * @param semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
	 */
	class Semaphore {
	private:
		VkSemaphore mSemaphore;

		VkWrapCore* mCorePointer;

	public:
		VkSemaphoreCreateInfo semaphoreInfo;

		Semaphore();

		~Semaphore();

		VkSemaphore& getSemaphore();

		VkWrapCore* getCorePtr();

		Semaphore& setCorePtr(VkWrapCore* const pCore);

		Semaphore& create();

		void destroy();
	};
#pragma endregion

#pragma region Fence class declaration
	/**
	 * @brief Fence wrapper class
	 * 
	 * @brief Default values
	 * @param fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
	 */
	class Fence {
	private:
		VkFence mFence;

		VkWrapCore* mCorePointer;

	public:
		VkFenceCreateInfo fenceInfo;

		Fence();

		~Fence();

		VkFence& getFence();

		VkWrapCore* getCorePtr();

		Fence& setCorePtr(VkWrapCore* const pCore);

		Fence& signaled();

		Fence& reset();

		/**
		 * @brief Wait for this fence to be signaled
		 * @return 
		 */
		Fence& wait(const uint64_t timeout = (~0ULL));
		
		Fence& create();

		void destroy();
	};
#pragma endregion

#pragma region DescriptorSetLayout class declaration
	/**
	 * @brief DescriptorSetLayout wrapper class
	 */
	class DescriptorSetLayout {
	private:
		VkDescriptorSetLayout mDescriptorSetLayout;

		VkWrapCore* mCorePointer;

	public:
		std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;

		DescriptorSetLayout();

		~DescriptorSetLayout();

		VkDescriptorSetLayout& getSetLayout();

		VkWrapCore* getCorePtr();

		DescriptorSetLayout& setCorePtr(VkWrapCore* const pCore);

		DescriptorSetLayout& addBinding(const uint32_t binding, const uint32_t descriptorCount, const VkDescriptorType descriptorType, const VkShaderStageFlags stageFlags, const VkSampler* pImmutableSamplers = nullptr);

		DescriptorSetLayout& clearBindings();

		DescriptorSetLayout& create();

		void destroy();
	};
#pragma endregion

#pragma region DescriptorPool class declaration

	/**
	 * @brief DescriptorPool wrapper class
	 * 
	 * @brief Default values
	 * @param descriptorPoolInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
	 * @param descriptorPoolInfo.flags = 0;
	 * @param descriptorPoolInfo.maxSets = 0;
	 * @param descriptorPoolInfo.poolSizeCount = 0;
	 * @param descriptorPoolInfo.pPoolSizes = nullptr;
	 */
	class DescriptorPool {
	private:
		VkDescriptorPool mDescriptorPool;

		VkWrapCore* mCorePointer;

		std::vector<VkDescriptorPoolSize> mDescriptorPoolSizes;

	public:
		VkDescriptorPoolCreateInfo descriptorPoolInfo;

		DescriptorPool();

		~DescriptorPool();

		VkDescriptorPool& getPool();

		VkWrapCore* getCorePtr();

		DescriptorPool& setCorePtr(VkWrapCore* const pCore);

		DescriptorPool& addSize(const VkDescriptorType type, const uint32_t descriptorCount);

		DescriptorPool& clearSizes();

		DescriptorPool& maxSets(const uint32_t amount);

		DescriptorPool& flags(const VkDescriptorPoolCreateFlags flags);

		DescriptorPool& create();

		void destroy();
	};
#pragma endregion

#pragma region WriteDescriptorSet class declaration
	class WriteDescriptorSet {
	public:
		VkWriteDescriptorSet descriptorWriteSet;
		VkDescriptorBufferInfo descriptorBufferInfo;
		VkDescriptorImageInfo descriptorImageInfo;

		WriteDescriptorSet();

		WriteDescriptorSet& type(const VkDescriptorType descriptorType);

		WriteDescriptorSet& binding(const uint32_t bindingPoint);

		WriteDescriptorSet& arrayElement(const uint32_t element);

		WriteDescriptorSet& asUniformBuffer(const VkBuffer dataBuffer, const VkDeviceSize range, const VkDeviceSize offset = 0);

		WriteDescriptorSet& asSampler(const VkImageView imageView, const VkImageLayout imageLayout, const VkSampler sampler);

		WriteDescriptorSet& setDescriptorSet(const VkDescriptorSet set);
	};
#pragma endregion

#pragma region DescriptorSet class declaration
	class DescriptorSet {
	private:
		VkDescriptorSet mDescriptorSet;

		VkWrapCore* mCorePointer;

	public:
		VkDescriptorSetAllocateInfo descriptorSetAllocInfo;
		
		DescriptorSet();

		~DescriptorSet();

		VkDescriptorSet& getSet();

		VkWrapCore* getCorePtr();

		DescriptorSet& setCorePtr(VkWrapCore* const pCore);

		DescriptorSet& setLayouts(VkDescriptorSetLayout* const pLayouts);

		DescriptorSet& allocate(const VkDescriptorPool pool);

		DescriptorSet& update(const std::vector<VkWriteDescriptorSet>& writeDescriptorSets);

		DescriptorSet& update(const std::vector<WriteDescriptorSet>& writeDescriptorSets);

		DescriptorSet& update(VkWriteDescriptorSet& writeDescriptorSet);

		void destroy();
	};
#pragma endregion

#pragma region PipelineLayout class declaration
	/**
	 * @brief PipelineLayout wrapper class
	 * 
	 * @brief Default values
	 * @param pipelineLayoutInfo = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
	 * @param pipelineLayoutInfo.pushConstantRangeCount = 0;
	 * @param pipelineLayoutInfo.pPushConstantRanges = nullptr;
	 * @param pipelineLayoutInfo.setLayoutCount = 0;
	 * @param pipelineLayoutInfo.pSetLayouts = nullptr;
	 */
	class PipelineLayout {
	private:
		VkPipelineLayout mPipelineLayout;

		VkWrapCore* mCorePointer;

		std::vector<VkDescriptorSetLayout> mDescriptorLayouts;
		std::vector<VkPushConstantRange> mPushConstantRanges;

	public:
		VkPipelineLayoutCreateInfo pipelineLayoutInfo;

		PipelineLayout();

		~PipelineLayout();

		VkPipelineLayout& getLayout();

		VkWrapCore* getCorePtr();

		PipelineLayout& setCorePtr(VkWrapCore* const pCore);

		PipelineLayout& addLayouts(const VkDescriptorSetLayout setLayouts);

		PipelineLayout& addConstantRanges(const VkPushConstantRange pushConstantRanges);

		PipelineLayout& clearLayouts();

		PipelineLayout& clearConstantRanges();

		PipelineLayout& create();

		void destroy();
	};
#pragma endregion

#pragma region Submit class declaration
	/**
	 * @brief Submit wrapper class. Done for convinience
	 */
	class Submit {
	private:
		std::vector<VkSemaphore> mWaitSemaphores;
		std::vector<VkSemaphore> mSignalSemaphores;
		std::vector<VkPipelineStageFlags> mWaitDstStageMask;
		std::vector<VkCommandBuffer> mCommandBuffers;

	public:
		VkSubmitInfo submitInfo;
		
		Submit();

		~Submit();

		Submit& addWait(const VkSemaphore waitSemaphore);

		Submit& addSignal(const VkSemaphore signalSemaphore);

		Submit& addWaitDstMask(const VkPipelineStageFlags waitDstStageMask);

		Submit& addCommandBuffer(const VkCommandBuffer commandBuffer);

		VkResult submit(const VkQueue queue, const VkFence fence);
	};
#pragma endregion

#pragma region Present class declaration
	/**
	 * @brief Present wrapper class. Done for convinience
	 */
	class Present {
	private:
		std::vector<VkSemaphore> mWaitSemaphores;
		std::vector<VkSwapchainKHR> mSwapchains;

	public:
		VkPresentInfoKHR presentInfo;

		Present();

		~Present();

		Present& addWait(const VkSemaphore waitSemaphore);

		Present& addSwapchain(const VkSwapchainKHR swapchain);

		VkResult present(const VkQueue queue, const uint32_t* const pImageIndides);
	};
#pragma endregion

#pragma region Image class declaration
	/**
	 * @brief Image wrapper class
	 * 
	 * @brief Default values
	 * @param imageInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
	 * @param imageInfo.imageType = VK_IMAGE_TYPE_2D;
	 * @param imageInfo.extent = { 0, 0, 0 };
	 * @param imageInfo.mipLevels = 1;
	 * @param imageInfo.arrayLayers = 1;
	 * @param imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	 * @param imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	 * @param imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	 * @param imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
	 * @param imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	 * @param imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	 * @param imageInfo.flags = 0;
	 *
	 * @param imageAllocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
	 *  
	 * @param mMemoryProperty = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	 */
	class Image {
	private:
		VkImage mImage;
		VkDeviceMemory mImageMemory;
		VkMemoryPropertyFlags mMemoryProperty;

		VkWrapCore* mCorePointer;

		uint32_t _findMemoryType();

	public:
		VkImageCreateInfo imageInfo;
		VkMemoryAllocateInfo imageAllocInfo;

		Image();

		~Image();

		VkImage& getImage();

		VkDeviceMemory& getImageMemory();

		VkWrapCore* getCorePtr();

		Image& setCorePtr(VkWrapCore* const pCore);

		Image& extent(const VkExtent3D extent);

		Image& imageType(const VkImageType type);

		Image& mipmapLevels(const uint32_t levels);

		Image& arrayLayers(const uint32_t layers);

		Image& format(const VkFormat format);

		Image& tiling(const VkImageTiling tiling);

		Image& initialLayout(const VkImageLayout imageLayout);

		Image& usage(const VkImageUsageFlags imageUsage);

		Image& sharingMode(const VkSharingMode sharingMode);
		
		Image& samples(const VkSampleCountFlagBits samples);

		Image& flags(const VkImageCreateFlags imageFlags);

		Image& properties(const VkMemoryPropertyFlags memoryProperties);

		Image& create();

		void destroy();
	};
#pragma endregion

#pragma region Sampler class declaration
	/**
	 * @brief Sampler wrapper class
	 * 
	 * @brief Default values
	 * @param samplerInfo = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
	 * @param samplerInfo.magFilter = VK_FILTER_LINEAR;
	 * @param samplerInfo.minFilter = VK_FILTER_LINEAR;
	 * @param samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	 * @param samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	 * @param samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	 * @param samplerInfo.anisotropyEnable = VK_FALSE;
	 * @param samplerInfo.maxAnisotropy = 0;
	 * @param samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	 * @param samplerInfo.unnormalizedCoordinates = VK_FALSE;
	 * @param samplerInfo.compareEnable = VK_FALSE;
	 * @param samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	 * @param samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	 * @param samplerInfo.mipLodBias = 0.0f;
	 * @param samplerInfo.minLod = 0.0f;
	 * @param samplerInfo.maxLod = 0.0f;
	 */
	class Sampler {
	private:
		VkSampler mSampler;

		VkWrapCore* mCorePointer;

	public:
		VkSamplerCreateInfo samplerInfo;

		Sampler();

		~Sampler();

		VkSampler& getSampler();

		VkWrapCore* getCorePtr();

		Sampler& setCorePtr(VkWrapCore* const pCore);

		Sampler& magFilter(const VkFilter filter);

		Sampler& minFilter(const VkFilter filter);

		Sampler& addressMode(const VkSamplerAddressMode u, const VkSamplerAddressMode v, const VkSamplerAddressMode w);

		/**
		 * @brief 
		 * @param anisotropy 0 turn offs anisotropy and -1 sets it to maximal possible value, by default anisotropy is disabled
		 * @return 
		 */
		Sampler& maxAnisotropy(const int32_t anisotropy);

		Sampler& borderColor(const VkBorderColor color);

		Sampler& unnormalizedCoordinates();

		Sampler& normalizedCoordinates();

		Sampler& compare(const VkBool32 enable, const VkCompareOp op);

		Sampler& mipmap(const VkSamplerMipmapMode mode, const float lodBias, const float minLod, const float maxLod);

		Sampler& create();

		void destroy();
	};
#pragma endregion

#pragma region RenderPass class declaration
	class RenderPass {
	public:
		struct Subpass {
			std::vector<VkAttachmentReference2> subpassInputAttachmentRefs;
			std::vector<VkAttachmentReference2> subpassOutputAttachmentRefs;
			VkAttachmentReference2 subpassDepthAttachmentRef;
			bool depthRefExist;

			VkSubpassDescription2 subpassDescription;
			std::vector<VkSubpassDependency2> subpassDependencies;
		};

	private:
		VkRenderPass mRenderPass;

		std::vector<Subpass> mSubpasses;
		std::vector<VkAttachmentDescription2> mAttachments;
		std::unordered_map<std::string, uint32_t> mAttachmentIndices;

		VkWrapCore* mCorePointer;

	public:
		VkRenderPassCreateInfo2 renderPassInfo;

		RenderPass();

		~RenderPass();

		VkRenderPass& getRenderPass();

		std::vector<VkAttachmentDescription2>& getAttachments();

		std::vector<Subpass>& getSubpasses();

		uint32_t getAttachmentId(const std::string name);

		VkWrapCore* getCorePtr();

		RenderPass& setCorePtr(VkWrapCore* const pCore);

		RenderPass& addAttachment(const std::string name, const VkFormat format, const VkSampleCountFlagBits samples, const VkAttachmentLoadOp loadOp, const VkAttachmentStoreOp storeOp, const VkImageLayout finalLayout);

		RenderPass& clearAttachments();

		RenderPass& addSubpass(const bool withDepthReference);

		RenderPass& clearSubpasses();

		RenderPass& addSubpassDependency(const uint32_t subpassId, const uint32_t srcSubpass, const uint32_t dstSubpass, const VkPipelineStageFlags srcStage, const VkPipelineStageFlags dstStage, const VkAccessFlags srcAccess, const VkAccessFlags dstAccess);

		RenderPass& clearSubpassDependencies(const uint32_t subpassId);

		RenderPass& subpassAddInputRef(const uint32_t subpassId, const uint32_t attachment, const VkImageLayout layout, const VkImageAspectFlags aspectMask);

		RenderPass& subpassAddOutputRef(const uint32_t subpassId, const uint32_t attachment, const VkImageLayout layout, const VkImageAspectFlags aspectMask);

		RenderPass& clearSubpassRefs(const uint32_t subpassId);

		RenderPass& subpassDepthRef(const uint32_t subpassId, const uint32_t attachment, const VkImageLayout layout, const VkImageAspectFlags aspectMask);

		RenderPass& create();

		void destroy();
	};
#pragma endregion
}

#endif