#pragma once
#ifndef _SPECTRAFRAME_VK_CORE_
#define _SPECTRAFRAME_VK_CORE_

#include <vulkan/vulkan.hpp>
#include <memory>

namespace sf {
	class Vulkan {
	private:
    bool mCheckSwapchainExtensionSupport;
    bool mDefaultDebugMessenger;

    std::vector<const char*> mInstanceEnabledLayers;
		std::vector<const char*> mInstanceEnabledExtensions;
		vk::Instance mInstance;

		vk::DebugUtilsMessengerEXT mDebugUtilsMessenger;

		vk::SurfaceFormatKHR mSelectedSurfaceFormat;
		vk::PresentModeKHR mSelectedPresentMode;
		std::vector<vk::SurfaceFormatKHR> mAvailableSurfaceFormats;
		std::vector<vk::PresentModeKHR> mAvailablePresentModes;
		vk::SurfaceCapabilitiesKHR mSurfaceCapabilities;
		vk::SurfaceKHR* mSurfacePtr;
    
		vk::PhysicalDevice mSelectedPhysicalDevice;
		std::vector<vk::PhysicalDevice> mAvailablePhysicalDevices;

		std::vector<const char*> mDeviceEnabledLayers;
		std::vector<const char*> mDeviceEnabledExtensions;

		std::array<vk::DeviceQueueCreateInfo, 3> mDeviceQueueInfos;
		float mDeviceQueuePriority;

		int32_t mGraphicsQueueIndex;
		int32_t mTransferQueueIndex;
		int32_t mComputeQueueIndex;

		vk::Queue mGraphicsQueue;
		vk::Queue mTransferQueue;
		vk::Queue mComputeQueue;

		vk::Device mDevice;

		static vk::Bool32 _debugUtilsMessengerCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageTypes, const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

		bool _checkInstanceLayersSupport();

		void _getAvailableSurfaceData();

		vk::Format _findSupportedFormat(const std::vector<vk::Format>& desiredFormats, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features);

    bool _checkPhysicalDeviceSwapchainSupport(vk::PhysicalDevice& physicalDevice, const std::vector<vk::ExtensionProperties>& deviceProperties);

    uint32_t _scorePhysicalDevice(vk::PhysicalDevice& physicalDevice);

	public:
		vk::ApplicationInfo applicationInfo;
		vk::InstanceCreateInfo instanceInfo;
		vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo;
		vk::DeviceCreateInfo deviceInfo;
		vk::PhysicalDeviceFeatures deviceEnabledFeatures;

		static std::unique_ptr<Vulkan> sVulkanInstancePtr;

		static Vulkan& getVk();

		/**
		 * @brief Same as Vulkan::getVk().refreshSurfaceCapabilities()
		 */
		static Vulkan& refreshCapabilities();

		/**
		 * @brief Same as Vulkan::getVk().waitForDeviceIdle()
		 */
		static Vulkan& waitForIdle();

		/**
		 * @brief Gets instance handle I guess, Vulkan::getVk().getInstance()
		 * @return 
		 */
		static vk::Instance& instance();

		/**
		 * @brief Gets surface handle if provided I guess, Vulkan::getVk().getSurface()
		 * @return 
		 */
		static vk::SurfaceKHR& surface();

		/**
		 * @brief Propably gets surface capabilities, Vulkan::getVk().getSurfaceCapabilities()
		 * @return 
		 */
		static vk::SurfaceCapabilitiesKHR& surfaceCapabilities();

		/**
		 * @brief Should get surface desired format, Vulkan::getVk().getSurfaceFormat()
		 * @return 
		 */
		static vk::SurfaceFormatKHR& surfaceFormat();

		/**
		 * @brief Is it getting desired present mode?, Vulkan::getVk().getPresentMode()
		 * @return 
		 */
		static vk::PresentModeKHR& presentMode();

		/**
		 * @brief You should of guessed that this gets selected physical device (most likely selected GPU), Vulkan::getVk().getGPU()
		 * @return 
		 */
		static vk::PhysicalDevice& gpu();

		/**
		 * @brief Gets all available physical devices, wild, I know, Vulkan::getVk().getAvailableGPUs()
		 * @return 
		 */
		static std::vector<vk::PhysicalDevice>& availableGPUs();

		/**
		 * @brief Unforseen function for getting graphics queue index, Vulkan::getVk().getGraphicsQueueIndex()
		 * @return 
		 */
		static int32_t graphicsQueueIndex();

		/**
		 * @brief Magnificent method if getting transfer queue index, Vulkan::getVk().getTransferQueueIndex()
		 * @return 
		 */
		static int32_t transferQueueIndex();

		/**
		 * @brief Marvelous way to get compute queue index, Vulkan::getVk().getComputeQueueIndex()
		 * @return 
		 */
		static int32_t computeQueueIndex();

		/**
		 * @brief Ostentatious method of getting graphics queue handle, Vulkan::getVk().getGraphicsQueue()
		 * @return 
		 */
		static vk::Queue& graphicsQueue();

		/**
		 * @brief Glorious way of getting transfer queue handle, Vulkan::getVk().getTransferQueue()
		 * @return 
		 */
		static vk::Queue& transferQueue();

		/**
		 * @brief Miraculous piece of functionality to get compute queue handle, Vulkan::getVk().getComputeQueue()
		 * @return 
		 */
		static vk::Queue& computeQueue();

		/**
		 * @brief Dignified way to get device handle, Vulkan::getVk().getDevice()
		 * @return 
		 */
		static vk::Device& device();

		Vulkan();

		~Vulkan();

		/**
		 * @brief Gets instance handle I guess
		 * @return 
		 */
		vk::Instance& getInstance();

		/**
		 * @brief Gets surface handle if provided I guess
		 * @return 
		 */
		vk::SurfaceKHR& getSurface();

		/**
		 * @brief Propably gets surface capabilities
		 * @return 
		 */
		vk::SurfaceCapabilitiesKHR& getSurfaceCapabilities();

		/**
		 * @brief Should get surface desired format
		 * @return 
		 */
		vk::SurfaceFormatKHR& getSurfaceFormat();

		/**
		 * @brief Is it getting desired present mode?
		 * @return 
		 */
		vk::PresentModeKHR& getPresentMode();

		/**
		 * @brief You should of guessed that this gets selected physical device (most likely selected GPU)
		 * @return 
		 */
		vk::PhysicalDevice& getGPU();

		/**
		 * @brief Gets all available physical devices, wild, I know
		 * @return 
		 */
		std::vector<vk::PhysicalDevice>& getAvailableGPUs();

		/**
		 * @brief Unforseen function for getting graphics queue index
		 * @return 
		 */
		int32_t getGraphicsQueueIndex();

		/**
		 * @brief Magnificent method if getting transfer queue index
		 * @return 
		 */
		int32_t getTransferQueueIndex();

		/**
		 * @brief Marvelous way to get compute queue index
		 * @return 
		 */
		int32_t getComputeQueueIndex();

		/**
		 * @brief Ostentatious method of getting graphics queue handle
		 * @return 
		 */
		vk::Queue& getGraphicsQueue();

		/**
		 * @brief Glorious way of getting transfer queue handle
		 * @return 
		 */
		vk::Queue& getTransferQueue();

		/**
		 * @brief Miraculous piece of functionality to get compute queue handle
		 * @return 
		 */
		vk::Queue& getComputeQueue();

		/**
		 * @brief Dignified way to get device handle
		 * @return 
		 */
		vk::Device& getDevice();

		/**
		 * @brief Lofty yet much needed for graphics, method of setting surface
		 * @param rSurface 
		 * @return 
		 */
		Vulkan& setSurface(vk::SurfaceKHR& rSurface);

		/**
		 * @brief Remarkable way of overriding instance enabled layers
		 * @param rLayers 
		 * @return 
		 */
		Vulkan& setInstanceLayers(const std::vector<const char*>& rLayers);

		/**
		 * @brief Delightful method of overriding instance enabled extensions, for graphics use graphicsRequiredExtensions(<os_specific_extension_name>)
		 * @see graphicsRequiredExtensions()
		 * @param rExtensions 
		 * @return 
		 */
		Vulkan& setInstanceExtensions(const std::vector<const char*>& rExtensions);

		/**
		 * @brief Exquisite function of overriding device enabled layers
		 * @param rLayers 
		 * @return 
		 */
		Vulkan& setDeviceLayers(const std::vector<const char*>& rLayers);

		/**
		 * @brief Splendid way to override device enabled extensions, graphicsRequiredExtensions(<os_specific_extension_name>) sets it automaticly
		 * @see graphicsRequiredExtensions()
		 * @param rExtensions 
		 * @return 
		 */
		Vulkan& setDeviceExtensions(const std::vector<const char*>& rExtensions);

		// NOTE: Here we are, I do not have more synonyms to describe word "great", I think >:)

		// Autosetup functions

		/**
		 * @brief Enables basic validation layer debug messaging
		 * @return 
		 */
		Vulkan& enableDebugMessenger(bool withInfo = false);

		/**
		 * @brief Setups required extension for graphics, there is no check if extension is already added so do it on your side
		 * @param systemDependandVulkanExtension you need to specify your system extension name eg. VK_KHR_WIN32_SURFACE_EXTENSION_NAME
		 * @return 
		 */
		Vulkan& graphicsRequiredExtensions(const char* systemDependantVulkanExtension);

		/**
		 * @brief Selects desired format if possible, must be used after pickGPU() method, default desired selection is {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}
		 * @see pickGPU()
		 * @default {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}
		 * @param desiredFormat 
		 * @return 
		 */
		Vulkan& selectSurfaceFormat(const vk::SurfaceFormatKHR desiredFormat);

		/**
		 * @brief Selects desired present mode if possible, must be used after pickGPU() method, default desired selection is VK_PRESENT_MODE_FIFO_KHR
		 * @see pickGPU()
		 * @param desiredPresentMode 
		 * @return 
		 */
		Vulkan& selectPresentMode(const vk::PresentModeKHR desiredPresentMode);

		/**
		 * @brief Refreshes surface capabilities, available formats and present modes (calls _getAvailableSurfaceData())
		 * @return 
		 */
		Vulkan& refreshSurfaceCapabilities();

		// create/pick functions

		/**
		 * @brief Creates instance from data set before
		 * @return 
		 */
		Vulkan& createInstance();

		/**
		 * @brief Picks best physical device (most likely discrete GPU if you have one), instance must be created before this method, setSurface(<surface_handle>) must be set before if you want do swapchain
		 * @return 
		 */
		Vulkan& pickGPU();

		/**
		 * @brief Creates logical device, physical device (GPU) must be picked before this method
		 * @return 
		 */
		Vulkan& createDevice();

		// Control functions

		/**
		 * @brief vkDeviceWaitIdle
		 * @return 
		 */
		Vulkan& waitForDeviceIdle();

		/**
		 * @brief Destroys all objects in VkWrapCore including surface if one exist
		 */
		void destroy();
	};
}

#endif
