#pragma once
#ifndef _SPECTRAFRAME_FRAMEPASS_
#define _SPECTRAFRAME_FRAMEPASS_

#include <functional>
#include "sfFramepassResource.h"

namespace sf {
  struct Pass {
    std::string passName;
    
    vk::RenderPass renderPass;

    std::function<void(FramepassResource&)> execute;
  };

  class Framepass {
  private:
    FramepassResource mResources;

    std::vector<Pass> mFramepasses;
    std::unordered_map<std::string, Pass*> mFramepassesMapped;

  public:

    ~Framepass();

    /**
     * Adds new pass to framepass, passName is also resource name that image will be rendered in, so don`t forget to add #color@rgba32f prefix to match output
     */    
    void add(const std::string& passName, std::function<void(FramepassResource&)> execute);

    /**
     * Executes desired pass, can write image to additional images, usefull for swapchain
     */
    void execute(const std::string& passName, std::vector<vk::ImageView>& rAdditionalImageOutputs);

    /**
     * Destroys single, desired pass
     */
    void destroy(const std::string& passName);

    /**
     * Destroys all framepasses
     */
    void destroy();
  };
}

#endif
