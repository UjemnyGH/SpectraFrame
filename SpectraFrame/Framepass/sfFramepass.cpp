#include "sfFramepass.h"

sf::Framepass::~Framepass() {
  destroy();
}

void sf::Framepass::add(const std::string& passName, std::function<void(FramepassResource&)> execute) {
  mFramepasses.push_back((Pass){.passName = passName, .renderPass = nullptr, .execute = execute});
  mFramepassesMapped[passName] = &mFramepasses[mFramepasses.size() - 1];

  vk::RenderPassCreateInfo2KHR renderPassInfo;
  renderPassInfo
    .
}
