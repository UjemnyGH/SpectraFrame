#pragma once
#ifndef _SPECRTAFRAME_WINDOW_
#define _SPECRTAFRAME_WINDOW_

#include "sfVkCore.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

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

    bool mWindowResized;
    int mWidth;
    int mHeight;

    bool mCreateVulkanDebugger;

    static void _framebufferCallback(GLFWwindow* wnd, int width, int height);

    void _createSwapchain();

    void _destroySwapchainRelatedObjects();

  public:
    static Window* sWindowInstancePtr;
    
    Window();

    ~Window();

    virtual void start() {}

    virtual void update() {}

    virtual void lateUpdate() {}

    virtual void end() {}

    Window& useVulkanDebugger(bool useDebugger = false);

    Window& create(const char* title, const int width, const int height);

    Window& runLoop();

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

    const uint32_t getCurrentImageIndex() const; 

    void destroy();
  };
}

#endif
