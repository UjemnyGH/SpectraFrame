#include "sfRenderPassManager.h"
#include "../Core/sfWindow.h"

sf::RenderPassManager::~RenderPassManager() {
	destroy();
}

sf::RenderPassManager& sf::RenderPassManager::addPass(const std::string& name) {
	//mDescriptions.push_back()

	return *this;
}

sf::RenderPassManager& sf::RenderPassManager::addSubpass(const std::string& renderPassName, const RenderSubpass& subpassDesc) {
	return *this;
}

sf::RenderPassManager& sf::RenderPassManager::build() {
	return *this;
}

sf::RenderPassManager& sf::RenderPassManager::execute(vkw::CommandBuffer& commandBuffer) {
	return *this;
}

void sf::RenderPassManager::destroy() {
	for (RenderPassBuildInfo& rpBuild : mBuild) {
		if (rpBuild.renderPass)
			vkDestroyRenderPass(Window::getVk().getDevice(), rpBuild.renderPass, nullptr);
	}
}