#include "sfSwapchain.h"

sf::Swapchain::~Swapchain() {
  destroy();
}

void sf::Swapchain::create(const uint32_t width, const uint32_t height) {
  destroy();

  vk::SwapchainCreateInfoKHR swapchainInfo;
  swapchainInfo
    .setClipped(vk::True)
    .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
    .setMinImageCount(Vulkan::surfaceCapabilities().minImageCount <= 2 ? 3 : Vulkan::surfaceCapabilities().minImageCount)
    .setImageArrayLayers(1)
    .setImageColorSpace(Vulkan::surfaceFormat().colorSpace)
    .setImageFormat(Vulkan::surfaceFormat().format)
    .setImageExtent(vk::Extent2D(width, height))
    .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
    .setImageSharingMode(vk::SharingMode::eExclusive)
    .setPresentMode(Vulkan::presentMode())
    .setQueueFamilyIndexCount(0)
    .setPreTransform(Vulkan::surfaceCapabilities().currentTransform)
    .setSurface(Vulkan::surface());

  if(width == 0 || height == 0)
    swapchainInfo.setImageExtent(vk::Extent2D(Window::sWindowInstancePtr->getWidth(), Window::sWindowInstancePtr->getHeight()));

  Vulkan::device().createSwapchainKHR(&swapchainInfo, nullptr, &mSwapchain);

  uint32_t count = 0;
  Vulkan::device().getSwapchainImagesKHR(mSwapchain, &count, nullptr);

  mSwapchainImages.resize(count);
  Vulkan::device().getSwapchainImagesKHR(mSwapchain, &count, mSwapchainImages.data());

  mSwapchainImageViews.resize(mSwapchainImages.size());

  for(uint32_t i = 0; i < mSwapchainImages.size(); i++) {
    vk::ImageViewCreateInfo imageViewInfo;
    imageViewInfo
      .setImage(mSwapchainImages[i])
      .setViewType(vk::ImageViewType::e2D)
      .setComponents(vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA))
      .setFormat(Vulkan::surfaceFormat().format)
      .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

    Vulkan::device().createImageView(&imageViewInfo, nullptr, &mSwapchainImageViews[i]);
  }
}

void sf::Swapchain::destroy() {
  Vulkan::device().destroySwapchainKHR(mSwapchain, nullptr);

  for(vk::ImageView iv : mSwapchainImageViews) {
    Vulkan::device().destroyImageView(iv, nullptr);
  }

  mSwapchainImageViews.clear();
  mSwapchainImages.clear();
}

vk::Image& sf::Swapchain::getImage() {
  return mSwapchainImages[getImageIndex()];
}

vk::ImageView& sf::Swapchain::getView() {
  return mSwapchainImageViews[getImageIndex()];
}

uint32_t sf::Swapchain::getImageIndex() {
  return mAquiredImageIndex;
}

vk::Result sf::Swapchain::acquireImage(vk::Semaphore imageAquiredSemaphore) { 
  return Vulkan::device().acquireNextImageKHR(mSwapchain, ~0ULL, imageAquiredSemaphore, nullptr, &mAquiredImageIndex);
}

vk::Result sf::Swapchain::present(vk::Semaphore renderFinishedSemaphore) {
  vk::PresentInfoKHR presentInfo;
  presentInfo
    .setSwapchainCount(1)
    .setPSwapchains(&mSwapchain)
    .setWaitSemaphoreCount(1)
    .setPWaitSemaphores(&renderFinishedSemaphore)
    .setPImageIndices(&mAquiredImageIndex);

  return Vulkan::graphicsQueue().presentKHR(presentInfo);
}
