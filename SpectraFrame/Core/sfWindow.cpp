#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "sfWindow.h"
#include "sfLogger.h"
#include "sfVkCommon.h"

sf::Window* sf::Window::sWindowInstancePtr = nullptr;

void sf::Window::_framebufferCallback(GLFWwindow* wnd, int width, int height) {
  // Resize window to new size
  ((Window*)glfwGetWindowUserPointer(wnd))->resize(width, height);
}

sf::Window::Window() {
  mWindow = nullptr;
  mWindowSurface = nullptr;
  mWindowSwapchain = nullptr;

  mWindowResized = false;
  mWidth = 0;
  mHeight = 0;

  if (sWindowInstancePtr) {
    SF_CLOG("ERR: Cannot create another sf::Window instance!");
    
    abort();
  }

  sWindowInstancePtr = this;
}

void sf::Window::_createSwapchain() {
  // Check if we recreate swapchain for a new one
  vk::SwapchainKHR oldSwapchain = mWindowSwapchain != nullptr ? mWindowSwapchain : nullptr;

  vk::Extent2D swapchainExtent = Vulkan::surfaceCapabilities().currentExtent;

  if(swapchainExtent.width == ~0U)
    swapchainExtent = vk::Extent2D(mWidth, mHeight);

  // Fill swapchain create infos struct
  vk::SwapchainCreateInfoKHR swapchainInfo{};
  swapchainInfo
    .setSurface(mWindowSurface)
    .setImageArrayLayers(1)
    .setImageColorSpace(Vulkan::surfaceFormat().colorSpace)
    .setImageFormat(Vulkan::surfaceFormat().format)
    .setImageExtent(swapchainExtent)
    .setImageSharingMode(vk::SharingMode::eExclusive)
    .setClipped(vk::True)
    .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
    .setQueueFamilyIndexCount(0)
    .setPQueueFamilyIndices(nullptr)
    .setPresentMode(Vulkan::presentMode())
    .setPreTransform(Vulkan::surfaceCapabilities().currentTransform)
    .setMinImageCount(Vulkan::surfaceCapabilities().minImageCount < 3 ? 3 : Vulkan::surfaceCapabilities().minImageCount)
    .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

  if(oldSwapchain != nullptr)
    swapchainInfo.setOldSwapchain(oldSwapchain);

  // Create new swapchain
  if(Vulkan::device().createSwapchainKHR(&swapchainInfo, nullptr, &mWindowSwapchain) != vk::Result::eSuccess) {
    SF_CLOG("ERR: Cannot create swapchain!");
  }

  // Destroy the old one
  if(oldSwapchain != nullptr)
    Vulkan::device().destroySwapchainKHR(oldSwapchain, nullptr);

  // Get images of a new swapchain
  mWindowSwapchainImages = Vulkan::device().getSwapchainImagesKHR(mWindowSwapchain);

  // Make space for image views and synchonization handles
  mWindowSwapchainImageViews.resize(mWindowSwapchainImages.size());
  mSwapchainFencesInFlight.resize(mWindowSwapchainImageViews.size());
  mSwapchainImageAvailableSemaphores.resize(mSwapchainFencesInFlight.size());
  mSwapchainRenderFinishedSemaphores.resize(mSwapchainFencesInFlight.size());

  // Create image views and sync objects per frame/image
  for(uint32_t i = 0; i < mWindowSwapchainImageViews.size(); i++) {
    vk::ImageViewCreateInfo imageViewInfo{};
    imageViewInfo
      .setImage(mWindowSwapchainImages[i])
      .setViewType(vk::ImageViewType::e2D)
      .setFormat(Vulkan::surfaceFormat().format)
      .setComponents(vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA))
      .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

    Vulkan::device().createImageView(&imageViewInfo, nullptr, &mWindowSwapchainImageViews[i]);

    createVkFence(mSwapchainFencesInFlight[i]);
    createVkSemaphore(mSwapchainImageAvailableSemaphores[i]);
    createVkSemaphore(mSwapchainRenderFinishedSemaphores[i]);
  }
}

void sf::Window::_destroySwapchainRelatedObjects() {
  // Destroy everything beside swapchain
  for(uint32_t i = 0; i < mWindowSwapchainImageViews.size(); i++) {
    Vulkan::device().destroyImageView(mWindowSwapchainImageViews[i], nullptr);
    Vulkan::device().destroyFence(mSwapchainFencesInFlight[i], nullptr);
    Vulkan::device().destroySemaphore(mSwapchainImageAvailableSemaphores[i], nullptr);
    Vulkan::device().destroySemaphore(mSwapchainRenderFinishedSemaphores[i], nullptr);
  }

  // Clear data
  mWindowSwapchainImageViews.clear();
  mSwapchainFencesInFlight.clear();
  mSwapchainImageAvailableSemaphores.clear();
  mSwapchainRenderFinishedSemaphores.clear();
  mWindowSwapchainImages.clear();
}

sf::Window::~Window() {
  destroy();
}

sf::Window& sf::Window::useVulkanDebugger(bool useDebugger) {
  mCreateVulkanDebugger = useDebugger;

  return *this;
}

sf::Window& sf::Window::create(const char* title, const int width, const int height) {
  // Init glfw context
  glfwInit();

  // Throw out any client api that would be used along with window
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  // Create window with desired name and size
  mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

  // Check if we have window
  if (!mWindow)
    SF_CLOG("ERR: Cannot create window!");

  // Pass window class as pointer to all glfw callbacks
  glfwSetWindowUserPointer(mWindow, this);

  mWidth = width;
  mHeight = height;

  // Kinda useless, except of setting application name to title, as we set this up by default
  Vulkan::getVk().applicationInfo
    // Support for dynamic rendering, CANNOT BE LOWER THAN 1.3 TO GUARANTEE EXISTANCE OF RENDER PASS 2 AND SYNCHRONIZATION 2
    .setApiVersion(VK_API_VERSION_1_3)
    .setPApplicationName(title)
    .setPEngineName("SpectraFrame")
    .setEngineVersion(VK_MAKE_API_VERSION(0, 0, 0, 1));

  // Set up required instance extensions
  Vulkan::getVk()
#ifdef VK_USE_PLATFORM_WIN32_KHR
    .graphicsRequiredExtensions(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    .graphicsRequiredExtensions(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#else
    .graphicsRequiredExtensions(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#endif

  // If needed create debug messenger
  if (mCreateVulkanDebugger)
    Vulkan::getVk().enableDebugMessenger();

  // Create instance
  Vulkan::getVk().createInstance();

  SF_CLOG("Created instance");

  // Create surface
  VkSurfaceKHR surface;
  glfwCreateWindowSurface(Vulkan::instance(), mWindow, nullptr, &surface);
  mWindowSurface = vk::SurfaceKHR(surface);

  SF_CLOG("Created surface");

  // Pass surface to Vulkan class, pick best gpu and create device
  Vulkan::getVk()
    .setSurface(mWindowSurface)
    .pickGPU()
    .createDevice();

  SF_CLOG("Initialized vulkan");

  // Then make swapchain
  _createSwapchain();

  // And call start virtual function
  start();

  return *this;
}

sf::Window& sf::Window::runLoop() {
  // Check if we still want to have opened window
  while (!glfwWindowShouldClose(mWindow)) {
    // Run through counter for synchronization objects
    mFrameSyncCounter = (mFrameSyncCounter + 1) % mSwapchainFencesInFlight.size();

    // Poll glfw for events
    glfwPollEvents();

    // Wait for fences and reset them to signaled stage
    Vulkan::device().waitForFences(1, &mSwapchainFencesInFlight[mFrameSyncCounter], vk::True, ~0ULL);
    Vulkan::device().resetFences(1, &mSwapchainFencesInFlight[mFrameSyncCounter]);

    // Acquire image index
    vk::AcquireNextImageInfoKHR acquireInfo{};
    acquireInfo
      .setSemaphore(mSwapchainImageAvailableSemaphores[mFrameSyncCounter])
      .setSwapchain(mWindowSwapchain)
      .setFence(nullptr)
      .setDeviceMask(1)
      .setTimeout(~0ULL);

    vk::Result result = Vulkan::device().acquireNextImage2KHR(&acquireInfo, &mSwapchainCurrentImageIndex);

    // If swapchain was out of date, rebuild it
    if(result == vk::Result::eErrorOutOfDateKHR) {
      Vulkan::waitForIdle();
      Vulkan::refreshCapabilities();

      _destroySwapchainRelatedObjects();

      _createSwapchain();

      continue;
    }
      
    // Call update on everything else that is image related (rendering)
    update();

    // Present image in screen
    vk::PresentInfoKHR present;
    present
      .setPImageIndices(&mSwapchainCurrentImageIndex)
      .setSwapchainCount(1)
      .setPSwapchains(&mWindowSwapchain)
      .setWaitSemaphoreCount(1)
      .setPWaitSemaphores(&mSwapchainRenderFinishedSemaphores[mFrameSyncCounter]);

    result = Vulkan::graphicsQueue().presentKHR(&present);

    // If presentation was out of date, not optimal or window was resized, rebuild swapchain
    if(result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || mWindowResized) {
      Vulkan::waitForIdle();
      Vulkan::refreshCapabilities();

      _destroySwapchainRelatedObjects();

      _createSwapchain();
    }

    // Call late update on everything that is not image related (input, physics threads update, setting up data for next frame)
    lateUpdate();

    // Reset window resized flag
    mWindowResized = false;
  }

  return *this;
}

sf::Window& sf::Window::fullscreen() {
  int width = 0;
  int height = 0;

  // Enable fullscreen
  glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), nullptr, nullptr, &width, &height);
  glfwSetWindowMonitor(mWindow, glfwGetPrimaryMonitor(), 0, 0, width, height, GLFW_DONT_CARE);

  mWindowResized = true;

  return *this;
}

sf::Window& sf::Window::windowed() {
  // Disable fullscreen
  glfwSetWindowMonitor(mWindow, nullptr, 0, 0, mWidth, mHeight, GLFW_DONT_CARE);

  mWindowResized = true;

  return *this;
}

sf::Window& sf::Window::resize(const int width, const int height) {
  // If you want you can resize window manually
  mWindowResized = true;

  mWidth = width;
  mHeight = height;

  glfwSetWindowSize(mWindow, mWidth, mHeight);

  return *this;
}

bool sf::Window::isResized() {
  return mWindowResized;
}

uint32_t sf::Window::getWidth() {
  return mWidth;
}

uint32_t sf::Window::getHeight() {
  return mHeight;
}

const vk::Image& sf::Window::getCurrentImage() const {
  return mWindowSwapchainImages[mSwapchainCurrentImageIndex];
}

const vk::ImageView& sf::Window::getCurrentImageView() const {
  return mWindowSwapchainImageViews[mSwapchainCurrentImageIndex];
}

const std::vector<vk::Image>& sf::Window::getImages() const {
  return mWindowSwapchainImages;
}

const std::vector<vk::ImageView>& sf::Window::getImageViews() const {
  return mWindowSwapchainImageViews;
}

vk::Fence& sf::Window::getCurrentFence() {
  return mSwapchainFencesInFlight[mFrameSyncCounter];
}

vk::Semaphore& sf::Window::getCurrentImageAvailableSemaphore() {
  return mSwapchainImageAvailableSemaphores[mFrameSyncCounter];
}

vk::Semaphore& sf::Window::getCurrentRenderFinishedSemaphore() {
  return mSwapchainRenderFinishedSemaphores[mFrameSyncCounter];
}

const uint32_t sf::Window::getCurrentImageIndex() const {
  return mSwapchainCurrentImageIndex;
}

void sf::Window::destroy() {
  // Wait for everything to be idle to destroy
  Vulkan::waitForIdle();

  // Firstly end everything user defined or scene defined
  end();

  // Then destroy vulkan window objects (swapchain and everything related) and critical objects
  _destroySwapchainRelatedObjects();

  Vulkan::device().destroySwapchainKHR(mWindowSwapchain, nullptr);

  Vulkan::getVk().destroy();

  glfwDestroyWindow(mWindow);
  glfwTerminate();
}
