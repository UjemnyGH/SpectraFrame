#include "vulkanWrap.h"
// For map and it`s ordering functionality
#include <map>

VkResult VKWNS::vkwAssert(VkResult result) {
	switch (result) {
	case VK_SUCCESS: return result;
	case VK_NOT_READY: std::cout << "VKW Assertion error: VK_NOT_READY" << std::endl; return result;
	case VK_TIMEOUT: std::cout << "VKW Assertion error: VK_TIMEOUT" << std::endl; return result;
	case VK_EVENT_SET: std::cout << "VKW Assertion error: VK_EVENT_SET" << std::endl; return result;
	case VK_EVENT_RESET: std::cout << "VKW Assertion error: VK_EVENT_RESET" << std::endl; return result;
	case VK_INCOMPLETE: std::cout << "VKW Assertion error: VK_INCOMPLETE" << std::endl; return result;
	case VK_ERROR_OUT_OF_HOST_MEMORY: std::cout << "VKW Assertion error: VK_ERROR_OUT_OF_HOST_MEMORY" << std::endl; return result;
	case VK_ERROR_OUT_OF_DEVICE_MEMORY: std::cout << "VKW Assertion error: VK_ERROR_OUT_OF_DEVICE_MEMORY" << std::endl; return result;
	case VK_ERROR_INITIALIZATION_FAILED: std::cout << "VKW Assertion error: VK_ERROR_INITIALIZATION_FAILED" << std::endl; return result;
	case VK_ERROR_DEVICE_LOST: std::cout << "VKW Assertion error: VK_ERROR_DEVICE_LOST" << std::endl; return result;
	case VK_ERROR_MEMORY_MAP_FAILED: std::cout << "VKW Assertion error: VK_ERROR_MEMORY_MAP_FAILED" << std::endl; return result;
	case VK_ERROR_LAYER_NOT_PRESENT: std::cout << "VKW Assertion error: VK_ERROR_LAYER_NOT_PRESENT" << std::endl; return result;
	case VK_ERROR_EXTENSION_NOT_PRESENT: std::cout << "VKW Assertion error: VK_ERROR_EXTENSION_NOT_PRESENT" << std::endl; return result;
	case VK_ERROR_FEATURE_NOT_PRESENT: std::cout << "VKW Assertion error: VK_ERROR_FEATURE_NOT_PRESENT" << std::endl; return result;
	case VK_ERROR_INCOMPATIBLE_DRIVER: std::cout << "VKW Assertion error: VK_ERROR_INCOMPATIBLE_DRIVER" << std::endl; return result;
	case VK_ERROR_TOO_MANY_OBJECTS: std::cout << "VKW Assertion error: VK_ERROR_TOO_MANY_OBJECTS" << std::endl; return result;
	case VK_ERROR_FORMAT_NOT_SUPPORTED: std::cout << "VKW Assertion error: VK_ERROR_FORMAT_NOT_SUPPORTED" << std::endl; return result;
	case VK_ERROR_FRAGMENTED_POOL: std::cout << "VKW Assertion error: VK_ERROR_FRAGMENTED_POOL" << std::endl; return result;
	case VK_ERROR_UNKNOWN: std::cout << "VKW Assertion error: VK_ERROR_UNKNOWN" << std::endl; return result;
	case VK_ERROR_OUT_OF_POOL_MEMORY: std::cout << "VKW Assertion error: VK_ERROR_OUT_OF_POOL_MEMORY" << std::endl; return result;
	case VK_ERROR_INVALID_EXTERNAL_HANDLE: std::cout << "VKW Assertion error: VK_ERROR_INVALID_EXTERNAL_HANDLE" << std::endl; return result;
	case VK_ERROR_FRAGMENTATION: std::cout << "VKW Assertion error: VK_ERROR_FRAGMENTATION" << std::endl; return result;
	case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: std::cout << "VKW Assertion error: VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS" << std::endl; return result;
	case VK_PIPELINE_COMPILE_REQUIRED: std::cout << "VKW Assertion error: VK_PIPELINE_COMPILE_REQUIRED" << std::endl; return result;
	case VK_ERROR_NOT_PERMITTED: std::cout << "VKW Assertion error: VK_ERROR_NOT_PERMITTED" << std::endl; return result;
	case VK_ERROR_SURFACE_LOST_KHR: std::cout << "VKW Assertion error: VK_ERROR_SURFACE_LOST_KHR" << std::endl; return result;
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: std::cout << "VKW Assertion error: VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" << std::endl; return result;
	case VK_SUBOPTIMAL_KHR: std::cout << "VKW Assertion error: VK_SUBOPTIMAL_KHR" << std::endl; return result;
	case VK_ERROR_OUT_OF_DATE_KHR: std::cout << "VKW Assertion error: VK_ERROR_OUT_OF_DATE_KHR" << std::endl; return result;
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: std::cout << "VKW Assertion error: VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" << std::endl; return result;
	case VK_ERROR_VALIDATION_FAILED_EXT: std::cout << "VKW Assertion error: VK_ERROR_VALIDATION_FAILED_EXT" << std::endl; return result;
	case VK_ERROR_INVALID_SHADER_NV: std::cout << "VKW Assertion error: VK_ERROR_INVALID_SHADER_NV" << std::endl; return result;
	case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: std::cout << "VKW Assertion error: VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR" << std::endl; return result;
	case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: std::cout << "VKW Assertion error: VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR" << std::endl; return result;
	case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: std::cout << "VKW Assertion error: VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR" << std::endl; return result;
	case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: std::cout << "VKW Assertion error: VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR" << std::endl; return result;
	case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: std::cout << "VKW Assertion error: VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR" << std::endl; return result;
	case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: std::cout << "VKW Assertion error: VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR" << std::endl; return result;
	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: std::cout << "VKW Assertion error: VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT" << std::endl; return result;
	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: std::cout << "VKW Assertion error: VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT" << std::endl; return result;
	case VK_THREAD_IDLE_KHR: std::cout << "VKW Assertion error: VK_THREAD_IDLE_KHR" << std::endl; return result;
	case VK_THREAD_DONE_KHR: std::cout << "VKW Assertion error: VK_THREAD_DONE_KHR" << std::endl; return result;
	case VK_OPERATION_DEFERRED_KHR: std::cout << "VKW Assertion error: VK_OPERATION_DEFERRED_KHR" << std::endl; return result;
	case VK_OPERATION_NOT_DEFERRED_KHR: std::cout << "VKW Assertion error: VK_OPERATION_NOT_DEFERRED_KHR" << std::endl; return result;
	case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: std::cout << "VKW Assertion error: VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR" << std::endl; return result;
	case VK_ERROR_COMPRESSION_EXHAUSTED_EXT: std::cout << "VKW Assertion error: VK_ERROR_COMPRESSION_EXHAUSTED_EXT" << std::endl; return result;
	case VK_INCOMPATIBLE_SHADER_BINARY_EXT: std::cout << "VKW Assertion error: VK_INCOMPATIBLE_SHADER_BINARY_EXT" << std::endl; return result;
	case VK_PIPELINE_BINARY_MISSING_KHR: std::cout << "VKW Assertion error: VK_PIPELINE_BINARY_MISSING_KHR" << std::endl; return result;
	case VK_ERROR_NOT_ENOUGH_SPACE_KHR: std::cout << "VKW Assertion error: VK_ERROR_NOT_ENOUGH_SPACE_KHR" << std::endl; return result;
	case VK_RESULT_MAX_ENUM: std::cout << "VKW Assertion error: VK_RESULT_MAX_ENUM" << std::endl; return result;
	}
}

#pragma region VkWrapCore private method definitions

VkBool32 VKWNS::VkWrapCore::_debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
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

bool VKWNS::VkWrapCore::_checkInstanceLayersSupport() {
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

void VKWNS::VkWrapCore::_getAvailableSurfaceData() {
	if (!(mCoreCreateFlags & VKWRAP_CORE_CHECK_SWAPCHAIN_EXTENSION_SUPPORT))
		return;

	assert(mSurface && "No surface provided, add to your code .setSurface(<surface_handle_name>);");

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mSelectedPhysicalDevice, mSurface, &mSurfaceCapabilities);

	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(mSelectedPhysicalDevice, mSurface, &formatCount, nullptr);

	mAvailableSurfaceFormats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(mSelectedPhysicalDevice, mSurface, &formatCount, &mAvailableSurfaceFormats[0]);

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(mSelectedPhysicalDevice, mSurface, &presentModeCount, nullptr);

	mAvailableSurfacePresentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(mSelectedPhysicalDevice, mSurface, &presentModeCount, &mAvailableSurfacePresentModes[0]);
}

VkFormat VKWNS::VkWrapCore::_findSupportedFormat(const std::vector<VkFormat>& desiredFormats, const VkImageTiling tiling, const VkFormatFeatureFlags features) {
	for (VkFormat format : desiredFormats) {
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(mSelectedPhysicalDevice, format, &properties);

		if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	return VK_FORMAT_D32_SFLOAT_S8_UINT;
}

#pragma endregion

#pragma region VkWrapCore public method definitions
VKWNS::VkWrapCore::VkWrapCore() {
	applicationInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
	instanceInfo = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
	debugUtilsMessengerInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
	deviceInfo = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};

	applicationInfo.apiVersion = VK_API_VERSION_1_0;
	applicationInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	applicationInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	applicationInfo.pApplicationName = "DefualtApplicationName";
	applicationInfo.pEngineName = "DefaultEngineName";

	instanceInfo.pApplicationInfo = &applicationInfo;

	debugUtilsMessengerInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	debugUtilsMessengerInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	debugUtilsMessengerInfo.pfnUserCallback = _debugUtilsMessengerCallback;
	debugUtilsMessengerInfo.pUserData = nullptr;

	mSurfaceCapabilities = {};

	deviceEnabledFeatures = {};
	deviceEnabledFeatures.samplerAnisotropy = VK_TRUE;

	deviceInfo.pEnabledFeatures = &deviceEnabledFeatures;

	mDeviceQueueInfos = {};

	mCoreCreateFlags = 0;
	mGraphicsQueueIndex = -1;
	mTransferQueueIndex = -1;
	mComputeQueueIndex = -1;

	mSelectedSurfaceFormat = {};
	mSelectedSurfacePresentMode = {};

	mInstance = VK_NULL_HANDLE;
	mDebugUtilsMessenger = VK_NULL_HANDLE;
	mSurface = VK_NULL_HANDLE;
	mSelectedPhysicalDevice = VK_NULL_HANDLE;
	mGraphicsQueue = VK_NULL_HANDLE;
	mTransferQueue = VK_NULL_HANDLE;
	mComputeQueue = VK_NULL_HANDLE;
	mDevice = VK_NULL_HANDLE;
}

VKWNS::VkWrapCore::~VkWrapCore() {
	destroy();
}

// Getters

VkInstance& VKWNS::VkWrapCore::getInstance() { return mInstance; }

VkSurfaceKHR& VKWNS::VkWrapCore::getSurface() { return mSurface; }

VkSurfaceCapabilitiesKHR& VKWNS::VkWrapCore::getSurfaceCapabilities() { return mSurfaceCapabilities; }

VkSurfaceFormatKHR& VKWNS::VkWrapCore::getSurfaceFormat() { return mSelectedSurfaceFormat; }

VkPresentModeKHR& VKWNS::VkWrapCore::getPresentMode() { return mSelectedSurfacePresentMode; }

VkPhysicalDevice& VKWNS::VkWrapCore::getGPU() { return mSelectedPhysicalDevice; }

std::vector<VkPhysicalDevice>& VKWNS::VkWrapCore::getAvailableGPUs() { return mAvailablePhysicalDevices; }

uint32_t VKWNS::VkWrapCore::getGraphicsQueueIndex() { return mGraphicsQueueIndex; }

uint32_t VKWNS::VkWrapCore::getTransferQueueIndex() { return mTransferQueueIndex; }

uint32_t VKWNS::VkWrapCore::getComputeQueueIndex() { return mComputeQueueIndex; }

VkQueue& VKWNS::VkWrapCore::getGraphicsQueue() { return mGraphicsQueue; }

VkQueue& VKWNS::VkWrapCore::getTransferQueue() { return mTransferQueue; }

VkQueue& VKWNS::VkWrapCore::getComputeQueue() { return mComputeQueue; }

VkDevice& VKWNS::VkWrapCore::getDevice() { return mDevice; }

VkFormat VKWNS::VkWrapCore::getDepthFormat() { return _findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT); }

// Setters

VKWNS::VkWrapCore& VKWNS::VkWrapCore::setSurface(VkSurfaceKHR& rSurface) { 
	// We need to enable checks and get queue to use this extensions
	mCoreCreateFlags |= VKWRAP_CORE_GET_GRAPHICS_QUEUE | VKWRAP_CORE_CHECK_SWAPCHAIN_EXTENSION_SUPPORT;

	mSurface = rSurface; 
	
	return *this; 
}

VKWNS::VkWrapCore& VKWNS::VkWrapCore::setFlags(const VkWrapCoreCreateFlags flags) { mCoreCreateFlags = flags; return *this; }

VKWNS::VkWrapCore& VKWNS::VkWrapCore::setInstanceLayers(const std::vector<const char*>& rLayers) { mInstanceEnabledLayers = rLayers; return *this; }

VKWNS::VkWrapCore& VKWNS::VkWrapCore::setInstanceExtensions(const std::vector<const char*>& rExtensions) { mInstanceEnabledExtensions = rExtensions; return *this; }

VKWNS::VkWrapCore& VKWNS::VkWrapCore::setDeviceLayers(const std::vector<const char*>& rLayers) { mDeviceEnabledLayers = rLayers; return *this; }

VKWNS::VkWrapCore& VKWNS::VkWrapCore::setDeviceExtensions(const std::vector<const char*>& rExtensions) { mDeviceEnabledExtensions = rExtensions; return *this; }

// Autosetup functions

VKWNS::VkWrapCore& VKWNS::VkWrapCore::apiVersion(const uint32_t apiVersion) { applicationInfo.apiVersion = apiVersion; return *this; }

VKWNS::VkWrapCore& VKWNS::VkWrapCore::applicationName(const char* appName) { applicationInfo.pApplicationName = appName; return *this; }

VKWNS::VkWrapCore& VKWNS::VkWrapCore::engineName(const char* engineName) { applicationInfo.pEngineName = engineName; return *this; }

VKWNS::VkWrapCore& VKWNS::VkWrapCore::applicationVersion(const uint32_t appVersion) { applicationInfo.applicationVersion = appVersion; return *this; }

VKWNS::VkWrapCore& VKWNS::VkWrapCore::engineVersion(const uint32_t engineVersion) { applicationInfo.engineVersion = engineVersion; return *this; }

VKWNS::VkWrapCore& VKWNS::VkWrapCore::enableDebugMessenger(bool withInfo) {
	if (withInfo)
		debugUtilsMessengerInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	else 
		debugUtilsMessengerInfo.messageSeverity &= ~VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

	mCoreCreateFlags |= VKWRAP_CORE_CREATE_DEBUG_MESSENGER; 
	mInstanceEnabledLayers.push_back("VK_LAYER_KHRONOS_validation"); 

	return *this;
}

VKWNS::VkWrapCore& VKWNS::VkWrapCore::graphicsRequiredExtensions(const char* systemDependantVulkanExtension) {
	// We need to enable checks and get queue to use this extensions
	mCoreCreateFlags |= VKWRAP_CORE_GET_GRAPHICS_QUEUE | VKWRAP_CORE_CHECK_SWAPCHAIN_EXTENSION_SUPPORT;

	mInstanceEnabledExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	// This extension is specified in main app and is OS dependant
	mInstanceEnabledExtensions.push_back(systemDependantVulkanExtension);

	mDeviceEnabledExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	return *this;
}

VKWNS::VkWrapCore& VKWNS::VkWrapCore::selectSurfaceFormat(const VkSurfaceFormatKHR desiredFormat) {
	mSelectedSurfaceFormat = mAvailableSurfaceFormats[0];

	for (const VkSurfaceFormatKHR format : mAvailableSurfaceFormats) {
		if (format.colorSpace == desiredFormat.colorSpace && format.format == desiredFormat.format) {
			mSelectedSurfaceFormat = format;

			break;
		}
	}

	return *this;
}

VKWNS::VkWrapCore& VKWNS::VkWrapCore::selectPresentMode(const VkPresentModeKHR desiredPresentMode) {
	mSelectedSurfacePresentMode = mAvailableSurfacePresentModes[0];

	for (const VkPresentModeKHR mode : mAvailableSurfacePresentModes) {
		if (mode == desiredPresentMode) {
			mSelectedSurfacePresentMode = mode;

			break;
		}
	}

	return *this;
}

VKWNS::VkWrapCore& VKWNS::VkWrapCore::refreshSurfaceCapabilities() {
	_getAvailableSurfaceData();

	return *this;
}

// create/pick functions

VKWNS::VkWrapCore& VKWNS::VkWrapCore::createInstance() {
	// Check if debug messenger is supported
	if (mCoreCreateFlags & VKWRAP_CORE_CREATE_DEBUG_MESSENGER && !_checkInstanceLayersSupport())
		std::cout << "Some vulkan instance layers are not supported, check added layer names!";

	// Check if we want debug messenger
	if (mCoreCreateFlags & VKWRAP_CORE_CREATE_DEBUG_MESSENGER && _checkInstanceLayersSupport())
		instanceInfo.pNext = &debugUtilsMessengerInfo;

	if (mCoreCreateFlags & VKWRAP_CORE_CREATE_DEBUG_MESSENGER)
		mInstanceEnabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	// Set all extension and layers
	instanceInfo.enabledExtensionCount = mInstanceEnabledExtensions.empty() ? 0 : static_cast<uint32_t>(mInstanceEnabledExtensions.size());
	instanceInfo.enabledLayerCount = mInstanceEnabledLayers.empty() ? 0 : static_cast<uint32_t>(mInstanceEnabledLayers.size());
	instanceInfo.ppEnabledExtensionNames = mInstanceEnabledExtensions.empty() ? nullptr : &mInstanceEnabledExtensions[0];
	instanceInfo.ppEnabledLayerNames = mInstanceEnabledLayers.empty() ? nullptr : &mInstanceEnabledLayers[0];

	// Create instanec and check for errors
	assert(vkwAssert(vkCreateInstance(&instanceInfo, nullptr, &mInstance)) == VK_SUCCESS);
	
	// If we dont want debug messenger then return
	if (!(mCoreCreateFlags & VKWRAP_CORE_CREATE_DEBUG_MESSENGER))
		return *this;

	// If we want debug messenger then we need to load proper function to create it
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT");

	// Check if function if loaded and if debug messenger is created correctly
	if (func)
		assert(vkwAssert(func(mInstance, &debugUtilsMessengerInfo, nullptr, &mDebugUtilsMessenger)) == VK_SUCCESS);

	return *this;
}

VKWNS::VkWrapCore& VKWNS::VkWrapCore::pickGPU() {
	// Get all available physical devices
	uint32_t count = 0;
	vkEnumeratePhysicalDevices(mInstance, &count, nullptr);

	assert(count && "There is none GPU that support Vulkan API, update drivers or in worst case consider buying new graphics card (lowest spec cards supporting Vulkan 1.0 are Radeon HD 7000/Geforce 700 series)");

	mAvailablePhysicalDevices.resize(count);
	vkEnumeratePhysicalDevices(mInstance, &count, &mAvailablePhysicalDevices[0]);

	// Make score system for available physical devices (map orders value by key (lo to hi), where key is score (more=better))
	std::map<uint32_t, VkPhysicalDevice> scoredPhysicalDevices;

	// Iterate through all physical devices
	for (const VkPhysicalDevice physicalDevice : mAvailablePhysicalDevices) {
		// Current device score
		uint32_t currentScore = 0;

		// Available extensions of physical device
		uint32_t extensionPropertiesCount = 0;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionPropertiesCount, nullptr);

		std::vector<VkExtensionProperties> extensionProperties(extensionPropertiesCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionPropertiesCount, &extensionProperties[0]);

		if (mCoreCreateFlags & VKWRAP_CORE_CHECK_SWAPCHAIN_EXTENSION_SUPPORT) {
			// If we want swapchain support, check if we have surface
			assert(mSurface && "Provide handle to surface before trying to select physical device!");

			bool supportSwapchain = false;

			// Check if swapchain is supported by physical device
			for (VkExtensionProperties& extensionProperty : extensionProperties) {
				if (strcmp(extensionProperty.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
					supportSwapchain = true;

					break;
				}
			}

			// If swapchain is not supported just skip current device
			if (!supportSwapchain)
				continue;

			// Get available surface formats and present modes
			uint32_t formatsCount = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &formatsCount, nullptr);

			std::vector<VkSurfaceFormatKHR> supportedFormats(formatsCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &formatsCount, &supportedFormats[0]);

			uint32_t modesCount = 0;
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mSurface, &modesCount, nullptr);

			std::vector<VkPresentModeKHR> supportedModes(modesCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mSurface, &modesCount, &supportedModes[0]);

			// Check if we have at least 1 format and present mode, if not skip current device
			if (supportedFormats.empty() || supportedModes.empty())
				continue;
		}

		VkPhysicalDeviceFeatures currentFeatures;
		VkPhysicalDeviceProperties currentProperties;

		// get current physical device properties and features (such as support for geometryShaders or device type)
		vkGetPhysicalDeviceFeatures(physicalDevice, &currentFeatures);
		vkGetPhysicalDeviceProperties(physicalDevice, &currentProperties);

		if (currentProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) currentScore += 10000;

		if (currentFeatures.geometryShader) currentScore += 100;

		currentScore += currentProperties.limits.maxImageArrayLayers;

		// Get all queue family properties
		uint32_t availableQueuesCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &availableQueuesCount, nullptr);

		std::vector<VkQueueFamilyProperties> currentQueueProperties(availableQueuesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &availableQueuesCount, &currentQueueProperties[0]);

		uint32_t index = 0;

		// Search through queue families to find desired queues
		for (VkQueueFamilyProperties& queueProperties : currentQueueProperties) {
			if (mCoreCreateFlags & VKWRAP_CORE_GET_GRAPHICS_QUEUE && queueProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				currentScore += 1000;

			if (mCoreCreateFlags & VKWRAP_CORE_GET_COMPUTE_QUEUE && queueProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
				currentScore += 1000;

			if (mCoreCreateFlags & VKWRAP_CORE_GET_TRANSFER_QUEUE && queueProperties.queueFlags & VK_QUEUE_TRANSFER_BIT)
				currentScore += 1000;

			VkBool32 surfaceSupported = VK_FALSE;

			// Check for additional support of swapchain if needed
			if (mCoreCreateFlags & VKWRAP_CORE_CHECK_SWAPCHAIN_EXTENSION_SUPPORT && mSurface)
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, mSurface, &surfaceSupported);

			if (surfaceSupported)
				currentScore += 1000;

			index++;
		}

		scoredPhysicalDevices.insert(std::make_pair(currentScore, physicalDevice));
	}

	// Best is last
	mSelectedPhysicalDevice = scoredPhysicalDevices.rbegin()->second;

	// If there is no selected device try to select worst one just to show something
	// TODO: Possibly throw some warning later for selecting worst gpu in system
	if (!mSelectedPhysicalDevice)
		mSelectedPhysicalDevice = scoredPhysicalDevices.begin()->second;

	// If there still is no gpu selected, program can`t continue
	assert(mSelectedPhysicalDevice && "Couldn`t pick suitable GPU, consider writting your own impmenentation of pickGPU() function to meet your needs");

	// Get our surface data (such as surface capabilities, available formats and present modes)
	_getAvailableSurfaceData();

	// get selected gpu queue family properties
	uint32_t selectedGPUAvailableQueueFamiliesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(mSelectedPhysicalDevice, &selectedGPUAvailableQueueFamiliesCount, nullptr);

	std::vector<VkQueueFamilyProperties> selectedGPUAvailableQueueFamilyProperties(selectedGPUAvailableQueueFamiliesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(mSelectedPhysicalDevice, &selectedGPUAvailableQueueFamiliesCount, &selectedGPUAvailableQueueFamilyProperties[0]);

	int32_t selectedQueueIndex = 0;

	// Set desired flags to fetch queue indices from
	VkWrapCoreCreateFlags foundAllQueueIndicesFlags = mCoreCreateFlags & (
		VKWRAP_CORE_CHECK_SWAPCHAIN_EXTENSION_SUPPORT
		| VKWRAP_CORE_GET_COMPUTE_QUEUE
		| VKWRAP_CORE_GET_GRAPHICS_QUEUE
		| VKWRAP_CORE_GET_TRANSFER_QUEUE);

	for (VkQueueFamilyProperties& queueFamilyProperty : selectedGPUAvailableQueueFamilyProperties) {
		// For graphics, transfer and compute queues just copy whatever current index is and remove appropriate flag from foundAllQueueIndicesFlags
		if (foundAllQueueIndicesFlags & VKWRAP_CORE_GET_GRAPHICS_QUEUE && queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			mGraphicsQueueIndex = selectedQueueIndex;

			foundAllQueueIndicesFlags &= ~VKWRAP_CORE_GET_GRAPHICS_QUEUE;
		}

		if (foundAllQueueIndicesFlags & VKWRAP_CORE_GET_TRANSFER_QUEUE && queueFamilyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			mTransferQueueIndex = selectedQueueIndex;

			foundAllQueueIndicesFlags &= ~VKWRAP_CORE_GET_TRANSFER_QUEUE;
		}

		if (foundAllQueueIndicesFlags & VKWRAP_CORE_GET_COMPUTE_QUEUE && queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			mComputeQueueIndex = selectedQueueIndex;

			foundAllQueueIndicesFlags &= ~VKWRAP_CORE_GET_COMPUTE_QUEUE;
		}

		VkBool32 surfaceSupported = VK_FALSE;

		// For swapchain extension we need to first check if we want swapchain and have surface
		if (foundAllQueueIndicesFlags & VKWRAP_CORE_CHECK_SWAPCHAIN_EXTENSION_SUPPORT && mSurface)
			vkGetPhysicalDeviceSurfaceSupportKHR(mSelectedPhysicalDevice, selectedQueueIndex, mSurface, &surfaceSupported);

		// If we found queue that support surface and swapchain, and at the same time support graphics (which should by default), then 
		// override graphics queue index and remove swapchain and graphics flags from foundAllQueueIndicesFlags
		if (surfaceSupported && queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			mGraphicsQueueIndex = selectedQueueIndex;

			foundAllQueueIndicesFlags &= ~(VKWRAP_CORE_CHECK_SWAPCHAIN_EXTENSION_SUPPORT | VK_QUEUE_GRAPHICS_BIT);
		}

		// If foundAllFlags is equal to 0 then we have found all queue indices
		if (!foundAllQueueIndicesFlags)
			break;

		selectedQueueIndex++;
	}

	// Quick check of foundAllQueueIndicesFlags leftovers
	// TODO: Better handling of this leftover queue indices flags check
	assert(!foundAllQueueIndicesFlags && "Desired queue indices (graphics, transfer and/or compute) couldn`t be found in selected GPU, consider not using them or modify this code for your needs");

	if (mCoreCreateFlags & VKWRAP_CORE_CHECK_SWAPCHAIN_EXTENSION_SUPPORT) {
		selectSurfaceFormat({ VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR });
		selectPresentMode(VK_PRESENT_MODE_FIFO_KHR);
	}

	return *this;
}

VKWNS::VkWrapCore& VKWNS::VkWrapCore::createDevice() {
	// TODO: GOOD ENOUGH FOR NOW: Before creation of device throw better error if gpu was not selected
	assert(mSelectedPhysicalDevice && "GPU selection failed, if you see this, then it is more than likely possible that you forgot to add .pickGPU() in your initialization code, consider adding it");

	for (uint32_t i = 0; i < mDeviceQueueInfos.size(); i++) {
		mDeviceQueueInfos[i] = VkDeviceQueueCreateInfo{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0, (~0U), 1, &mDeviceQueuePriority };
	}

	// Count and setup queues
	if (mCoreCreateFlags & VKWRAP_CORE_GET_GRAPHICS_QUEUE)
		mDeviceQueueInfos[mDeviceQueuesAmount++].queueFamilyIndex = mGraphicsQueueIndex;

	if (mCoreCreateFlags & VKWRAP_CORE_GET_TRANSFER_QUEUE)
		mDeviceQueueInfos[mDeviceQueuesAmount++].queueFamilyIndex = mTransferQueueIndex;

	if (mCoreCreateFlags & VKWRAP_CORE_GET_COMPUTE_QUEUE)
		mDeviceQueueInfos[mDeviceQueuesAmount++].queueFamilyIndex = mComputeQueueIndex;

	// Setup gathered info
	deviceInfo.enabledExtensionCount = mDeviceEnabledExtensions.empty() ? 0 : static_cast<uint32_t>(mDeviceEnabledExtensions.size());
	deviceInfo.enabledLayerCount = mDeviceEnabledLayers.empty() ? 0 : static_cast<uint32_t>(mDeviceEnabledLayers.size());
	deviceInfo.ppEnabledExtensionNames = mDeviceEnabledExtensions.empty() ? nullptr : &mDeviceEnabledExtensions[0];
	deviceInfo.ppEnabledLayerNames = mDeviceEnabledLayers.empty() ? nullptr : &mDeviceEnabledLayers[0];

	deviceInfo.pQueueCreateInfos = &mDeviceQueueInfos[0];
	deviceInfo.queueCreateInfoCount = mDeviceQueuesAmount;

	// Create device
	assert(vkwAssert(vkCreateDevice(mSelectedPhysicalDevice, &deviceInfo, nullptr, &mDevice)) == VK_SUCCESS);

	// Get queues
	if (mCoreCreateFlags & VKWRAP_CORE_GET_GRAPHICS_QUEUE)
		vkGetDeviceQueue(mDevice, mGraphicsQueueIndex, 0, &mGraphicsQueue);

	if (mCoreCreateFlags & VKWRAP_CORE_GET_TRANSFER_QUEUE)
		vkGetDeviceQueue(mDevice, mTransferQueueIndex, 0, &mTransferQueue);

	if (mCoreCreateFlags & VKWRAP_CORE_GET_COMPUTE_QUEUE)
		vkGetDeviceQueue(mDevice, mComputeQueueIndex, 0, &mComputeQueue);

	return *this;
}

VKWNS::VkWrapCore& VKWNS::VkWrapCore::waitForDeviceIdle() {
	// Causes error at the end
	//assert(mDevice != VK_NULL_HANDLE);

	if(mDevice)
		vkDeviceWaitIdle(mDevice);

	return *this;
}

void VKWNS::VkWrapCore::destroy() {
	if(mDevice)
		vkDestroyDevice(mDevice, nullptr);

	if (mSurface)
		vkDestroySurfaceKHR(mInstance, mSurface, nullptr);

	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT");

	if (func && mDebugUtilsMessenger)
		func(mInstance, mDebugUtilsMessenger, nullptr);

	if (mInstance)
		vkDestroyInstance(mInstance, nullptr);

	mDevice = VK_NULL_HANDLE;
	mSurface = VK_NULL_HANDLE;
	mDebugUtilsMessenger = VK_NULL_HANDLE;
	mInstance = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region Swapchain method definitions

VKWNS::Swapchain::Swapchain() {
	mCorePointer = nullptr;
	mSwapchain = VK_NULL_HANDLE;
	swapchainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	mSwapchainExtent = { 0, 0 };
}

VKWNS::Swapchain::~Swapchain() {
	destroy();

	mCorePointer = nullptr;
}

VkSwapchainKHR& VKWNS::Swapchain::getSwapchain() {
	return mSwapchain;
}

VKWNS::VkWrapCore* VKWNS::Swapchain::getCorePtr() {
	assert(mCorePointer != nullptr);

	return mCorePointer;
}

VkExtent2D VKWNS::Swapchain::getExtent() {
	return mSwapchainExtent;
}

uint32_t VKWNS::Swapchain::getImagesAmount() {
	return swapchainImages.size();
}

VKWNS::Swapchain& VKWNS::Swapchain::setCorePtr(VkWrapCore *const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::Swapchain& VKWNS::Swapchain::create(const VkExtent2D swapchainExtent) {
	// Check if we have core pointer set
	assert(mCorePointer != nullptr);

	mSwapchainExtent = swapchainExtent;

	// Set correct value for minimal images amount
	swapchainInfo.minImageCount = mCorePointer->getSurfaceCapabilities().minImageCount + 1 < mCorePointer->getSurfaceCapabilities().maxImageCount
		&& mCorePointer->getSurfaceCapabilities().minImageCount > 0 
		&& mCorePointer->getSurfaceCapabilities().maxImageCount > 0
		?
		mCorePointer->getSurfaceCapabilities().minImageCount
		:
		mCorePointer->getSurfaceCapabilities().maxImageCount;

	// Set rest of data used to get images from swapchain
	swapchainInfo.imageFormat = mCorePointer->getSurfaceFormat().format;
	swapchainInfo.imageColorSpace = mCorePointer->getSurfaceFormat().colorSpace;
	swapchainInfo.imageExtent = mSwapchainExtent;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.surface = mCorePointer->getSurface();
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainInfo.preTransform = mCorePointer->getSurfaceCapabilities().currentTransform;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.presentMode = mCorePointer->getPresentMode();
	swapchainInfo.clipped = VK_TRUE;

	assert(vkwAssert(vkCreateSwapchainKHR(mCorePointer->getDevice(), &swapchainInfo, nullptr, &mSwapchain)) == VK_SUCCESS);
	
	uint32_t imageCount = 0;

	vkGetSwapchainImagesKHR(mCorePointer->getDevice(), mSwapchain, &imageCount, nullptr);

	swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(mCorePointer->getDevice(), mSwapchain, &imageCount, &swapchainImages[0]);

	return *this;
}

void VKWNS::Swapchain::destroy() {
	assert(mCorePointer != nullptr);

	swapchainImages.clear();

	if (mSwapchain)
		vkDestroySwapchainKHR(mCorePointer->getDevice(), mSwapchain, nullptr);

	mSwapchain = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region ImageView method definitions

VKWNS::ImageView::ImageView() {
	// Initialize baseline data
	imageViewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
	imageViewInfo.image = VK_NULL_HANDLE;
	imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewInfo.format = VK_FORMAT_UNDEFINED;
	imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
	imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
	imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
	imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
	imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewInfo.subresourceRange.baseArrayLayer = 0;
	imageViewInfo.subresourceRange.baseMipLevel = 0;
	imageViewInfo.subresourceRange.layerCount = 1;
	imageViewInfo.subresourceRange.levelCount = 1;

	mCorePointer = nullptr;

	mImageView = VK_NULL_HANDLE;
}

VKWNS::ImageView::~ImageView() {
	destroy();

	mCorePointer = nullptr;
}

VkImageView& VKWNS::ImageView::getImageView() {
	return mImageView;
}

VKWNS::VkWrapCore* VKWNS::ImageView::getCorePtr() {
	assert(mCorePointer != nullptr);

	return mCorePointer;
}

VKWNS::ImageView& VKWNS::ImageView::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::ImageView& VKWNS::ImageView::viewType(const VkImageViewType viewType) {
	imageViewInfo.viewType = viewType;

	return *this;
}

VKWNS::ImageView& VKWNS::ImageView::format(const VkFormat format) {
	imageViewInfo.format = format;

	return *this;
}

VKWNS::ImageView& VKWNS::ImageView::subresource(const VkImageAspectFlags aspectMask, const uint32_t baseArrayLayer, const uint32_t baseMipLevel, const uint32_t layerCount, const uint32_t levelCount) {
	imageViewInfo.subresourceRange.aspectMask = aspectMask;
	imageViewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
	imageViewInfo.subresourceRange.baseMipLevel = baseMipLevel;
	imageViewInfo.subresourceRange.layerCount = layerCount;
	imageViewInfo.subresourceRange.levelCount = levelCount;

	return *this;
}

VKWNS::ImageView& VKWNS::ImageView::create(const VkImage image) {
	assert(mCorePointer != nullptr);

	imageViewInfo.image = image;

	// If there is no format defined set default choosen one
	if (imageViewInfo.format == VK_FORMAT_UNDEFINED)
		imageViewInfo.format = mCorePointer->getSurfaceFormat().format;

	assert(vkwAssert(vkCreateImageView(mCorePointer->getDevice(), &imageViewInfo, nullptr, &mImageView)) == VK_SUCCESS);

	return *this;
}

void VKWNS::ImageView::destroy() {
	if (mImageView)
		vkDestroyImageView(mCorePointer->getDevice(), mImageView, nullptr);

	mImageView = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region Framebuffer method definitions

VKWNS::Framebuffer::Framebuffer() {
	framebufferInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
	framebufferInfo.attachmentCount = 0;
	framebufferInfo.pAttachments = nullptr;
	framebufferInfo.renderPass = VK_NULL_HANDLE;
	framebufferInfo.layers = 1;
	framebufferInfo.width = 0;
	framebufferInfo.height = 0;

	mFramebuffer = VK_NULL_HANDLE;
	framebufferAttachments.clear();
	mCorePointer = nullptr;
}

VKWNS::Framebuffer::~Framebuffer() {
	destroy();

	mCorePointer = nullptr;
}

VkFramebuffer& VKWNS::Framebuffer::getFramebuffer() {
	return mFramebuffer;
}

VKWNS::VkWrapCore* VKWNS::Framebuffer::getCorePtr() {
	return mCorePointer;
}

VKWNS::Framebuffer& VKWNS::Framebuffer::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::Framebuffer& VKWNS::Framebuffer::extent(const VkExtent2D extent) {
	framebufferInfo.width = extent.width;
	framebufferInfo.height = extent.height;

	return *this;
}

VKWNS::Framebuffer& VKWNS::Framebuffer::renderPass(const VkRenderPass renderPass) {
	framebufferInfo.renderPass = renderPass;

	return *this;
}

VKWNS::Framebuffer& VKWNS::Framebuffer::addAttachment(const VkImageView attachment) {
	framebufferAttachments.push_back(attachment);

	framebufferInfo.attachmentCount = static_cast<uint32_t>(framebufferAttachments.size());
	framebufferInfo.pAttachments = &framebufferAttachments[0];

	return *this;
}

VKWNS::Framebuffer& VKWNS::Framebuffer::clearAttachments() {
	framebufferAttachments.clear();

	framebufferInfo.attachmentCount = 0;
	framebufferInfo.pAttachments = nullptr;

	return *this;
}

VKWNS::Framebuffer& VKWNS::Framebuffer::layers(const uint32_t layers) {
	framebufferInfo.layers = layers;

	return *this;
}

VKWNS::Framebuffer& VKWNS::Framebuffer::create() {
	assert(mCorePointer != nullptr);

	assert(vkwAssert(vkCreateFramebuffer(mCorePointer->getDevice(), &framebufferInfo, nullptr, &mFramebuffer)) == VK_SUCCESS);

	return *this;
}

void VKWNS::Framebuffer::destroy() {
	if (mFramebuffer)
		vkDestroyFramebuffer(mCorePointer->getDevice(), mFramebuffer, nullptr);

	mFramebuffer = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region CommandPool method definitions

VKWNS::CommandPool::CommandPool() {
	commandPoolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };

	mCommandPool = VK_NULL_HANDLE;
	mCorePointer = nullptr;
}

VKWNS::CommandPool::~CommandPool() {
	destroy();

	mCorePointer = nullptr;
}

VkCommandPool& VKWNS::CommandPool::getCommandPool() {
	return mCommandPool;
}

VKWNS::VkWrapCore* VKWNS::CommandPool::getCorePtr() {
	return mCorePointer;
}

VKWNS::CommandPool& VKWNS::CommandPool::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::CommandPool& VKWNS::CommandPool::resetCommandBuffersFlag() {
	commandPoolInfo.flags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	return *this;
}

VKWNS::CommandPool& VKWNS::CommandPool::transientFlag() {
	commandPoolInfo.flags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

	return *this;
}

VKWNS::CommandPool& VKWNS::CommandPool::protectedFlag() {
	commandPoolInfo.flags |= VK_COMMAND_POOL_CREATE_PROTECTED_BIT;

	return *this;
}

VKWNS::CommandPool& VKWNS::CommandPool::clearFlags() {
	commandPoolInfo.flags = 0;

	return *this;
}

VKWNS::CommandPool& VKWNS::CommandPool::create(const uint32_t queueIndex) {
	assert(mCorePointer != nullptr);

	commandPoolInfo.queueFamilyIndex = queueIndex;

	assert(vkwAssert(vkCreateCommandPool(mCorePointer->getDevice(), &commandPoolInfo, nullptr, &mCommandPool)) == VK_SUCCESS);

	return *this;
}

void VKWNS::CommandPool::destroy() {
	if (mCommandPool)
		vkDestroyCommandPool(mCorePointer->getDevice(), mCommandPool, nullptr);

	mCommandPool = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region Buffer method definitions

uint32_t VKWNS::Buffer::_findMemoryType() {
	assert(mCorePointer != nullptr);

	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(mCorePointer->getGPU(), &memoryProperties);

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(mCorePointer->getDevice(), mBuffer, &memoryRequirements);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if (memoryRequirements.memoryTypeBits & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & mMemoryProperty) == mMemoryProperty)
			return i;
	}

	assert(false && "Cannot find suitable memory type");

	return ~0U;
}

VKWNS::Buffer::Buffer() {
	bufferAllocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
	bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.size = 0;
	bufferInfo.usage = 0;

	mBuffer = VK_NULL_HANDLE;
	mBufferMemory = VK_NULL_HANDLE;

	mCorePointer = nullptr;

	mMemoryProperty = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
}

VKWNS::Buffer::~Buffer() {
	destroy();

	mCorePointer = nullptr;
}

VkBuffer& VKWNS::Buffer::getBuffer() {
	return mBuffer;
}

VkDeviceMemory& VKWNS::Buffer::getMemory() {
	return mBufferMemory;
}

VKWNS::VkWrapCore* VKWNS::Buffer::getCorePtr() {
	return mCorePointer;
}

VKWNS::Buffer& VKWNS::Buffer::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::Buffer& VKWNS::Buffer::usage(const VkBufferUsageFlags usage) {
	bufferInfo.usage = usage;

	return *this;
}

VKWNS::Buffer& VKWNS::Buffer::properties(const VkMemoryPropertyFlags property) {
	mMemoryProperty = property;

	return *this;
}

VKWNS::Buffer& VKWNS::Buffer::map(void** ppData, const VkDeviceSize size, const VkDeviceSize offset) {
	vkMapMemory(mCorePointer->getDevice(), mBufferMemory, offset, size, 0, ppData);

	return *this;
}

VKWNS::Buffer& VKWNS::Buffer::unmap() {
	vkUnmapMemory(mCorePointer->getDevice(), mBufferMemory);

	return *this;
}

VKWNS::Buffer& VKWNS::Buffer::create(const uint32_t size) {
	assert(mCorePointer != nullptr);
	assert(bufferInfo.usage != 0);
	
	bufferInfo.size = size;

	assert(vkwAssert(vkCreateBuffer(mCorePointer->getDevice(), &bufferInfo, nullptr, &mBuffer)) == VK_SUCCESS);

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(mCorePointer->getDevice(), mBuffer, &memoryRequirements);

	bufferAllocInfo.memoryTypeIndex = _findMemoryType();
	bufferAllocInfo.allocationSize = memoryRequirements.size;

	assert(vkwAssert(vkAllocateMemory(mCorePointer->getDevice(), &bufferAllocInfo, nullptr, &mBufferMemory)) == VK_SUCCESS);

	vkBindBufferMemory(mCorePointer->getDevice(), mBuffer, mBufferMemory, 0);

	return *this;
}

void VKWNS::Buffer::destroy() {
	if (mBuffer)
		vkDestroyBuffer(mCorePointer->getDevice(), mBuffer, nullptr);

	if (mBufferMemory)
		vkFreeMemory(mCorePointer->getDevice(), mBufferMemory, nullptr);

	mBuffer = VK_NULL_HANDLE;
	mBufferMemory = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region ShaderModule method definitions

VKWNS::ShaderModule::ShaderModule() {
	shaderModuleInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
	
	mShaderModule = VK_NULL_HANDLE;
	mCorePointer = nullptr;
}

VKWNS::ShaderModule::~ShaderModule() {
	destroy();

	mCorePointer = nullptr;
}

VkShaderModule& VKWNS::ShaderModule::getShaderModule() {
	return mShaderModule;
}

VKWNS::VkWrapCore* VKWNS::ShaderModule::getCorePtr() {
	return mCorePointer;
}

VKWNS::ShaderModule& VKWNS::ShaderModule::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::ShaderModule& VKWNS::ShaderModule::create(const void* const srcData, const uint32_t size) {
	assert(mCorePointer != nullptr);

	shaderModuleInfo.codeSize = size;
	shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(srcData);

	assert(vkwAssert(vkCreateShaderModule(mCorePointer->getDevice(), &shaderModuleInfo, nullptr, &mShaderModule)) == VK_SUCCESS);

	return *this;
}

void VKWNS::ShaderModule::destroy() {
	if (mShaderModule)
		vkDestroyShaderModule(mCorePointer->getDevice(), mShaderModule, nullptr);

	mShaderModule = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region Pipeline method definitions

VKWNS::Pipeline::Pipeline() {
	vertexInputState = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
	inputAssemblyState = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, nullptr, 0, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE };
	tessellationState = { VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO };
	viewportState = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
	rasterizationState = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, nullptr, 0, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f };
	multisampleState = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, nullptr, 0, VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 0.0f, nullptr, VK_FALSE, VK_FALSE };
	depthStencilState = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
	colorBlendState = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, nullptr, 0, VK_FALSE, VK_LOGIC_OP_COPY, 0, nullptr, {0.0f, 0.0f, 0.0f, 0.0f} };
	dynamicState = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, nullptr, 0, 0, nullptr };

	graphicsPipelineInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, nullptr, 0, 0, nullptr, &vertexInputState, &inputAssemblyState, &tessellationState, &viewportState, &rasterizationState, &multisampleState, &depthStencilState, &colorBlendState, &dynamicState, VK_NULL_HANDLE, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, -1 };
	computePipelineInfo = { VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO, nullptr, 0, { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_COMPUTE_BIT, VK_NULL_HANDLE, "main", nullptr }, VK_NULL_HANDLE, VK_NULL_HANDLE, -1 };

	mPipeline = VK_NULL_HANDLE;
	mCorePointer = nullptr;

	mViewport = {};
	mScissor = {};
}

VKWNS::Pipeline::~Pipeline() {
	destroy();

	mCorePointer = nullptr;
}

VkPipeline& VKWNS::Pipeline::getPipeline() {
	return mPipeline;
}

VKWNS::VkWrapCore* VKWNS::Pipeline::getCorePtr() {
	return mCorePointer;
}

VKWNS::Pipeline& VKWNS::Pipeline::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::addShaderStage(const VkShaderModule module, const VkShaderStageFlagBits shaderStage, const char* const name) {
	shaderStages.emplace_back(VkPipelineShaderStageCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, shaderStage, module, name, nullptr });

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::clearShaderStages() {
	shaderStages.clear();

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::addDynamicState(const VkDynamicState state) {
	dynamicStates.push_back(state);

	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = &dynamicStates[0];

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::clearDynamicStates() {
	dynamicState.dynamicStateCount = 0;
	dynamicState.pDynamicStates = nullptr;
	
	dynamicStates.clear();

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::addAttribute(const uint32_t location, const uint32_t binding, const VkFormat format, const uint32_t offset) {
	vertexInputAttributes.emplace_back(VkVertexInputAttributeDescription{ location, binding, format, offset });

	vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
	vertexInputState.pVertexAttributeDescriptions = &vertexInputAttributes[0];

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::clearAttributes() {
	vertexInputState.vertexAttributeDescriptionCount = 0;
	vertexInputState.pVertexAttributeDescriptions = nullptr;

	vertexInputAttributes.clear();

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::addBinding(const uint32_t binding, const uint32_t stride, const VkVertexInputRate inputRate) {
	vertexInputBindings.emplace_back(VkVertexInputBindingDescription{ binding, stride, inputRate });

	vertexInputState.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindings.size());
	vertexInputState.pVertexBindingDescriptions = &vertexInputBindings[0];

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::clearBindings() {
	vertexInputState.vertexBindingDescriptionCount = 0;
	vertexInputState.pVertexBindingDescriptions = nullptr;

	vertexInputBindings.clear();

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::addColorBlendAttachment(const VkPipelineColorBlendAttachmentState colorBlendAttachment) {
	colorBlendAttachmentStates.push_back(colorBlendAttachment);

	colorBlendState.attachmentCount = static_cast<uint32_t>(colorBlendAttachmentStates.size());
	colorBlendState.pAttachments = &colorBlendAttachmentStates[0];

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::clearColorBlendAttachments() {
	colorBlendAttachmentStates.clear();

	colorBlendState.attachmentCount = 0;;
	colorBlendState.pAttachments = nullptr;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::topology(const VkPrimitiveTopology topology) {
	inputAssemblyState.topology = topology;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::viewport(const VkExtent2D size, const float x, const float y, const float minDepth, const float maxDepth) {
	mViewport.width = (float)((int)size.width);
	mViewport.height = (float)((int)size.height);
	mViewport.x = x;
	mViewport.y = y;
	mViewport.minDepth = minDepth;
	mViewport.maxDepth = maxDepth;

	viewportState.pViewports = &mViewport;
	viewportState.viewportCount = 1;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::scissor(const VkExtent2D size, const VkOffset2D offset) {
	mScissor.extent = size;
	mScissor.offset = offset;

	viewportState.pScissors = &mScissor;
	viewportState.scissorCount = 1;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::polygonMode(const VkPolygonMode polygonMode) {
	rasterizationState.polygonMode = polygonMode;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::cull(const VkCullModeFlags cullMode) {
	rasterizationState.cullMode = cullMode;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::frontFace(const VkFrontFace frontFace) {
	rasterizationState.frontFace = frontFace;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::renderPass(const VkRenderPass renderPass) {
	graphicsPipelineInfo.renderPass = renderPass;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::depthStencilEnable(const VkBool32 testEnable, const VkBool32 writeEnable, const VkBool32 boundTestEnable, const VkBool32 stencilTestEnable) {
	depthStencilState.depthTestEnable = testEnable;
	depthStencilState.depthWriteEnable = writeEnable;
	depthStencilState.depthBoundsTestEnable = boundTestEnable;
	depthStencilState.stencilTestEnable = stencilTestEnable;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::depthBounds(const float minDepth, const float maxDepth) {
	depthStencilState.minDepthBounds = minDepth;
	depthStencilState.maxDepthBounds = maxDepth;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::depthCompareOp(const VkCompareOp op) {
	depthStencilState.depthCompareOp = op;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::layout(const VkPipelineLayout layout) {
	graphicsPipelineInfo.layout = layout;
	computePipelineInfo.layout = layout;
	computePipelineInfo.layout = layout;

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::createGraphics() {
	assert(mCorePointer != nullptr);

	graphicsPipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
	graphicsPipelineInfo.pStages = &shaderStages[0];
	graphicsPipelineInfo.pVertexInputState = &vertexInputState;
	graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyState;
	graphicsPipelineInfo.pTessellationState = &tessellationState;
	graphicsPipelineInfo.pViewportState = &viewportState;
	graphicsPipelineInfo.pRasterizationState = &rasterizationState;
	graphicsPipelineInfo.pMultisampleState = &multisampleState;
	graphicsPipelineInfo.pDepthStencilState = &depthStencilState;
	graphicsPipelineInfo.pColorBlendState = &colorBlendState;
	graphicsPipelineInfo.pDynamicState = &dynamicState;

	assert(vkwAssert(vkCreateGraphicsPipelines(mCorePointer->getDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &mPipeline)) == VK_SUCCESS);

	return *this;
}

VKWNS::Pipeline& VKWNS::Pipeline::createCompute() {
	assert(mCorePointer != nullptr);

	computePipelineInfo.stage = shaderStages[0];

	assert(vkwAssert(vkCreateComputePipelines(mCorePointer->getDevice(), VK_NULL_HANDLE, 1, &computePipelineInfo, nullptr, &mPipeline)) == VK_SUCCESS);

	return *this;
}

void VKWNS::Pipeline::destroy() {
	if (mPipeline)
		vkDestroyPipeline(mCorePointer->getDevice(), mPipeline, nullptr);

	mPipeline = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region CommandBuffer method definitions

VKWNS::CommandBuffer::CommandBuffer() {
	commandBufferAllocateInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, nullptr, VK_NULL_HANDLE, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1 };
	commandBufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };

	mCommandBuffer = VK_NULL_HANDLE;
	mCorePointer = nullptr;
}

VKWNS::CommandBuffer::~CommandBuffer() {
	destroy();

	mCorePointer = nullptr;
}

VkCommandBuffer& VKWNS::CommandBuffer::getCommandBuffer() {
	return mCommandBuffer;
}

VKWNS::VkWrapCore* VKWNS::CommandBuffer::getCorePtr() {
	return mCorePointer;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::begin() {
	assert(vkwAssert(vkBeginCommandBuffer(mCommandBuffer, &commandBufferBeginInfo)) == VK_SUCCESS);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::end() {
	assert(vkwAssert(vkEndCommandBuffer(mCommandBuffer)) == VK_SUCCESS);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::reset(bool releaseResources) {
	assert(vkwAssert(vkResetCommandBuffer(mCommandBuffer, releaseResources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0)) == VK_SUCCESS);

	return *this;
}

// VULKAN 1.0 COMMANDS

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
	vkCmdBindPipeline(mCommandBuffer, pipelineBindPoint, pipeline);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) {
	vkCmdSetViewport(mCommandBuffer, firstViewport, viewportCount, pViewports);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) {
	vkCmdSetScissor(mCommandBuffer, firstScissor, scissorCount, pScissors);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetLineWidth(float lineWidth) {
	vkCmdSetLineWidth(mCommandBuffer, lineWidth);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) {
	vkCmdSetDepthBias(mCommandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetBlendConstants(const float blendConstants[4]) {
	vkCmdSetBlendConstants(mCommandBuffer, blendConstants);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetDepthBounds(float minDepthBounds, float maxDepthBounds) {
	vkCmdSetDepthBounds(mCommandBuffer, minDepthBounds, maxDepthBounds);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetStencilCompareMask(VkStencilFaceFlags faceMask, uint32_t compareMask) {
	vkCmdSetStencilCompareMask(mCommandBuffer, faceMask, compareMask);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetStencilWriteMask(VkStencilFaceFlags faceMask, uint32_t writeMask) {
	vkCmdSetStencilWriteMask(mCommandBuffer, faceMask, writeMask);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetStencilReference(VkStencilFaceFlags faceMask, uint32_t reference) {
	vkCmdSetStencilReference(mCommandBuffer, faceMask, reference);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) {
	vkCmdBindDescriptorSets(mCommandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
	vkCmdBindIndexBuffer(mCommandBuffer, buffer, offset, indexType);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) {
	vkCmdBindVertexBuffers(mCommandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
	vkCmdDraw(mCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
	vkCmdDrawIndexed(mCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdDrawIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
	vkCmdDrawIndirect(mCommandBuffer, buffer, offset, drawCount, stride);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdDrawIndexedIndirect(VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
	vkCmdDrawIndexedIndirect(mCommandBuffer, buffer, offset, drawCount, stride);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdDispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
	vkCmdDispatch(mCommandBuffer, groupCountX, groupCountY, groupCountZ);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdDispatchIndirect(VkBuffer buffer, VkDeviceSize offset) {
	vkCmdDispatchIndirect(mCommandBuffer, buffer, offset);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) {
	vkCmdCopyBuffer(mCommandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdCopyImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) {
	vkCmdCopyImage(mCommandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBlitImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) {
	vkCmdBlitImage(mCommandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdCopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
	vkCmdCopyBufferToImage(mCommandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdCopyImageToBuffer(VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
	vkCmdCopyImageToBuffer(mCommandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdUpdateBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) {
	vkCmdUpdateBuffer(mCommandBuffer, dstBuffer, dstOffset, dataSize, pData);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdFillBuffer(VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
	vkCmdFillBuffer(mCommandBuffer, dstBuffer, dstOffset, size, data);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdClearColorImage(VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
	vkCmdClearColorImage(mCommandBuffer, image, imageLayout, pColor, rangeCount, pRanges);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdClearDepthStencilImage(VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
	vkCmdClearDepthStencilImage(mCommandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdClearAttachments(uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) {
	vkCmdClearAttachments(mCommandBuffer, attachmentCount, pAttachments, rectCount, pRects);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdResolveImage(VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) {
	vkCmdResolveImage(mCommandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetEvent(VkEvent event, VkPipelineStageFlags stageMask) {
	vkCmdSetEvent(mCommandBuffer, event, stageMask);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdResetEvent(VkEvent event, VkPipelineStageFlags stageMask) {
	vkCmdResetEvent(mCommandBuffer, event, stageMask);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdWaitEvents(uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
	vkCmdWaitEvents(mCommandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
	vkCmdPipelineBarrier(mCommandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBeginQuery(VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) {
	vkCmdBeginQuery(mCommandBuffer, queryPool, query, flags);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdEndQuery(VkQueryPool queryPool, uint32_t query) {
	vkCmdEndQuery(mCommandBuffer, queryPool, query);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdResetQueryPool(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
	vkCmdResetQueryPool(mCommandBuffer, queryPool, firstQuery, queryCount);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdWriteTimestamp(VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
	vkCmdWriteTimestamp(mCommandBuffer, pipelineStage, queryPool, query);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdCopyQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) {
	vkCmdCopyQueryPoolResults(mCommandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdPushConstants(VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) {
	vkCmdPushConstants(mCommandBuffer, layout, stageFlags, offset, size, pValues);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBeginRenderPass(const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
	vkCmdBeginRenderPass(mCommandBuffer, pRenderPassBegin, contents);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdNextSubpass(VkSubpassContents contents) {
	vkCmdNextSubpass(mCommandBuffer, contents);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdEndRenderPass() {
	vkCmdEndRenderPass(mCommandBuffer);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdExecuteCommands(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
	vkCmdExecuteCommands(mCommandBuffer, commandBufferCount, pCommandBuffers);

	return *this;
}

// VULKAN 1.1 COMMANDS

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetDeviceMask(uint32_t deviceMask) {
	vkCmdSetDeviceMask(mCommandBuffer, deviceMask);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdDispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
	vkCmdDispatchBase(mCommandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);

	return *this;
}

// VULKAN 1.2 COMMANDS

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdDrawIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) { 
	vkCmdDrawIndirectCount(mCommandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride); 

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdDrawIndexedIndirectCount(VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) { 
	vkCmdDrawIndexedIndirectCount(mCommandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride); 

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBeginRenderPass2(const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfo* pSubpassBeginInfo) { 
	vkCmdBeginRenderPass2(mCommandBuffer, pRenderPassBegin, pSubpassBeginInfo); 

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdNextSubpass2(const VkSubpassBeginInfo* pSubpassBeginInfo, const VkSubpassEndInfo* pSubpassEndInfo) { 
	vkCmdNextSubpass2(mCommandBuffer, pSubpassBeginInfo, pSubpassEndInfo); 

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdEndRenderPass2(const VkSubpassEndInfo* pSubpassEndInfo) { 
	vkCmdEndRenderPass2(mCommandBuffer, pSubpassEndInfo); 

	return *this;
}


// VULKAN 1.3 COMMANDS

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetEvent2(VkEvent event, const VkDependencyInfo* pDependencyInfo) {
	vkCmdSetEvent2(mCommandBuffer, event, pDependencyInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdResetEvent2(VkEvent event, VkPipelineStageFlags2 stageMask) {
	vkCmdResetEvent2(mCommandBuffer, event, stageMask);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdWaitEvents2(uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfo* pDependencyInfos) {
	vkCmdWaitEvents2(mCommandBuffer, eventCount, pEvents, pDependencyInfos);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdPipelineBarrier2(const VkDependencyInfo* pDependencyInfo) {
	vkCmdPipelineBarrier2(mCommandBuffer, pDependencyInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdWriteTimestamp2(VkPipelineStageFlags2 stage, VkQueryPool queryPool, uint32_t query) {
	vkCmdWriteTimestamp2(mCommandBuffer, stage, queryPool, query);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdCopyBuffer2(const VkCopyBufferInfo2* pCopyBufferInfo) {
	vkCmdCopyBuffer2(mCommandBuffer, pCopyBufferInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdCopyImage2(const VkCopyImageInfo2* pCopyImageInfo) {
	vkCmdCopyImage2(mCommandBuffer, pCopyImageInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdCopyBufferToImage2(const VkCopyBufferToImageInfo2* pCopyBufferToImageInfo) {
	vkCmdCopyBufferToImage2(mCommandBuffer, pCopyBufferToImageInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdCopyImageToBuffer2(const VkCopyImageToBufferInfo2* pCopyImageToBufferInfo) {
	vkCmdCopyImageToBuffer2(mCommandBuffer, pCopyImageToBufferInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBlitImage2(const VkBlitImageInfo2* pBlitImageInfo) {
	vkCmdBlitImage2(mCommandBuffer, pBlitImageInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdResolveImage2(const VkResolveImageInfo2* pResolveImageInfo) {
	vkCmdResolveImage2(mCommandBuffer, pResolveImageInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBeginRendering(const VkRenderingInfo* pRenderingInfo) {
	vkCmdBeginRendering(mCommandBuffer, pRenderingInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdEndRendering() {
	vkCmdEndRendering(mCommandBuffer);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetCullMode(VkCullModeFlags cullMode) {
	vkCmdSetCullMode(mCommandBuffer, cullMode);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetFrontFace(VkFrontFace frontFace) {
	vkCmdSetFrontFace(mCommandBuffer, frontFace);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetPrimitiveTopology(VkPrimitiveTopology primitiveTopology) {
	vkCmdSetPrimitiveTopology(mCommandBuffer, primitiveTopology);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetViewportWithCount(uint32_t viewportCount, const VkViewport* pViewports) {
	vkCmdSetViewportWithCount(mCommandBuffer, viewportCount, pViewports);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetScissorWithCount(uint32_t scissorCount, const VkRect2D* pScissors) {
	vkCmdSetScissorWithCount(mCommandBuffer, scissorCount, pScissors);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBindVertexBuffers2(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) {
	vkCmdBindVertexBuffers2(mCommandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes, pStrides);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetDepthTestEnable(VkBool32 depthTestEnable) {
	vkCmdSetDepthTestEnable(mCommandBuffer, depthTestEnable);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetDepthWriteEnable(VkBool32 depthWriteEnable) {
	vkCmdSetDepthWriteEnable(mCommandBuffer, depthWriteEnable);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetDepthCompareOp(VkCompareOp depthCompareOp) {
	vkCmdSetDepthCompareOp(mCommandBuffer, depthCompareOp);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetDepthBoundsTestEnable(VkBool32 depthBoundsTestEnable) {
	vkCmdSetDepthBoundsTestEnable(mCommandBuffer, depthBoundsTestEnable);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetStencilTestEnable(VkBool32 stencilTestEnable) {
	vkCmdSetStencilTestEnable(mCommandBuffer, stencilTestEnable);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetStencilOp(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) {
	vkCmdSetStencilOp(mCommandBuffer, faceMask, failOp, passOp, depthFailOp, compareOp);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetRasterizerDiscardEnable(VkBool32 rasterizerDiscardEnable) {
	vkCmdSetRasterizerDiscardEnable(mCommandBuffer, rasterizerDiscardEnable);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetDepthBiasEnable(VkBool32 depthBiasEnable) {
	vkCmdSetDepthBiasEnable(mCommandBuffer, depthBiasEnable);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetPrimitiveRestartEnable(VkBool32 primitiveRestartEnable) {
	vkCmdSetPrimitiveRestartEnable(mCommandBuffer, primitiveRestartEnable);

	return *this;
}


// VULKAN 1.4 COMMANDS

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetLineStipple(uint32_t lineStippleFactor, uint16_t lineStipplePattern) {
	vkCmdSetLineStipple(mCommandBuffer, lineStippleFactor, lineStipplePattern);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBindIndexBuffer2(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType) {
	vkCmdBindIndexBuffer2(mCommandBuffer, buffer, offset, size, indexType);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdPushDescriptorSet(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) {
	vkCmdPushDescriptorSet(mCommandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdPushDescriptorSetWithTemplate(VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) {
	vkCmdPushDescriptorSetWithTemplate(mCommandBuffer, descriptorUpdateTemplate, layout, set, pData);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetRenderingAttachmentLocations(const VkRenderingAttachmentLocationInfo* pLocationInfo) {
	vkCmdSetRenderingAttachmentLocations(mCommandBuffer, pLocationInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdSetRenderingInputAttachmentIndices(const VkRenderingInputAttachmentIndexInfo* pInputAttachmentIndexInfo) {
	vkCmdSetRenderingInputAttachmentIndices(mCommandBuffer, pInputAttachmentIndexInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdBindDescriptorSets2(const VkBindDescriptorSetsInfo* pBindDescriptorSetsInfo) {
	vkCmdBindDescriptorSets2(mCommandBuffer, pBindDescriptorSetsInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdPushConstants2(const VkPushConstantsInfo* pPushConstantsInfo) {
	vkCmdPushConstants2(mCommandBuffer, pPushConstantsInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdPushDescriptorSet2(const VkPushDescriptorSetInfo* pPushDescriptorSetInfo) {
	vkCmdPushDescriptorSet2(mCommandBuffer, pPushDescriptorSetInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::cmdPushDescriptorSetWithTemplate2(const VkPushDescriptorSetWithTemplateInfo* pPushDescriptorSetWithTemplateInfo) {
	vkCmdPushDescriptorSetWithTemplate2(mCommandBuffer, pPushDescriptorSetWithTemplateInfo);

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::primaryLevel() {
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::secondaryLevel() {
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::beginFlag(const VkCommandBufferUsageFlags beginUsageFlags) {
	commandBufferBeginInfo.flags = beginUsageFlags;

	return *this;
}

VKWNS::CommandBuffer& VKWNS::CommandBuffer::allocate(const VkCommandPool commandPool) {
	assert(mCorePointer != nullptr);

	assert(commandPool != VK_NULL_HANDLE);

	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.commandBufferCount = 1;

	assert(vkwAssert(vkAllocateCommandBuffers(mCorePointer->getDevice(), &commandBufferAllocateInfo, &mCommandBuffer)) == VK_SUCCESS);

	return *this;
}

void VKWNS::CommandBuffer::destroy() {
	if (mCommandBuffer)
		vkFreeCommandBuffers(mCorePointer->getDevice(), commandBufferAllocateInfo.commandPool, 1, &mCommandBuffer);

	mCommandBuffer = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region Semaphore method definitions

VKWNS::Semaphore::Semaphore() {
	semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
	
	mSemaphore = VK_NULL_HANDLE;
	mCorePointer = nullptr;
}

VKWNS::Semaphore::~Semaphore() {
	destroy();

	mCorePointer = nullptr;
}

VkSemaphore& VKWNS::Semaphore::getSemaphore() {
	return mSemaphore;
}

VKWNS::VkWrapCore* VKWNS::Semaphore::getCorePtr() {
	return mCorePointer;
}

VKWNS::Semaphore& VKWNS::Semaphore::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::Semaphore& VKWNS::Semaphore::create() {
	assert(mCorePointer != nullptr);

	assert(vkwAssert(vkCreateSemaphore(mCorePointer->getDevice(), &semaphoreInfo, nullptr, &mSemaphore)) == VK_SUCCESS);

	return *this;
}

void VKWNS::Semaphore::destroy() {
	if (mSemaphore)
		vkDestroySemaphore(mCorePointer->getDevice(), mSemaphore, nullptr);

	mSemaphore = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region Fence method definitions

VKWNS::Fence::Fence() {
	fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };

	mFence = VK_NULL_HANDLE;
	mCorePointer = nullptr;
}

VKWNS::Fence::~Fence() {
	destroy();

	mCorePointer = nullptr;
}

VkFence& VKWNS::Fence::getFence() {
	return mFence;
}

VKWNS::VkWrapCore* VKWNS::Fence::getCorePtr() {
	return mCorePointer;
}

VKWNS::Fence& VKWNS::Fence::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::Fence& VKWNS::Fence::signaled() {
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	return *this;
}

VKWNS::Fence& VKWNS::Fence::reset() {
	assert(mCorePointer != nullptr);

	vkResetFences(mCorePointer->getDevice(), 1, &mFence);

	return *this;
}

VKWNS::Fence& VKWNS::Fence::wait(const uint64_t timeout) {
	assert(mCorePointer != nullptr);

	vkWaitForFences(mCorePointer->getDevice(), 1, &mFence, VK_TRUE, timeout);

	return *this;
}

VKWNS::Fence& VKWNS::Fence::create() {
	assert(mCorePointer != nullptr);

	assert(vkwAssert(vkCreateFence(mCorePointer->getDevice(), &fenceInfo, nullptr, &mFence)) == VK_SUCCESS);

	return *this;
}

void VKWNS::Fence::destroy() {
	if (mFence)
		vkDestroyFence(mCorePointer->getDevice(), mFence, nullptr);

	mFence = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region DescriptorSetLayout method definitions

VKWNS::DescriptorSetLayout::DescriptorSetLayout() {
	mDescriptorSetLayout = VK_NULL_HANDLE;
	mCorePointer = nullptr;
}

VKWNS::DescriptorSetLayout::~DescriptorSetLayout() {
	destroy();

	mCorePointer = nullptr;
}

VkDescriptorSetLayout& VKWNS::DescriptorSetLayout::getSetLayout() {
	return mDescriptorSetLayout;
}

VKWNS::VkWrapCore* VKWNS::DescriptorSetLayout::getCorePtr() {
	return mCorePointer;
}

VKWNS::DescriptorSetLayout& VKWNS::DescriptorSetLayout::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::DescriptorSetLayout& VKWNS::DescriptorSetLayout::addBinding(const uint32_t binding, const uint32_t descriptorCount, const VkDescriptorType descriptorType, const VkShaderStageFlags stageFlags, const VkSampler* pImmutableSamplers) {
	descriptorSetLayoutBindings.emplace_back(VkDescriptorSetLayoutBinding{ binding, descriptorType, descriptorCount, stageFlags, pImmutableSamplers });

	return *this;
}

VKWNS::DescriptorSetLayout& VKWNS::DescriptorSetLayout::clearBindings() {
	descriptorSetLayoutBindings.clear();

	return *this;
}

VKWNS::DescriptorSetLayout& VKWNS::DescriptorSetLayout::create() {
	assert(mCorePointer != nullptr);

	VkDescriptorSetLayoutCreateInfo createInfo{
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		nullptr,
		0,
		static_cast<uint32_t>(descriptorSetLayoutBindings.size()),
		&descriptorSetLayoutBindings[0]
	};

	assert(vkwAssert(vkCreateDescriptorSetLayout(mCorePointer->getDevice(), &createInfo, nullptr, &mDescriptorSetLayout)) == VK_SUCCESS);

	return *this;
}

void VKWNS::DescriptorSetLayout::destroy() {
	if (mDescriptorSetLayout)
		vkDestroyDescriptorSetLayout(mCorePointer->getDevice(), mDescriptorSetLayout, nullptr);

	mDescriptorSetLayout = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region DescriptorPool method definitions

VKWNS::DescriptorPool::DescriptorPool() {
	descriptorPoolInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
	descriptorPoolInfo.flags = 0;
	descriptorPoolInfo.maxSets = 0;
	descriptorPoolInfo.poolSizeCount = 0;
	descriptorPoolInfo.pPoolSizes = nullptr;
	
	mDescriptorPool = VK_NULL_HANDLE;
	mDescriptorPoolSizes = {};
	mCorePointer = nullptr;
}

VKWNS::DescriptorPool::~DescriptorPool() {
	destroy();

	mCorePointer = nullptr;
}

VkDescriptorPool& VKWNS::DescriptorPool::getPool() {
	return mDescriptorPool;
}

VKWNS::VkWrapCore* VKWNS::DescriptorPool::getCorePtr() {
	return mCorePointer;
}

VKWNS::DescriptorPool& VKWNS::DescriptorPool::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::DescriptorPool& VKWNS::DescriptorPool::addSize(const VkDescriptorType type, const uint32_t descriptorCount) {
	mDescriptorPoolSizes.emplace_back(VkDescriptorPoolSize{ type, descriptorCount });

	descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(mDescriptorPoolSizes.size());
	descriptorPoolInfo.pPoolSizes = &mDescriptorPoolSizes[0];

	return *this;
}

VKWNS::DescriptorPool& VKWNS::DescriptorPool::clearSizes() {
	mDescriptorPoolSizes.clear();

	return *this;
}

VKWNS::DescriptorPool& VKWNS::DescriptorPool::maxSets(const uint32_t amount) {
	descriptorPoolInfo.maxSets = amount;

	return *this;
}

VKWNS::DescriptorPool& VKWNS::DescriptorPool::flags(const VkDescriptorPoolCreateFlags flags) {
	descriptorPoolInfo.flags = flags;

	return *this;
}

VKWNS::DescriptorPool& VKWNS::DescriptorPool::create() {
	assert(mCorePointer != nullptr);

	assert(vkwAssert(vkCreateDescriptorPool(mCorePointer->getDevice(), &descriptorPoolInfo, nullptr, &mDescriptorPool)) == VK_SUCCESS);

	return *this;
}

void VKWNS::DescriptorPool::destroy() {
	if (mDescriptorPool)
		vkDestroyDescriptorPool(mCorePointer->getDevice(), mDescriptorPool, nullptr);

	mDescriptorPool = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region WriteDescriptorSet method definitions

VKWNS::WriteDescriptorSet::WriteDescriptorSet() {
	descriptorWriteSet = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
	descriptorWriteSet.descriptorCount = 1;
	descriptorWriteSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWriteSet.dstArrayElement = 0;
	descriptorWriteSet.dstBinding = 0;
	descriptorWriteSet.dstSet = nullptr;
	descriptorWriteSet.pBufferInfo = nullptr;
	descriptorWriteSet.pImageInfo = nullptr;
	descriptorWriteSet.pTexelBufferView = nullptr;

	descriptorBufferInfo.buffer = VK_NULL_HANDLE;
	descriptorBufferInfo.offset = 0;
	descriptorBufferInfo.range = VK_WHOLE_SIZE;

	descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	descriptorImageInfo.imageView = VK_NULL_HANDLE;
	descriptorImageInfo.sampler = VK_NULL_HANDLE;
}

VKWNS::WriteDescriptorSet& VKWNS::WriteDescriptorSet::type(const VkDescriptorType descriptorType) {
	descriptorWriteSet.descriptorType = descriptorType;

	return *this;
}

VKWNS::WriteDescriptorSet& VKWNS::WriteDescriptorSet::binding(const uint32_t bindingPoint) {
	descriptorWriteSet.dstBinding = bindingPoint;

	return *this;
}

VKWNS::WriteDescriptorSet& VKWNS::WriteDescriptorSet::arrayElement(const uint32_t element) {
	descriptorWriteSet.dstArrayElement = element;

	return *this;
}

VKWNS::WriteDescriptorSet& VKWNS::WriteDescriptorSet::asUniformBuffer(const VkBuffer dataBuffer, const VkDeviceSize range, const VkDeviceSize offset) {
	descriptorBufferInfo.buffer = dataBuffer;
	descriptorBufferInfo.range = range;
	descriptorBufferInfo.offset = offset;

	descriptorWriteSet.pBufferInfo = &descriptorBufferInfo;
	descriptorWriteSet.pImageInfo = nullptr;
	descriptorWriteSet.pTexelBufferView = VK_NULL_HANDLE;

	return *this;
}

VKWNS::WriteDescriptorSet& VKWNS::WriteDescriptorSet::asSampler(const VkImageView imageView, const VkImageLayout imageLayout, const VkSampler sampler) {
	descriptorImageInfo.imageView = imageView;
	descriptorImageInfo.imageLayout = imageLayout;
	descriptorImageInfo.sampler = sampler;

	descriptorWriteSet.pBufferInfo = nullptr;
	descriptorWriteSet.pImageInfo = &descriptorImageInfo;
	descriptorWriteSet.pTexelBufferView = VK_NULL_HANDLE;

	return *this;
}

VKWNS::WriteDescriptorSet& VKWNS::WriteDescriptorSet::setDescriptorSet(const VkDescriptorSet set) {
	descriptorWriteSet.dstSet = set;

	return *this;
}

#pragma endregion

#pragma region DescriptorSet method definitions

VKWNS::DescriptorSet::DescriptorSet() {
	descriptorSetAllocInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
	descriptorSetAllocInfo.descriptorPool = VK_NULL_HANDLE;
	descriptorSetAllocInfo.descriptorSetCount = 1;
	descriptorSetAllocInfo.pSetLayouts = nullptr;
	
	mDescriptorSet = VK_NULL_HANDLE;
	mCorePointer = nullptr;
}

VKWNS::DescriptorSet::~DescriptorSet() {
	destroy();

	mCorePointer = nullptr;
}

VkDescriptorSet& VKWNS::DescriptorSet::getSet() {
	return mDescriptorSet;
}

VKWNS::VkWrapCore* VKWNS::DescriptorSet::getCorePtr() {
	return mCorePointer;
}

VKWNS::DescriptorSet& VKWNS::DescriptorSet::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::DescriptorSet& VKWNS::DescriptorSet::setLayouts(VkDescriptorSetLayout* const pLayouts) {
	descriptorSetAllocInfo.pSetLayouts = pLayouts;

	return *this;
}

VKWNS::DescriptorSet& VKWNS::DescriptorSet::allocate(const VkDescriptorPool pool) {
	assert(mCorePointer != nullptr);

	assert(pool != VK_NULL_HANDLE);

	descriptorSetAllocInfo.descriptorPool = pool;

	assert(vkwAssert(vkAllocateDescriptorSets(mCorePointer->getDevice(), &descriptorSetAllocInfo, &mDescriptorSet)) == VK_SUCCESS);

	return *this;
}

VKWNS::DescriptorSet& VKWNS::DescriptorSet::update(const std::vector<VkWriteDescriptorSet>& writeDescriptorSets) {
	assert(mCorePointer != nullptr);

	vkUpdateDescriptorSets(mCorePointer->getDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), &writeDescriptorSets[0], 0, nullptr);

	return *this;
}

VKWNS::DescriptorSet& VKWNS::DescriptorSet::update(const std::vector<WriteDescriptorSet>& writeDescriptorSets) {
	std::vector<VkWriteDescriptorSet> writeSets;

	for (WriteDescriptorSet wds : writeDescriptorSets) {
		writeSets.push_back(wds.descriptorWriteSet);
	}
	
	assert(mCorePointer != nullptr);

	vkUpdateDescriptorSets(mCorePointer->getDevice(), static_cast<uint32_t>(writeSets.size()), &writeSets[0], 0, nullptr);

	return *this;
}

VKWNS::DescriptorSet& VKWNS::DescriptorSet::update(VkWriteDescriptorSet& writeDescriptorSets) {
	assert(mCorePointer != nullptr);

	vkUpdateDescriptorSets(mCorePointer->getDevice(), 1, &writeDescriptorSets, 0, nullptr);

	return *this;
}

void VKWNS::DescriptorSet::destroy() {
	if (mDescriptorSet)
		vkFreeDescriptorSets(mCorePointer->getDevice(), descriptorSetAllocInfo.descriptorPool, descriptorSetAllocInfo.descriptorSetCount, &mDescriptorSet);

	mDescriptorSet = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region PipelineLayout method definitions

VKWNS::PipelineLayout::PipelineLayout() {
	pipelineLayoutInfo = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;

	mDescriptorLayouts = {};
	mPushConstantRanges = {};

	mPipelineLayout = VK_NULL_HANDLE;
	mCorePointer = nullptr;
}

VKWNS::PipelineLayout::~PipelineLayout() {
	destroy();

	mCorePointer = nullptr;
}

VkPipelineLayout& VKWNS::PipelineLayout::getLayout() {
	return mPipelineLayout;
}

VKWNS::VkWrapCore* VKWNS::PipelineLayout::getCorePtr() {
	return mCorePointer;
}

VKWNS::PipelineLayout& VKWNS::PipelineLayout::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::PipelineLayout& VKWNS::PipelineLayout::addLayouts(const VkDescriptorSetLayout setLayouts) {
	mDescriptorLayouts.push_back(setLayouts);

	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(mDescriptorLayouts.size());
	pipelineLayoutInfo.pSetLayouts = &mDescriptorLayouts[0];

	return *this;
}

VKWNS::PipelineLayout& VKWNS::PipelineLayout::addConstantRanges(const VkPushConstantRange pushConstantRanges) {
	mPushConstantRanges.push_back(pushConstantRanges);

	pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(mPushConstantRanges.size());
	pipelineLayoutInfo.pPushConstantRanges = &mPushConstantRanges[0];

	return *this;
}

VKWNS::PipelineLayout& VKWNS::PipelineLayout::clearLayouts() {
	mDescriptorLayouts.clear();

	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;

	return *this;
}

VKWNS::PipelineLayout& VKWNS::PipelineLayout::clearConstantRanges() {
	mPushConstantRanges.clear();
	
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	return *this;
}

VKWNS::PipelineLayout& VKWNS::PipelineLayout::create() {
	assert(mCorePointer != nullptr);

	assert(vkwAssert(vkCreatePipelineLayout(mCorePointer->getDevice(), &pipelineLayoutInfo, nullptr, &mPipelineLayout)) == VK_SUCCESS);

	return *this;
}

void VKWNS::PipelineLayout::destroy() {
	if (mPipelineLayout)
		vkDestroyPipelineLayout(mCorePointer->getDevice(), mPipelineLayout, nullptr);

	mPipelineLayout = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region Submit method definitions

VKWNS::Submit::Submit() {
	submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
	submitInfo.commandBufferCount = 0;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pCommandBuffers = nullptr;
	submitInfo.pSignalSemaphores = nullptr;
	submitInfo.pWaitDstStageMask = nullptr;
	submitInfo.pWaitSemaphores = nullptr;
	submitInfo.pNext = nullptr;
}

VKWNS::Submit::~Submit() {
	submitInfo.commandBufferCount = 0;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pCommandBuffers = nullptr;
	submitInfo.pSignalSemaphores = nullptr;
	submitInfo.pWaitDstStageMask = nullptr;
	submitInfo.pWaitSemaphores = nullptr;
	submitInfo.pNext = nullptr;

	mWaitSemaphores.clear();
	mSignalSemaphores.clear();
	mWaitDstStageMask.clear();
	mCommandBuffers.clear();
}

VKWNS::Submit& VKWNS::Submit::addWait(const VkSemaphore waitSemaphore) {
	mWaitSemaphores.push_back(waitSemaphore);

	submitInfo.waitSemaphoreCount = static_cast<uint32_t>(mWaitSemaphores.size());
	submitInfo.pWaitSemaphores = &mWaitSemaphores[0];

	return *this;
}

VKWNS::Submit& VKWNS::Submit::addSignal(const VkSemaphore signalSemaphore) {
	mSignalSemaphores.push_back(signalSemaphore);

	submitInfo.signalSemaphoreCount = static_cast<uint32_t>(mSignalSemaphores.size());
	submitInfo.pSignalSemaphores = &mSignalSemaphores[0];

	return *this;
}

VKWNS::Submit& VKWNS::Submit::addWaitDstMask(const VkPipelineStageFlags waitDstStageMask) {
	mWaitDstStageMask.push_back(waitDstStageMask);

	submitInfo.pWaitDstStageMask = &mWaitDstStageMask[0];

	return *this;
}

VKWNS::Submit& VKWNS::Submit::addCommandBuffer(const VkCommandBuffer commandBuffer) {
	mCommandBuffers.push_back(commandBuffer);

	submitInfo.commandBufferCount = static_cast<uint32_t>(mCommandBuffers.size());
	submitInfo.pCommandBuffers = &mCommandBuffers[0];

	return *this;
}

VkResult VKWNS::Submit::submit(const VkQueue queue, const VkFence fence) {
	return vkQueueSubmit(queue, 1, &submitInfo, fence);
}

#pragma endregion

#pragma region Present method definitions

VKWNS::Present::Present() {
	presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	presentInfo.swapchainCount = 0;
	presentInfo.waitSemaphoreCount = 0;
	presentInfo.pImageIndices = nullptr;
	presentInfo.pNext = nullptr;
	presentInfo.pResults = nullptr;
	presentInfo.pSwapchains = nullptr;
	presentInfo.pWaitSemaphores = nullptr;
}

VKWNS::Present::~Present() {
	presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	presentInfo.swapchainCount = 0;
	presentInfo.waitSemaphoreCount = 0;
	presentInfo.pImageIndices = nullptr;
	presentInfo.pNext = nullptr;
	presentInfo.pResults = nullptr;
	presentInfo.pSwapchains = nullptr;
	presentInfo.pWaitSemaphores = nullptr;

	mWaitSemaphores.clear();
	mSwapchains.clear();
}

VKWNS::Present& VKWNS::Present::addWait(const VkSemaphore waitSemaphore) {
	mWaitSemaphores.push_back(waitSemaphore);

	presentInfo.waitSemaphoreCount = static_cast<uint32_t>(mWaitSemaphores.size());
	presentInfo.pWaitSemaphores = &mWaitSemaphores[0];

	return *this;
}

VKWNS::Present& VKWNS::Present::addSwapchain(const VkSwapchainKHR swapchain) {
	mSwapchains.push_back(swapchain);

	presentInfo.swapchainCount = static_cast<uint32_t>(mSwapchains.size());
	presentInfo.pSwapchains = &mSwapchains[0];

	return *this;
}

VkResult VKWNS::Present::present(const VkQueue queue, const uint32_t* const pImageIndides) {
	presentInfo.pImageIndices = pImageIndides;

	return vkQueuePresentKHR(queue, &presentInfo);
}

#pragma endregion

#pragma region Image method definitions

uint32_t VKWNS::Image::_findMemoryType() {
	assert(mCorePointer != nullptr);

	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(mCorePointer->getGPU(), &memoryProperties);

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(mCorePointer->getDevice(), mImage, &memoryRequirements);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if (memoryRequirements.memoryTypeBits & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & mMemoryProperty) == mMemoryProperty)
			return i;
	}

	assert(false && "Cannot find suitable memory type");

	return ~0U;
}

VKWNS::Image::Image() {
	imageInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent = { 0, 0, 0 };
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0;

	imageAllocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };

	mMemoryProperty = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

	mImage = VK_NULL_HANDLE;
	mImageMemory = VK_NULL_HANDLE;
	mCorePointer = nullptr;
}

VKWNS::Image::~Image() {
	destroy();

	mCorePointer = nullptr;
}

VkImage& VKWNS::Image::getImage() {
	return mImage;
}

VkDeviceMemory& VKWNS::Image::getImageMemory() {
	return mImageMemory;
}

VKWNS::VkWrapCore* VKWNS::Image::getCorePtr() {
	return mCorePointer;
}

VKWNS::Image& VKWNS::Image::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::Image& VKWNS::Image::extent(const VkExtent3D extent) {
	imageInfo.extent = extent;

	return *this;
}

VKWNS::Image& VKWNS::Image::imageType(const VkImageType type) {
	imageInfo.imageType = type;

	return *this;
}

VKWNS::Image& VKWNS::Image::mipmapLevels(const uint32_t levels) {
	imageInfo.mipLevels = levels;

	return *this;
}

VKWNS::Image& VKWNS::Image::arrayLayers(const uint32_t layers) {
	imageInfo.arrayLayers = layers;

	return *this;
}

VKWNS::Image& VKWNS::Image::format(const VkFormat format) {
	imageInfo.format = format;

	return *this;
}

VKWNS::Image& VKWNS::Image::tiling(const VkImageTiling tiling) {
	imageInfo.tiling = tiling;

	return *this;
}

VKWNS::Image& VKWNS::Image::initialLayout(const VkImageLayout imageLayout) {
	imageInfo.initialLayout = imageLayout;

	return *this;
}

VKWNS::Image& VKWNS::Image::usage(const VkImageUsageFlags imageUsage) {
	imageInfo.usage = imageUsage;

	return *this;
}

VKWNS::Image& VKWNS::Image::sharingMode(const VkSharingMode sharingMode) {
	imageInfo.sharingMode = sharingMode;

	return *this;
}

VKWNS::Image& VKWNS::Image::samples(const VkSampleCountFlagBits samples) {
	imageInfo.samples = samples;

	return *this;
}

VKWNS::Image& VKWNS::Image::flags(const VkImageCreateFlags imageFlags) {
	imageInfo.flags = imageFlags;

	return *this;
}

VKWNS::Image& VKWNS::Image::properties(const VkMemoryPropertyFlags memoryProperties) {
	mMemoryProperty = memoryProperties;

	return *this;
}

VKWNS::Image& VKWNS::Image::create() {
	assert(mCorePointer != nullptr);

	assert(vkwAssert(vkCreateImage(mCorePointer->getDevice(), &imageInfo, nullptr, &mImage)) == VK_SUCCESS);

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(mCorePointer->getDevice(), mImage, &memoryRequirements);

	imageAllocInfo.allocationSize = memoryRequirements.size;
	imageAllocInfo.memoryTypeIndex = _findMemoryType();

	assert(vkwAssert(vkAllocateMemory(mCorePointer->getDevice(), &imageAllocInfo, nullptr, &mImageMemory)) == VK_SUCCESS);

	vkBindImageMemory(mCorePointer->getDevice(), mImage, mImageMemory, 0);

	return *this;
}

void VKWNS::Image::destroy() {
	if (mImage)
		vkDestroyImage(mCorePointer->getDevice(), mImage, nullptr);

	if (mImageMemory)
		vkFreeMemory(mCorePointer->getDevice(), mImageMemory, nullptr);

	mImage = VK_NULL_HANDLE;
	mImageMemory = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region Sampler method definitions

VKWNS::Sampler::Sampler() {
	samplerInfo = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 0;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	mSampler = VK_NULL_HANDLE;
	mCorePointer = nullptr;
}

VKWNS::Sampler::~Sampler() {
	destroy();

	mCorePointer = nullptr;
}

VkSampler& VKWNS::Sampler::getSampler() {
	return mSampler;
}

VKWNS::VkWrapCore* VKWNS::Sampler::getCorePtr() {
	return mCorePointer;
}

VKWNS::Sampler& VKWNS::Sampler::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::Sampler& VKWNS::Sampler::magFilter(const VkFilter filter) {
	samplerInfo.magFilter = filter;

	return *this;
}

VKWNS::Sampler& VKWNS::Sampler::minFilter(const VkFilter filter) {
	samplerInfo.minFilter = filter;

	return *this;
}

VKWNS::Sampler& VKWNS::Sampler::addressMode(const VkSamplerAddressMode u, const VkSamplerAddressMode v, const VkSamplerAddressMode w) {
	samplerInfo.addressModeU = u;
	samplerInfo.addressModeV = v;
	samplerInfo.addressModeW = w;

	return *this;
}

VKWNS::Sampler& VKWNS::Sampler::maxAnisotropy(const int32_t anisotropy) {
	assert(mCorePointer != nullptr);

	VkPhysicalDeviceProperties gpuProperties{};
	vkGetPhysicalDeviceProperties(mCorePointer->getGPU(), &gpuProperties);

	if (anisotropy == 0) {
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 0;

		return *this;
	}

	if (anisotropy < 0) {
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = gpuProperties.limits.maxSamplerAnisotropy;

		return *this;
	}

	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = anisotropy > gpuProperties.limits.maxSamplerAnisotropy ? gpuProperties.limits.maxSamplerAnisotropy : anisotropy;

	return *this;
}

VKWNS::Sampler& VKWNS::Sampler::borderColor(const VkBorderColor color) {
	samplerInfo.borderColor = color;

	return *this;
}

VKWNS::Sampler& VKWNS::Sampler::unnormalizedCoordinates() {
	samplerInfo.unnormalizedCoordinates = VK_TRUE;

	return *this;
}

VKWNS::Sampler& VKWNS::Sampler::normalizedCoordinates() {
	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	return *this;
}

VKWNS::Sampler& VKWNS::Sampler::compare(const VkBool32 enable, const VkCompareOp op) {
	samplerInfo.compareEnable = enable;
	samplerInfo.compareOp = op;

	return *this;
}

VKWNS::Sampler& VKWNS::Sampler::mipmap(const VkSamplerMipmapMode mode, const float lodBias, const float minLod, const float maxLod) {
	samplerInfo.mipmapMode = mode;
	samplerInfo.mipLodBias = lodBias;
	samplerInfo.minLod = minLod;
	samplerInfo.maxLod = maxLod;

	return *this;
}

VKWNS::Sampler& VKWNS::Sampler::create() {
	assert(mCorePointer != nullptr);

	assert(vkwAssert(vkCreateSampler(mCorePointer->getDevice(), &samplerInfo, nullptr, &mSampler)) == VK_SUCCESS);

	return *this;
}

void VKWNS::Sampler::destroy() {
	if (mSampler)
		vkDestroySampler(mCorePointer->getDevice(), mSampler, nullptr);

	mSampler = VK_NULL_HANDLE;
}

#pragma endregion

#pragma region RenderPass method definitions

VKWNS::RenderPass::RenderPass() {
	renderPassInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2 };

	mRenderPass = VK_NULL_HANDLE;
	mCorePointer = nullptr;
}

VKWNS::RenderPass::~RenderPass() {
	destroy();

	mCorePointer = nullptr;
}

VkRenderPass& VKWNS::RenderPass::getRenderPass() {
	return mRenderPass;
}

std::vector<VkAttachmentDescription2>& VKWNS::RenderPass::getAttachments() {
	return mAttachments;
}

std::vector<VKWNS::RenderPass::Subpass>& VKWNS::RenderPass::getSubpasses() {
	return mSubpasses;
}

uint32_t VKWNS::RenderPass::getAttachmentId(const std::string name) {
	return mAttachmentIndices[name];
}

VKWNS::VkWrapCore* VKWNS::RenderPass::getCorePtr() {
	return mCorePointer;
}

VKWNS::RenderPass& VKWNS::RenderPass::setCorePtr(VkWrapCore* const pCore) {
	mCorePointer = pCore;

	return *this;
}

VKWNS::RenderPass& VKWNS::RenderPass::addAttachment(const std::string name, const VkFormat format, const VkSampleCountFlagBits samples, const VkAttachmentLoadOp loadOp, const VkAttachmentStoreOp storeOp, const VkImageLayout finalLayout) {
	mAttachments.emplace_back(VkAttachmentDescription2{ 
		VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2, nullptr, 0, 
		format, 
		samples, 
		loadOp, storeOp, 
		VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, 
		VK_IMAGE_LAYOUT_UNDEFINED, finalLayout 
	});

	mAttachmentIndices[name] = mAttachments.size() - 1;

	return *this;
}

VKWNS::RenderPass& VKWNS::RenderPass::clearAttachments() {
	mAttachments.clear();

	return *this;
}

VKWNS::RenderPass& VKWNS::RenderPass::addSubpass(const bool withDepthReference) {
	mSubpasses.emplace_back(Subpass{
		// References: input, output, depth
		{},
		{},
		{},
		// Should be with depth?
		withDepthReference,
		// Subpass description
		{
			VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2,
			nullptr,
			0,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			0,
			// Input attachments
			0, nullptr,
			// Color attachments
			0, nullptr,
			// Resolve attachment
			nullptr,
			// Depth attachment
			nullptr,
			// Preserve attachment
			0, nullptr
		},
		// Subpass dependency
		{}
	});

	return *this;
}

VKWNS::RenderPass& VKWNS::RenderPass::clearSubpasses() {
	mSubpasses.clear();

	return *this;
}

VKWNS::RenderPass& VKWNS::RenderPass::addSubpassDependency(const uint32_t subpassId, const uint32_t srcSubpass, const uint32_t dstSubpass, const VkPipelineStageFlags srcStage, const VkPipelineStageFlags dstStage, const VkAccessFlags srcAccess, const VkAccessFlags dstAccess) {
	mSubpasses[subpassId].subpassDependencies.emplace_back(VkSubpassDependency2{
		VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2, nullptr,
		srcSubpass, dstSubpass,
		srcStage, dstStage,
		srcAccess, dstAccess
	});

	return *this;
}

VKWNS::RenderPass& VKWNS::RenderPass::clearSubpassDependencies(const uint32_t subpassId) {
	mSubpasses[subpassId].subpassDependencies.clear();

	return *this;
}

VKWNS::RenderPass& VKWNS::RenderPass::subpassAddInputRef(const uint32_t subpassId, const uint32_t attachment, const VkImageLayout layout, const VkImageAspectFlags aspectMask) {
	mSubpasses[subpassId].subpassInputAttachmentRefs.emplace_back(VkAttachmentReference2{ VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2, nullptr, attachment, layout, aspectMask });

	mSubpasses[subpassId].subpassDescription.inputAttachmentCount = static_cast<uint32_t>(mSubpasses[subpassId].subpassInputAttachmentRefs.size());
	mSubpasses[subpassId].subpassDescription.pInputAttachments = &mSubpasses[subpassId].subpassInputAttachmentRefs[0];

	return *this;
}

VKWNS::RenderPass& VKWNS::RenderPass::subpassAddOutputRef(const uint32_t subpassId, const uint32_t attachment, const VkImageLayout layout, const VkImageAspectFlags aspectMask) {
	mSubpasses[subpassId].subpassOutputAttachmentRefs.emplace_back(VkAttachmentReference2{ VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2, nullptr, attachment, layout, aspectMask });

	mSubpasses[subpassId].subpassDescription.colorAttachmentCount = static_cast<uint32_t>(mSubpasses[subpassId].subpassOutputAttachmentRefs.size());
	mSubpasses[subpassId].subpassDescription.pColorAttachments = &mSubpasses[subpassId].subpassOutputAttachmentRefs[0];

	return *this;
}

VKWNS::RenderPass& VKWNS::RenderPass::clearSubpassRefs(const uint32_t subpassId) {
	mSubpasses[subpassId].subpassDepthAttachmentRef = {};
	mSubpasses[subpassId].subpassInputAttachmentRefs.clear();
	mSubpasses[subpassId].subpassOutputAttachmentRefs.clear();

	mSubpasses[subpassId].subpassDescription.inputAttachmentCount = 0;
	mSubpasses[subpassId].subpassDescription.colorAttachmentCount = 0;
	mSubpasses[subpassId].subpassDescription.pInputAttachments = nullptr;
	mSubpasses[subpassId].subpassDescription.pColorAttachments = nullptr;
	mSubpasses[subpassId].subpassDescription.pDepthStencilAttachment = nullptr;

	return *this;
}

VKWNS::RenderPass& VKWNS::RenderPass::subpassDepthRef(const uint32_t subpassId, const uint32_t attachment, const VkImageLayout layout, const VkImageAspectFlags aspectMask) {
	mSubpasses[subpassId].subpassDepthAttachmentRef = VkAttachmentReference2{ VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2, nullptr, attachment, layout, aspectMask };

	if (mSubpasses[subpassId].depthRefExist)
		mSubpasses[subpassId].subpassDescription.pDepthStencilAttachment = &mSubpasses[subpassId].subpassDepthAttachmentRef;

	return *this;
}

VKWNS::RenderPass& VKWNS::RenderPass::create() {
	assert(mCorePointer != nullptr);

	renderPassInfo.attachmentCount = static_cast<uint32_t>(mAttachments.size());
	renderPassInfo.pAttachments = &mAttachments[0];

	std::vector<VkSubpassDependency2> dependencies;
	std::vector<VkSubpassDescription2> subpasses;

	for (Subpass& subpass : mSubpasses) {
		std::copy(subpass.subpassDependencies.begin(), subpass.subpassDependencies.end(), std::back_inserter(dependencies));
		subpasses.emplace_back(subpass.subpassDescription);
	}

	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = &dependencies[0];
	renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
	renderPassInfo.pSubpasses = &subpasses[0];

	assert(vkwAssert(vkCreateRenderPass2(mCorePointer->getDevice(), &renderPassInfo, nullptr, &mRenderPass)) == VK_SUCCESS);

	// Clean out of scope pointers
	renderPassInfo.pDependencies = nullptr;
	renderPassInfo.pSubpasses = nullptr;

	return *this;
}

void VKWNS::RenderPass::destroy() {
	if (mRenderPass != VK_NULL_HANDLE)
		vkDestroyRenderPass(mCorePointer->getDevice(), mRenderPass, nullptr);

	mRenderPass = VK_NULL_HANDLE;
}

#pragma endregion