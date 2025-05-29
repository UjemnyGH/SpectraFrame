#pragma once
#ifndef _SPECTRAFRAME_SWAPCHAIN_
#define _SPECTRAFRAME_SWAPCHAIN_

#include "sfWindow.h"

namespace sf {
  class Swapchain {
  private:
    vk::SwapchainKHR mSwapchain;  
    std::vector<vk::Image> mSwapchainImages;
    std::vector<vk::ImageView> mSwapchainImageViews;
    uint32_t mAquiredImageIndex;

  public:
    ~Swapchain();

    void create(const uint32_t width = 0, const uint32_t height = 0);

    void destroy();

    vk::Image& getImage();

    vk::ImageView& getView();

    uint32_t getImageIndex();

    vk::Result acquireImage(vk::Semaphore imageAquiredSemaphore);

    vk::Result present(vk::Semaphore renderFinishedSemaphore);
  };
}

#endif
