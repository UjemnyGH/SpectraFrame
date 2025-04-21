#include "Core/sfWindow.h"
#include "Framegraph/sfFramegraph.h"
#include "Scene/sfScene.h"
#include "Core/sfLogger.h"
#include <stb_image.h>

#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Math/sfVector.h"
#include "Math/sfMatrix.h"
#include "Math/sfQuaterion.h"

class TestScene : public sf::Scene {
public:
	TestScene() : sf::Scene("TestScene", true) {}

	struct PipelineData {
		vkw::DescriptorSetLayout setLayout;
		vkw::DescriptorPool pool;
		vkw::DescriptorSet set;

		std::vector<vkw::WriteDescriptorSet> writeSets;

		vkw::PipelineLayout pipelineLayout;
		vkw::Pipeline pipeline;
	};

	vkw::Swapchain swapchain;
	vkw::Fence fence;
	vkw::Semaphore imageAvailable, renderFinished;
	//std::vector<vkw::ImageView> swapchainImageViews;
	vkw::CommandBuffer drawCmd;
	vkw::CommandPool commandPool;

	PipelineData geometryPipeline;
	PipelineData lightPipeline;

	vkw::Buffer uniformBuffer;

	vkw::Image textureImage;
	vkw::ImageView textureImageView;
	vkw::Sampler textureSampler;

	struct PVT {
		sf::FMat4 projection;
		sf::FMat4 view;
		sf::FMat4 transform;
	} *pvt;

	virtual void start() {
		SF_CDEBUG("Started test scene");

		swapchain
			.setCorePtr(&sf::Window::getVk())
			.create({sf::Window::sWindowInstancePtr->getWidth(), sf::Window::sWindowInstancePtr->getHeight()});

		fence
			.setCorePtr(&sf::Window::getVk())
			.signaled()
			.create();
		imageAvailable
			.setCorePtr(&sf::Window::getVk())
			.create();
		renderFinished
			.setCorePtr(&sf::Window::getVk())
			.create();

		commandPool
			.setCorePtr(&sf::Window::getVk())
			.resetCommandBuffersFlag()
			.create(sf::Window::getVk().getGraphicsQueueIndex());

		drawCmd
			.setCorePtr(&sf::Window::getVk())
			.allocate(commandPool.getCommandPool());

		geometryPipeline.setLayout
			.setCorePtr(&sf::Window::getVk())
			.addBinding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.addBinding(1, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
			.create();

		geometryPipeline.pool
			.setCorePtr(&sf::Window::getVk())
			.addSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1)
			.addSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)
			.create();

		geometryPipeline.set
			.setCorePtr(&sf::Window::getVk())
			.setLayouts(&geometryPipeline.setLayout.getSetLayout())
			.allocate(geometryPipeline.pool.getPool());

		uniformBuffer
			.setCorePtr(&sf::Window::getVk())
			.usage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
			.properties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			.create(sizeof(PVT))
			.map((void**)&pvt, sizeof(PVT));

		int width = 0, height = 0, nrChann = 0;
		// There it is, my profile picture (I didnt have better image for texture thus it stays there till the end of github servers)
		uint8_t* pixels = stbi_load("testTexture.jpg", &width, &height, &nrChann, STBI_rgb_alpha);

		void* tData;

		vkw::Buffer textureStageBuffer;
		textureStageBuffer
			.setCorePtr(&sf::Window::getVk())
			.properties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			.usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
			.create(width * height * 4)
			.map(&tData, width * height * 4);

		memcpy(tData, pixels, width * height * 4);

		textureStageBuffer.unmap();

		stbi_image_free(pixels);

		textureImage
			.setCorePtr(&sf::Window::getVk())
			.properties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
			.usage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
			.extent({ (uint32_t)width, (uint32_t)height, 1 })
			.create();

		vkw::CommandBuffer oneTimeCopy;
		sf::TransitionImage texBarrier, texBarrierShaderReady;
		texBarrier
			.setCommandBuffer(oneTimeCopy)
			.accessMask(0, VK_ACCESS_TRANSFER_WRITE_BIT)
			.subresources({ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 })
			.layouts(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		texBarrierShaderReady
			.setCommandBuffer(oneTimeCopy)
			.accessMask(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT)
			.subresources({ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 })
			.layouts(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		sf::CopyBuffer copyImage;
		copyImage
			.setCommandBuffer(oneTimeCopy);

		sf::commandBufferOneTimeBegin(oneTimeCopy, commandPool);
		texBarrier.transition(textureImage, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
		copyImage.copyBufferToImage(textureStageBuffer, textureImage, { (uint32_t)width, (uint32_t)height, 1 }, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 });
		texBarrierShaderReady.transition(textureImage, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
		sf::commandBufferOneTimeEnd(oneTimeCopy, sf::Window::getVk().getGraphicsQueue());

		textureStageBuffer.destroy();

		textureImageView
			.setCorePtr(&sf::Window::getVk())
			.format(VK_FORMAT_R8G8B8A8_SRGB)
			.subresource(VK_IMAGE_ASPECT_COLOR_BIT)
			.create(textureImage.getImage());

		textureSampler
			.setCorePtr(&sf::Window::getVk())
			.maxAnisotropy(-1)
			.create();

		geometryPipeline.writeSets.resize(2);
		geometryPipeline.writeSets[0]
			.setDescriptorSet(geometryPipeline.set.getSet())
			.type(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
			.binding(0)
			.asUniformBuffer(uniformBuffer.getBuffer(), sizeof(PVT));

		geometryPipeline.writeSets[1]
			.setDescriptorSet(geometryPipeline.set.getSet())
			.type(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
			.binding(1)
			.asSampler(textureImageView.getImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, textureSampler.getSampler());
	}

	virtual void update() {
		uint32_t imageIndex = 0;

		fence.wait();

		VkResult result = vkAcquireNextImageKHR(sf::Window::getVk().getDevice(), swapchain.getSwapchain(), ~(0ULL), imageAvailable.getSemaphore(), VK_NULL_HANDLE, &imageIndex);

		fence.reset();

		drawCmd.begin();

		drawCmd.end();

		vkw::Submit submit;
		submit
			.addCommandBuffer(drawCmd.getCommandBuffer())
			.addSignal(renderFinished.getSemaphore())
			.addWait(imageAvailable.getSemaphore())
			.addWaitDstMask(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
			.submit(sf::Window::getVk().getGraphicsQueue(), fence.getFence());

		vkw::Present present;
		result = present
			.addSwapchain(swapchain.getSwapchain())
			.addWait(renderFinished.getSemaphore())
			.present(sf::Window::getVk().getGraphicsQueue(), &imageIndex);
	}

	virtual void lateUpdate() {
		
	}

	virtual void end() {
		swapchain.destroy();

		SF_CDEBUG("Ended test scene");
	}
} G_TEST_SCENE;