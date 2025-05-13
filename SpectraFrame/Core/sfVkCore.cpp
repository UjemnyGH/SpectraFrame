#include "sfVkCore.h"
#include <iostream>
#include <map>

std::unique_ptr<sf::Vulkan> sf::Vulkan::sVulkanInstancePtr = std::make_unique<Vulkan>();

vk::Bool32 sf::Vulkan::_debugUtilsMessengerCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageTypes, const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
  // Just check what message severity is and put message in console
  switch (messageSeverity) {
	case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
		std::cout << "[VERBOSE] {VK VL}: ";
		break;

	case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
		std::cout << "[INFO] {VK VL}: ";
		break;

  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
		std::cout << "[WARNING] {VK VL}: ";
		break;

	case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
		std::cout << "[ERROR] {VK VL}: ";
		break;

	default:
		std::cout << "[UNKNOWN] {VK VL}: ";
		break;
	}

  // TODO: Message types
	/*switch (messageTypes) {
  case vk::Flags<vk::DebugUtilsMessageTypeFlagBitsEXT>(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral):
		std::cout << " GENERAL}: ";
		break;

  case vk::Flags<vk::DebugUtilsMessageTypeFlagBitsEXT>(vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation):
		std::cout << " VALIDATION}: ";
		break;

  case vk::Flags<vk::DebugUtilsMessageTypeFlagBitsEXT>(vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance):
		std::cout << " PERFORMANCE}: ";
		break;

	default:
		std::cout << " UNKNOWN}: ";
		break;
	}*/

	std::cout << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

bool sf::Vulkan::_checkInstanceLayersSupport() {
  uint32_t count = 0;

  // Get all properties of and instance layer
  vk::enumerateInstanceLayerProperties(&count, nullptr);

	std::vector<vk::LayerProperties> properties(count);
  vk::enumerateInstanceLayerProperties(&count, &properties[0]);

  // Then check if we have desired layers
	for (const char* name : mInstanceEnabledLayers) {
		bool found = false;

		for (const VkLayerProperties layer : properties) {
      // If names match return true
			if (strcmp(name, layer.layerName) == 0) {
				found = true;

				break;
			}
		}
    
    // If names dont match return false
		if (!found) 
      return false;
	}

	return true;
}

void sf::Vulkan::_getAvailableSurfaceData() {
  // Chekc if we want to do graphics work (ofc we want, this is graphics/game engine)
  if(!mCheckSwapchainExtensionSupport) return;

  // Get currently used surface capabilities
  mSelectedPhysicalDevice.getSurfaceCapabilitiesKHR(*mSurfacePtr, &mSurfaceCapabilities);

  uint32_t formatCount = 0;

  // Get all available surface formats
  mSelectedPhysicalDevice.getSurfaceFormatsKHR(*mSurfacePtr, &formatCount, nullptr);
  mAvailableSurfaceFormats.resize(formatCount);
  mSelectedPhysicalDevice.getSurfaceFormatsKHR(*mSurfacePtr, &formatCount, &mAvailableSurfaceFormats[0]);

  uint32_t presentModeCount = 0;
  
  // Get all available surfrace present modes
  mSelectedPhysicalDevice.getSurfacePresentModesKHR(*mSurfacePtr, &presentModeCount, nullptr);
  mAvailablePresentModes.resize(presentModeCount);
  mSelectedPhysicalDevice.getSurfacePresentModesKHR(*mSurfacePtr, &presentModeCount, &mAvailablePresentModes[0]);
}

vk::Format sf::Vulkan::_findSupportedFormat(const std::vector<vk::Format>& desiredFormats, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features) {
  // Iterate through all desired formats
  for(vk::Format format : desiredFormats) {
    // Find desired format properties
    vk::FormatProperties formatProps;
    mSelectedPhysicalDevice.getFormatProperties(format, *formatProps);

    // Select best format
    if(tiling == vk::ImageTiling::eLinear && (formatProps.linearTilingFeatures & features) == features) 
      return format;
    else if(tiling == vk::ImageTiling::eOptimal && (formatProps.optimalTilingFeatures & features) == features)
      return format;
  }

  // In case of no availability choose this one
  return vk::Format::eD32SfloatS8Uint;
}

bool sf::Vulkan::_checkPhysicalDeviceSwapchainSupport(vk::PhysicalDevice& physicalDevice, const std::vector<vk::ExtensionProperties>& deviceProperties) {
  assert(mSurfacePtr && "There is no surface pointer provided");

  bool supportSwapchain = false;

  for(const vk::ExtensionProperties prop : deviceProperties) {
    if(strcmp(&prop.extensionName[0], VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
      supportSwapchain = true;

      break;
    }
  }

  if(supportSwapchain)
    return false;

  uint32_t formatCount = 0;

  physicalDevice.getSurfaceFormatsKHR(*mSurfacePtr, &formatCount, nullptr);
  std::vector<vk::SurfaceFormatKHR> formats(formatCount);
  physicalDevice.getSurfaceFormatsKHR(*mSurfacePtr, &formatCount, &formats[0]);

  uint32_t modeCount = 0;
  
  physicalDevice.getSurfacePresentModesKHR(*mSurfacePtr, &modeCount, nullptr);
  std::vector<vk::PresentModeKHR> modes(modeCount);
  physicalDevice.getSurfacePresentModesKHR(*mSurfacePtr, &modeCount, &modes[0]);

  return !formats.empty() && !modes.empty();
}


uint32_t sf::Vulkan::_scorePhysicalDevice(vk::PhysicalDevice& physicalDevice) {
  uint32_t currentScore = 0;

  uint32_t extensionCount = 0;

  physicalDevice.enumerateDeviceExtensionProperties(nullptr, &extensionCount, nullptr);
  std::vector<vk::ExtensionProperties> extensions(extensionCount);
  physicalDevice.enumerateDeviceExtensionProperties(nullptr, &extensionCount, &extensions[0]);

  if(mCheckSwapchainExtensionSupport && !_checkPhysicalDeviceSwapchainSupport(physicalDevice, extensions)) 
    return 0;

  vk::PhysicalDeviceFeatures currentFeatures;
  vk::PhysicalDeviceProperties currentProperties;

  physicalDevice.getFeatures(&currentFeatures);
  physicalDevice.getProperties(&currentProperties);

  if(currentProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
    currentScore += 10000;

  currentScore += currentProperties.limits.maxImageArrayLayers;

  uint32_t availableQueueCount = 0;

  physicalDevice.getQueueFamilyProperties(&availableQueueCount, nullptr);
  std::vector<vk::QueueFamilyProperties> queueProperties(availableQueueCount);
  physicalDevice.getQueueFamilyProperties(&availableQueueCount, &queueProperties[0]);

  uint32_t index = 0;

  for(vk::QueueFamilyProperties queues : queueProperties) {
    if(queues.queueFlags & vk::QueueFlagBits::eGraphics)
      currentScore += 1000;

    if(queues.queueFlags & vk::QueueFlagBits::eCompute)
      currentScore += 1000;

    if(queues.queueFlags & vk::QueueFlagBits::eTransfer)
      currentScore += 1000;

    vk::Bool32 surfaceSupported = vk::False;

    if(mCheckSwapchainExtensionSupport && mSurfacePtr)
      physicalDevice.getSurfaceSupportKHR(index, *mSurfacePtr);

    if(surfaceSupported)
      currentScore += 1000;

    index++;
  }

  return currentScore;
}

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
  /* 
  if(sVulkanInstancePtr == nullptr) {
    sVulkanInstancePtr = std::make_unique<Vulkan>();
  } 
  */ 

  applicationInfo = {
    "SpectraFrameEngine",
    VK_MAKE_API_VERSION(0, 1, 0, 0),
    "SpectraFrame",
    VK_MAKE_API_VERSION(0, 1, 0, 0),
    VK_API_VERSION_1_3
  };

  instanceInfo.setPApplicationInfo(&applicationInfo);

  debugUtilsMessengerInfo.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning);
  debugUtilsMessengerInfo.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
  debugUtilsMessengerInfo.setPfnUserCallback(Vulkan::_debugUtilsMessengerCallback);

  deviceEnabledFeatures.samplerAnisotropy = VK_TRUE;

  deviceInfo.pEnabledFeatures = &deviceEnabledFeatures;

  mCheckSwapchainExtensionSupport = false;
  mDefaultDebugMessenger = false;

  mGraphicsQueueIndex = -1;
  mTransferQueueIndex = -1;
  mComputeQueueIndex = -1;

  mSelectedSurfaceFormat = vk::SurfaceFormatKHR();
  mSelectedPresentMode = {};
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
  mInstanceEnabledLayers = rLayers;

  return *this;
}

sf::Vulkan& sf::Vulkan::setInstanceExtensions(const std::vector<const char*>& rExtensions) {
  mInstanceEnabledExtensions = rExtensions;

  return *this;
}

sf::Vulkan& sf::Vulkan::setDeviceLayers(const std::vector<const char*>& rLayers) {
  mDeviceEnabledLayers = rLayers;

  return *this;
}

sf::Vulkan& sf::Vulkan::setDeviceExtensions(const std::vector<const char*>& rExtensions) {
  mDeviceEnabledExtensions = rExtensions;
  
  return *this;
}

sf::Vulkan& sf::Vulkan::enableDebugMessenger(bool withInfo) {
  mDefaultDebugMessenger = true;

  if(withInfo)
    debugUtilsMessengerInfo.messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
  else
    debugUtilsMessengerInfo.messageSeverity &= ~vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;

  mInstanceEnabledLayers.push_back("VK_LAYER_KHRONOS_validation");

  return *this;
}

sf::Vulkan& sf::Vulkan::graphicsRequiredExtensions(const char* systemDependantVulkanExtension) {
  mCheckSwapchainExtensionSupport = true;

  mInstanceEnabledExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

  mInstanceEnabledExtensions.push_back(systemDependantVulkanExtension);

  mDeviceEnabledExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

  return *this;
}

sf::Vulkan& sf::Vulkan::selectSurfaceFormat(const vk::SurfaceFormatKHR desiredFormat) {
  mSelectedSurfaceFormat = mAvailableSurfaceFormats[0];

  for(const vk::SurfaceFormatKHR format : mAvailableSurfaceFormats) {
    if(format.colorSpace == desiredFormat.colorSpace && format.format == desiredFormat.format) {
      mSelectedSurfaceFormat = format;

      break;
    }
  }

  return *this;
}

sf::Vulkan& sf::Vulkan::selectPresentMode(const vk::PresentModeKHR desiredPresentMode) {
  mSelectedPresentMode = mAvailablePresentModes[0];

  for(const vk::PresentModeKHR mode : mAvailablePresentModes) {
    if(mode == desiredPresentMode) {
      mSelectedPresentMode = mode;

      break;
    }
  }

  return *this;
}

sf::Vulkan& sf::Vulkan::refreshSurfaceCapabilities() {
  _getAvailableSurfaceData();

  return *this;
}

sf::Vulkan& sf::Vulkan::createInstance() {
  if(mDefaultDebugMessenger && !_checkInstanceLayersSupport())
    std::cout << "[WARNING]: Some vulkan instance layers arent supported!" << std::endl;

  if(mDefaultDebugMessenger && _checkInstanceLayersSupport())
    instanceInfo.pNext = &debugUtilsMessengerInfo;

  if(mDefaultDebugMessenger)
    mInstanceEnabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  instanceInfo
    .setEnabledExtensionCount(mInstanceEnabledExtensions.empty() ? 0 : static_cast<uint32_t>(mInstanceEnabledExtensions.size()))
    .setEnabledLayerCount(mInstanceEnabledLayers.empty() ? 0 : static_cast<uint32_t>(mInstanceEnabledLayers.size()));

  instanceInfo.ppEnabledExtensionNames = mInstanceEnabledExtensions.empty() ? nullptr : &mInstanceEnabledExtensions[0];
  instanceInfo.ppEnabledLayerNames = mInstanceEnabledLayers.empty() ? nullptr : &mInstanceEnabledLayers[0];

  assert(vk::createInstance(&instanceInfo, nullptr, &mInstance) == vk::Result::eSuccess);

  if(mDefaultDebugMessenger)
    mInstance.createDebugUtilsMessengerEXT(&debugUtilsMessengerInfo, nullptr, &mDebugUtilsMessenger);

  return *this;
}

sf::Vulkan& sf::Vulkan::pickGPU() {
  uint32_t count = 0;

  mInstance.enumeratePhysicalDevices(&count, nullptr);
  mAvailablePhysicalDevices.resize(count);
  mInstance.enumeratePhysicalDevices(&count, &mAvailablePhysicalDevices[0]);

  std::map<uint32_t, vk::PhysicalDevice> scoredDevices;

  for(vk::PhysicalDevice physicalDevice : mAvailablePhysicalDevices) {
    uint32_t score = _scorePhysicalDevice(physicalDevice);

    scoredDevices.insert(std::make_pair(score, physicalDevice));
  }

  mSelectedPhysicalDevice = scoredDevices.rbegin()->second;

  assert(mSelectedPhysicalDevice && "Do you see this message? If yes then your vulkan drivers are outdated or your gpu is broken");

  _getAvailableSurfaceData();

  uint32_t selectedAvailableQueueFamiliesCount = 0;

  mSelectedPhysicalDevice.getQueueFamilyProperties(&selectedAvailableQueueFamiliesCount, nullptr);
  std::vector<vk::QueueFamilyProperties> selectedAvailableQueueFamilies(selectedAvailableQueueFamiliesCount);
  mSelectedPhysicalDevice.getQueueFamilyProperties(&selectedAvailableQueueFamiliesCount, &selectedAvailableQueueFamilies[0]);

  int32_t selectedQueueIndex = 0;

  for(const vk::QueueFamilyProperties queueFamilyProperty : selectedAvailableQueueFamilies) {
    if(queueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics) {
      vk::Bool32 surfaceSupported = vk::True;

      if(mSurfacePtr && mCheckSwapchainExtensionSupport) {
        surfaceSupported = vk::False;

        mSelectedPhysicalDevice.getSurfaceSupportKHR(selectedQueueIndex, *mSurfacePtr, &surfaceSupported);
      }

      if(surfaceSupported)
        mGraphicsQueueIndex = selectedQueueIndex;
    }

    if(queueFamilyProperty.queueFlags & vk::QueueFlagBits::eCompute)
      mComputeQueueIndex = selectedQueueIndex;

    if(queueFamilyProperty.queueFlags & vk::QueueFlagBits::eTransfer)
      mTransferQueueIndex = selectedQueueIndex;

    if(mGraphicsQueueIndex != -1 && mComputeQueueIndex != -1 && mTransferQueueIndex != -1)
      break;

    selectedQueueIndex++;
  }

  assert(mGraphicsQueueIndex != -1 && mComputeQueueIndex != -1 && mTransferQueueIndex != -1);

  if(mCheckSwapchainExtensionSupport) {
    selectPresentMode(vk::PresentModeKHR::eFifo);
    selectSurfaceFormat({vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear});
  }

  return *this;
}

sf::Vulkan& sf::Vulkan::createDevice() {
  assert(mSelectedPhysicalDevice && "Physical device (GPU) is not selected!");

  // Array could be moved here 
  mDeviceQueueInfos[0]
    .setPQueuePriorities(&mDeviceQueuePriority)
    .setQueueCount(1)
    .setQueueFamilyIndex(mGraphicsQueueIndex);

  mDeviceQueueInfos[1]
    .setPQueuePriorities(&mDeviceQueuePriority)
    .setQueueCount(1)
    .setQueueFamilyIndex(mComputeQueueIndex);

  mDeviceQueueInfos[2]
    .setPQueuePriorities(&mDeviceQueuePriority)
    .setQueueCount(1)
    .setQueueFamilyIndex(mTransferQueueIndex);

  deviceInfo
    .setQueueCreateInfoCount(static_cast<uint32_t>(mDeviceQueueInfos.size()))
    .setPQueueCreateInfos(&mDeviceQueueInfos[0])
    .setEnabledExtensionCount(mDeviceEnabledExtensions.empty() ? 0 : static_cast<uint32_t>(mDeviceEnabledExtensions.size()))
    .setEnabledLayerCount(mDeviceEnabledLayers.empty() ? 0 : static_cast<uint32_t>(mDeviceEnabledLayers.size()));

  deviceInfo.ppEnabledLayerNames = mDeviceEnabledLayers.empty() ? nullptr : &mDeviceEnabledLayers[0];
  deviceInfo.ppEnabledExtensionNames = mDeviceEnabledExtensions.empty() ? nullptr : &mDeviceEnabledExtensions[0];

  assert(mSelectedPhysicalDevice.createDevice(&deviceInfo, nullptr, &mDevice) == vk::Result::eSuccess);

  mDevice.getQueue(mGraphicsQueueIndex, 0, &mGraphicsQueue);
  mDevice.getQueue(mComputeQueueIndex, 0, &mComputeQueue);
  mDevice.getQueue(mTransferQueueIndex, 0, &mTransferQueue);

  return *this;
}

sf::Vulkan& sf::Vulkan::waitForDeviceIdle() {
  if((VkDevice)mDevice) 
    mDevice.waitIdle();

  return *this;
}

void sf::Vulkan::destroy() {
  if((VkDevice)mDevice)
    mDevice.destroy(nullptr);

  if(mSurfacePtr)
    mInstance.destroySurfaceKHR(*mSurfacePtr, nullptr);

  if((VkDebugUtilsMessengerEXT)mDebugUtilsMessenger)
    mInstance.destroyDebugUtilsMessengerEXT(mDebugUtilsMessenger, nullptr);

  if((VkInstance)mInstance)
    mInstance.destroy(nullptr);
}
