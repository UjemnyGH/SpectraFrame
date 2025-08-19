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

sf::Window::Window() : mThreadPool(std::thread::hardware_concurrency()) {
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

    if(Vulkan::device().createImageView(&imageViewInfo, nullptr, &mWindowSwapchainImageViews[i]) != vk::Result::eSuccess)
      SF_CLOG("ERR: Cannot create swapchain image view " << i);

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

void sf::Window::_createSurface() {
  // Create surface
#ifdef VK_USE_PLATFORM_WIN32_KHR
  vk::Win32SurfaceCreateInfoKHR win32SurfaceInfo{};
  win32SurfaceInfo
    .setHwnd(glfwGetWin32Window(mWindow))  
    .setHinstance(GetModuleHandle());

  mWindowSurface = Vulkan::instance().createWin32SurfaceKHR(win32SurfaceInfo);

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
  vk::XlibSurfaceCreateInfoKHR xlibSurfaceInfo{};
  xlibSurfaceInfo
    .setDpy(glfwGetX11Display())
    .setWindow(glfwGetX11Window(mWindow));

  mWindowSurface = Vulkan::instance().createXlibSurfaceKHR(xlibSurfaceInfo);

#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
  vk::WaylandSurfaceCreateInfoKHR waylandSurfaceInfo{};
  waylandSurfaceInfo
    .setDisplay(glfwGetWaylandDisplay())
    .setSurface(glfwGetWaylandWindow(mWindow));

  mWindowSurface = Vulkan::instance().createWaylandSurfaceKHR(waylandSurfaceInfo);
#endif
}

sf::Window& sf::Window::window() {
  return *sWindowInstancePtr;
}

sf::Window& sf::Window::beginFrame() {
  return window().beginFrameRendering();
}

sf::Window& sf::Window::endFrame() {
  return window().endFrameRendering();
}

sf::Window& sf::Window::sFullscreen() {
  window().fullscreen();

  return window();
}

sf::Window& sf::Window::sWindowed() {
  window().windowed();

  return window();
}

bool sf::Window::sIsResized() {
  return window().isResized();
}

uint32_t sf::Window::width() {
  return window().getWidth();
}

uint32_t sf::Window::height() {
  return window().getHeight();
}

const vk::Image& sf::Window::currentImage() {
  return window().getCurrentImage();
}

const vk::ImageView& sf::Window::currentView() {
  return window().getCurrentImageView();
}

const std::vector<vk::Image>& sf::Window::images() {
  return window().getImages();
}

const std::vector<vk::ImageView>& sf::Window::imageViews() {
  return window().getImageViews();
}

vk::Fence& sf::Window::currentFence() {
  return window().getCurrentFence();
}

vk::Semaphore& sf::Window::currentImageAvailableSemaphore() {
  return window().getCurrentImageAvailableSemaphore();
}

vk::Semaphore& sf::Window::currentRenderFinishedSemaphore() {
  return window().getCurrentRenderFinishedSemaphore();
}

vk::CommandPool& sf::Window::commandPool() {
  return window().getCommandPool();
}

vk::CommandBuffer& sf::Window::currentCommandBuffer() {
  return window().getCurrentCommandBuffer();
}

const uint32_t sf::Window::imageIndex() {
  return window().getCurrentImageIndex();
}

sf::ThreadPool& sf::Window::threadPool() {
  return window().getThreadPool();
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

  SF_CLOG("Using Win32");
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    .graphicsRequiredExtensions(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);

  SF_CLOG("Using X11");
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    .graphicsRequiredExtensions(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
    
  SF_CLOG("Using Wayland");
#endif

  // If needed create debug messenger
  if (mCreateVulkanDebugger)
    Vulkan::getVk().enableDebugMessenger(false);

  // Create instance
  Vulkan::getVk().createInstance();

  _createSurface();

  // Pass surface to Vulkan class, pick best gpu and create device
  Vulkan::getVk()
    .setSurface(mWindowSurface)
    .pickGPU()
    .createDevice();

  // Then make swapchain
  _createSwapchain();

  createVkCommandPool(mSwapchainCommandPool, Vulkan::graphicsQueueIndex());
  
  mSwapchainCommandBuffers.resize(mWindowSwapchainImages.size());

  for(int i = 0; i < mSwapchainCommandBuffers.size(); i++) {
    allocateCommandBuffer(mSwapchainCommandBuffers[i], mSwapchainCommandPool);  
  }

  // And call start virtual function
  start();

  return *this;
}

sf::Window& sf::Window::runLoop() {
  // Check if we still want to have opened window
  while (!glfwWindowShouldClose(mWindow)) {
    // Run through counter for synchronization objects
    mFrameSyncCounter = (mFrameSyncCounter++) % mWindowSwapchainImages.size();

    glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);

    // Poll glfw for events
    glfwPollEvents();

    // Wait for fences and reset them to signaled stage
    if(Vulkan::device().waitForFences(1, &getCurrentFence(), vk::True, UINT64_MAX) != vk::Result::eSuccess)
      SF_CLOG("ERR: Waiting for fence was not sucessful?!"); 

    if(Vulkan::device().resetFences(1, &getCurrentFence()) != vk::Result::eSuccess)
      SF_CLOG("ERR: Reseting fence was not succesful?!");

    // Acquire image index
    vk::AcquireNextImageInfoKHR acquireInfo{};
    acquireInfo
      .setSemaphore(getCurrentImageAvailableSemaphore())
      .setSwapchain(mWindowSwapchain)
      .setFence(nullptr)
      .setDeviceMask(1)
      .setTimeout(~0ULL);

    vk::Result result = Vulkan::device().acquireNextImage2KHR(&acquireInfo, &mSwapchainCurrentImageIndex); 

    // If swapchain was out of date, rebuild it
    if(result == vk::Result::eErrorOutOfDateKHR || mWindowResized) {
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
      .setPWaitSemaphores(&getCurrentRenderFinishedSemaphore());

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

sf::Window& sf::Window::beginFrameRendering() {
  vk::CommandBufferBeginInfo beginInfo{};

  if(getCurrentCommandBuffer().begin(&beginInfo) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot begin sweapchain command buffer!");

  return window();
}

sf::Window& sf::Window::endFrameRendering() {
  getCurrentCommandBuffer().end();

  vk::CommandBufferSubmitInfo cmdInfo;
  cmdInfo
    .setCommandBuffer(getCurrentCommandBuffer());

  vk::SemaphoreSubmitInfo waitInfo;
  waitInfo
    .setSemaphore(getCurrentImageAvailableSemaphore())
    .setStageMask(vk::PipelineStageFlagBits2::eTopOfPipe);
    
  vk::SemaphoreSubmitInfo sigInfo;
  sigInfo
    .setSemaphore(getCurrentRenderFinishedSemaphore())
    .setStageMask(vk::PipelineStageFlagBits2::eTopOfPipe);

  vk::SubmitInfo2 submitInfo{};
  submitInfo
    .setCommandBufferInfoCount(1)
    .setPCommandBufferInfos(&cmdInfo)
    .setWaitSemaphoreInfoCount(1)
    .setPWaitSemaphoreInfos(&waitInfo)
    .setSignalSemaphoreInfoCount(1)
    .setPSignalSemaphoreInfos(&sigInfo);

  if(sf::Vulkan::graphicsQueue().submit2(1, &submitInfo, getCurrentFence()) != vk::Result::eSuccess)
      SF_CLOG("ERR: Cannot submit command buffer");

  return window();
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

vk::CommandPool& sf::Window::getCommandPool() {
  return mSwapchainCommandPool;
}

vk::CommandBuffer& sf::Window::getCurrentCommandBuffer() {
  return mSwapchainCommandBuffers[mFrameSyncCounter];
}

const uint32_t sf::Window::getCurrentImageIndex() const {
  return mSwapchainCurrentImageIndex;
}

sf::ThreadPool& sf::Window::getThreadPool() {
  return mThreadPool;
}

void sf::Window::destroy() {
  mThreadPool.stop();

  // Wait for everything to be idle to destroy
  Vulkan::waitForIdle();

  // Firstly end everything user defined or scene defined
  end();

  Vulkan::device().freeCommandBuffers(mSwapchainCommandPool, static_cast<uint32_t>(mSwapchainCommandBuffers.size()), mSwapchainCommandBuffers.data());
  Vulkan::device().destroyCommandPool(mSwapchainCommandPool, nullptr);

  // Then destroy vulkan window objects (swapchain and everything related) and critical objects
  _destroySwapchainRelatedObjects();

  Vulkan::device().destroySwapchainKHR(mWindowSwapchain, nullptr);
  
  Vulkan::getVk().destroy();

  glfwDestroyWindow(mWindow);
  glfwTerminate();
}
