#include "sfRenderPassManager.h"
#include "../Core/sfWindow.h"

sf::RenderPassManager::~RenderPassManager() {
	destroy();
}

sf::RenderPassManager& sf::RenderPassManager::addPass(const std::string& name) {
	mDescriptions.push_back()
}

sf::RenderPassManager& sf::RenderPassManager::addSubpass(const std::string& renderPassName, const RenderSubpass& subpassDesc) {

}

sf::RenderPassManager& sf::RenderPassManager::build() {

}

sf::RenderPassManager& sf::RenderPassManager::execute(vkw::CommandBuffer& commandBuffer) {

}

void sf::RenderPassManager::destroy() {
	for (RenderPassBuildInfo& rpBuild : mBuild) {
		if (rpBuild.renderPass)
			vkDestroyRenderPass(Window::getVk().getDevice(), rpBuild.renderPass, nullptr);
	}
}