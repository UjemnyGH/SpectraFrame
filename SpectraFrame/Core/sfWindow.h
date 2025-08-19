#pragma once
#ifndef _SPECRTAFRAME_WINDOW_
#define _SPECRTAFRAME_WINDOW_

#include "sfVkCore.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "sfThreadPoolWorker.h"

namespace sf {
  class Window {
  private:
    GLFWwindow* mWindow;

    vk::SurfaceKHR mWindowSurface;
    
    vk::SwapchainKHR mWindowSwapchain;
    std::vector<vk::Image> mWindowSwapchainImages;
    std::vector<vk::ImageView> mWindowSwapchainImageViews;
    
    uint32_t mSwapchainCurrentImageIndex;
    uint32_t mFrameSyncCounter;

    std::vector<vk::Fence> mSwapchainFencesInFlight;
    std::vector<vk::Semaphore> mSwapchainImageAvailableSemaphores;
    std::vector<vk::Semaphore> mSwapchainRenderFinishedSemaphores;

    vk::CommandPool mSwapchainCommandPool;
    std::vector<vk::CommandBuffer> mSwapchainCommandBuffers;

    bool mWindowResized;
    int mWidth;
    int mHeight;

    bool mCreateVulkanDebugger;

    ThreadPool mThreadPool;

    static void _framebufferCallback(GLFWwindow* wnd, int width, int height);

    void _createSwapchain();

    void _destroySwapchainRelatedObjects();

    void _createSurface();

  public:
    static Window* sWindowInstancePtr;
   
    static Window& window();

    static Window& beginFrame();

    static Window& endFrame();

    static Window& sFullscreen();

    static Window& sWindowed();

    static bool sIsResized();

    static uint32_t width();

    static uint32_t height();

    static const vk::Image& currentImage();

    static const vk::ImageView& currentView();

    static const std::vector<vk::Image>& images();

    static const std::vector<vk::ImageView>& imageViews();

    static vk::Fence& currentFence();

    static vk::Semaphore& currentImageAvailableSemaphore();

    static vk::Semaphore& currentRenderFinishedSemaphore();

    static vk::CommandPool& commandPool();

    static vk::CommandBuffer& currentCommandBuffer();

    static const uint32_t imageIndex();

    static ThreadPool& threadPool();

    Window();

    virtual void start() {}

    virtual void update() {}

    virtual void lateUpdate() {}

    virtual void end() {}

    Window& useVulkanDebugger(bool useDebugger = false);

    Window& create(const char* title, const int width, const int height);

    Window& runLoop();

    Window& beginFrameRendering();

    Window& endFrameRendering();

    Window& fullscreen();

    Window& windowed();

    Window& resize(const int width, const int height);

    bool isResized();

    uint32_t getWidth();

    uint32_t getHeight();

    const vk::Image& getCurrentImage() const;

    const vk::ImageView& getCurrentImageView() const;

    const std::vector<vk::Image>& getImages() const;

    const std::vector<vk::ImageView>& getImageViews() const;

    vk::Fence& getCurrentFence();

    vk::Semaphore& getCurrentImageAvailableSemaphore();

    vk::Semaphore& getCurrentRenderFinishedSemaphore();

    vk::CommandPool& getCommandPool();

    vk::CommandBuffer& getCurrentCommandBuffer();

    const uint32_t getCurrentImageIndex() const;

    ThreadPool& getThreadPool();

    void destroy();
  };
}

#endif
