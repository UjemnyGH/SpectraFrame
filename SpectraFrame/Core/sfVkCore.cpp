#include "sfVkCore.h"

std::unique_ptr<sf::Vulkan> sf::Vulkan::sVulkanInstancePtr = nullptr;

vk::Bool32 sf::Vulkan::_debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) ) {
  switch (messageSeverity) {
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		std::cout << "[VERBOSE] {VK VL";
		break;

	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		std::cout << "[INFO] {VK VL";
		break;

	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		std::cout << "[WARNING] {VK VL";
		break;

	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		std::cout << "[ERROR] {VK VL";
		break;

	default:
		std::cout << "[UNKNOWN] {VK VL";
		break;
	}

	switch (messageTypes) {
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		std::cout << " GENERAL}: ";
		break;

	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		std::cout << " VALIDATION}: ";
		break;

	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		std::cout << " PERFORMANCE}: ";
		break;

	default:
		std::cout << " UNKNOWN}: ";
		break;
	}

	std::cout << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

bool sf::Vulkan::_checkInstanceLayersSupport() {
  uint32_t count = 0;
	vkEnumerateInstanceLayerProperties(&count, nullptr);

	std::vector<VkLayerProperties> properties(count);
	vkEnumerateInstanceLayerProperties(&count, &properties[0]);

	for (const char* name : mInstanceEnabledLayers) {
		bool found = false;

		for (const VkLayerProperties layer : properties) {
			if (strcmp(name, layer.layerName) == 0) {
				found = true;

				break;
			}
		}

		if (!found) return false;
	}

	return true;
}

void sf::Vulkan::_getAvailableSurfaceData() {
 if(!mCheckSwapchainExtensionSupport) return;

  mSelectedPhysicalDevice.
}

vk::Format _findSupportedFormat(const std::vector<vk::Format>& desiredFormats, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features);


sf::Vulkan& sf::Vulkan::getVk() {
  return *sVulkanInstancePtr;
}

sf::Vulkan& sf::Vulkan::refreshCapabilities() {
  getVk().refreshSurfaceCapabilities();

  return getVk();
}

sf::Vulkan& sf::Vulkan::waitForIdle() {
  getVk().waitForDeviceIdle();

  return getVk();
}

vk::Instance& sf::Vulkan::instance() {
  return getVk().getInstance();
}

vk::SurfaceKHR& sf::Vulkan::surface() {
  return getVk().getSurface();
}

vk::SurfaceCapabilitiesKHR& sf::Vulkan::surfaceCapabilities() {
 return getVk().getSurfaceCapabilities(); 
}

vk::SurfaceFormatKHR& sf::Vulkan::surfaceFormat() {
  return getVk().getSurfaceFormat();
}

vk::PresentModeKHR& sf::Vulkan::presentMode() {
  return getVk().getPresentMode();
}

vk::PhysicalDevice& sf::Vulkan::gpu() {
  return getVk().getGPU();
}

std::vector<vk::PhysicalDevice>& sf::Vulkan::availableGPUs() {
  return getVk().getAvailableGPUs();
}

int32_t sf::Vulkan::graphicsQueueIndex() {
  return getVk().getGraphicsQueueIndex();
}

int32_t sf::Vulkan::transferQueueIndex() {
  return getVk().getTransferQueueIndex();
}

int32_t sf::Vulkan::computeQueueIndex() {
  return getVk().getComputeQueueIndex();
}

vk::Queue& sf::Vulkan::graphicsQueue() {
  return getVk().getGraphicsQueue();
}

vk::Queue& sf::Vulkan::transferQueue() {
  return getVk().getTransferQueue();
}

vk::Queue& sf::Vulkan::computeQueue() {
  return getVk().getComputeQueue();
}

vk::Device& sf::Vulkan::device() {
  return getVk().getDevice();
}

sf::Vulkan::Vulkan() {
  if(sVulkanInstancePtr.get() == nullptr) {
    sVulkanInstancePtr = std::make_unique<Vulkan>(this);
  }
}

sf::Vulkan::~Vulkan() {
  destroy();
}

vk::Instance& sf::Vulkan::getInstance() {
  return mInstance;
}

vk::SurfaceKHR& sf::Vulkan::getSurface() {
  return *mSurfacePtr;
}

vk::SurfaceCapabilitiesKHR& sf::Vulkan::getSurfaceCapabilities() {
  return mSurfaceCapabilities;
}

vk::SurfaceFormatKHR& sf::Vulkan::getSurfaceFormat() {
  return mSelectedSurfaceFormat;
}

vk::PresentModeKHR& sf::Vulkan::getPresentMode() {
  return mSelectedPresentMode;
}

vk::PhysicalDevice& sf::Vulkan::getGPU() {
  return mSelectedPhysicalDevice;
}

std::vector<vk::PhysicalDevice>& sf::Vulkan::getAvailableGPUs() {
  return mAvailablePhysicalDevices;
}

int32_t sf::Vulkan::getGraphicsQueueIndex() {
  return mGraphicsQueueIndex;
}

int32_t sf::Vulkan::getTransferQueueIndex() {
  return mTransferQueueIndex;
}

int32_t sf::Vulkan::getComputeQueueIndex() {
  return mComputeQueueIndex;
}

vk::Queue& sf::Vulkan::getGraphicsQueue() {
  return mGraphicsQueue;
}

vk::Queue& sf::Vulkan::getTransferQueue() {
  return mTransferQueue;
}

vk::Queue& sf::Vulkan::getComputeQueue() {
  return mComputeQueue;
}

vk::Device& sf::Vulkan::getDevice() {
  return mDevice;
}

sf::Vulkan& sf::Vulkan::setSurface(vk::SurfaceKHR& rSurface) {
  mSurfacePtr = &rSurface;

  return *this;
}

sf::Vulkan& sf::Vulkan::setInstanceLayers(const std::vector<const char*>& rLayers) {
  
}

Vulkan& setInstanceExtensions(const std::vector<const char*>& rExtensions);

Vulkan& setDeviceLayers(const std::vector<const char*>& rLayers);

Vulkan& setDeviceExtensions(const std::vector<const char*>& rExtensions);

Vulkan& apiVersion(const uint32_t apiVersion);

Vulkan& applicationName(const char* appName);

Vulkan& engineName(const char* engineName);

Vulkan& applicationVersion(const uint32_t appVersion);

Vulkan& engineVersion(const uint32_t engineVersion);

Vulkan& enableDebugMessenger(bool withInfo = false);

Vulkan& graphicsRequiredExtensions(const char* systemDependantVulkanExtension);

Vulkan& selectSurfaceFormat(const VkSurfaceFormatKHR desiredFormat);

Vulkan& selectPresentMode(const VkPresentModeKHR desiredPresentMode);

Vulkan& refreshSurfaceCapabilities();

Vulkan& createInstance();

Vulkan& pickGPU();

Vulkan& createDevice();

Vulkan& waitForDeviceIdle();

void destroy();
