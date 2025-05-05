#include "Core/sfWindow.h"
#include "Framegraph/sfFramegraph.h"
#include "Scene/sfScene.h"
#include "Core/sfLogger.h"
#include "stb_image.h"

#include "Math/sfQuaterion.h"

#include "Core/sfPLYLoader.h"
#include "Core/sfCommon.h"

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
	std::vector<vkw::ImageView> swapchainViews;
	std::vector<vkw::Framebuffer> swapchainFramebuffers;
	
	vkw::Fence fence;
	vkw::Semaphore imageAvailable, renderFinished;
	
	vkw::CommandBuffer drawCmd;
	vkw::CommandPool commandPool;

	PipelineData geometryPipeline;
	PipelineData lightPipeline;
	vkw::RenderPass renderPass;

	vkw::Buffer uniformBuffer;

	vkw::Image textureImage;
	vkw::ImageView textureImageView;
	vkw::Sampler textureSampler;

	sf::DepthImage depthImage;

	sf::VertexBuffer vertexBuffer;

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

		std::vector<float> vBufferData;
		sf::MeshRawData mesh = sf::loadPLYMesh("cube.ply");
		sf::makeOneBufferData(mesh.vertices, mesh.normals, mesh.textureCoordinates, vBufferData);

		vertexBuffer.update(commandPool, sf::Window::getVk().getGraphicsQueue(), vBufferData);

		geometryPipeline.setLayout
			.setCorePtr(&sf::Window::getVk())
			.addBinding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.addBinding(1, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
			.create();

		geometryPipeline.pool
			.setCorePtr(&sf::Window::getVk())
			.flags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
			.addSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1)
			.addSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)
			.create();

		geometryPipeline.set
			.setCorePtr(&sf::Window::getVk())
			.setLayouts(&geometryPipeline.setLayout.getSetLayout())
			.allocate(geometryPipeline.pool.getPool());

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

		geometryPipeline.set.update(geometryPipeline.writeSets);

		geometryPipeline.pipelineLayout
			.setCorePtr(&sf::Window::getVk())
			.addLayouts(geometryPipeline.setLayout.getSetLayout())
			.create();

		std::vector<uint8_t> vertexShaderBin, fragmentShaderBin;
		sf::readFile("g_buffer_vert.spv", vertexShaderBin);
		sf::readFile("g_buffer_frag.spv", fragmentShaderBin);

		vkw::ShaderModule vertexShader, fragmentShader;
		vertexShader
			.setCorePtr(&sf::Window::getVk())
			.create(&vertexShaderBin[0], vertexShaderBin.size());

		fragmentShader
			.setCorePtr(&sf::Window::getVk())
			.create(&fragmentShaderBin[0], fragmentShaderBin.size());

		renderPass
			.setCorePtr(&sf::Window::getVk())
			.addAttachment("ColorAttachment", sf::Window::getVk().getSurfaceFormat().format, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			.addAttachment("DepthAtatchment", sf::Window::getVk().getDepthFormat(), VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
			.addSubpass(true)
			.subpassAddOutputRef(0, renderPass.getAttachmentId("ColorAttachment"), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT)
			.subpassDepthRef(0, renderPass.getAttachmentId("DepthAttachment"), VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT)
			.addSubpassDependency(
				0,
				VK_SUBPASS_EXTERNAL,
				0,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
				0,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
			)
			.create();

		geometryPipeline.pipeline
			.setCorePtr(&sf::Window::getVk())
			.addBinding(0, sizeof(vBufferData[0])* vBufferData.size())
			.addAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0)
			.addAttribute(1, 0, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3)
			.addAttribute(2, 0, VK_FORMAT_R32G32_SFLOAT, sizeof(float) * 6)
			.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT)
			.addDynamicState(VK_DYNAMIC_STATE_SCISSOR)
			.depthBounds(0.0f, 1.0f)
			.depthCompareOp(VK_COMPARE_OP_LESS)
			.depthStencilEnable(VK_TRUE, VK_TRUE, VK_FALSE, VK_FALSE)
			.viewport(swapchain.getExtent())
			.scissor(swapchain.getExtent())
			.addShaderStage(vertexShader.getShaderModule(), VK_SHADER_STAGE_VERTEX_BIT)
			.addShaderStage(fragmentShader.getShaderModule(), VK_SHADER_STAGE_FRAGMENT_BIT)
			.addColorBlendAttachment(
				VkPipelineColorBlendAttachmentState{
					.blendEnable = VK_FALSE,
					.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
					.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
					.colorBlendOp = VK_BLEND_OP_ADD,
					.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
					.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
					.alphaBlendOp = VK_BLEND_OP_ADD,
					.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
				}
			)
			.layout(geometryPipeline.pipelineLayout.getLayout())
			.renderPass(renderPass.getRenderPass())
			.createGraphics();

		depthImage.create({ swapchain.getExtent().width, swapchain.getExtent().height, 1 }, commandPool);
	}

	virtual void update() {
		do {

		uint32_t imageIndex = 0;

		fence.wait();

		VkResult result = vkAcquireNextImageKHR(sf::Window::getVk().getDevice(), swapchain.getSwapchain(), ~(0ULL), imageAvailable.getSemaphore(), VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			sf::Window::getVk()
				.waitForDeviceIdle()
				.refreshSurfaceCapabilities();

			for (vkw::Framebuffer& sfb : swapchainFramebuffers) {
				sfb
					.clearAttachments()
					.destroy();
			}

			for (vkw::ImageView& siv : swapchainViews) {
				siv.destroy();
			}

			swapchainFramebuffers.clear();
			swapchainViews.clear();

			depthImage.destroy();

			swapchain.destroy();

			swapchain.create({ (uint32_t)sf::Window::sWindowInstancePtr->getWidth(), (uint32_t)sf::Window::sWindowInstancePtr->getHeight() });

			swapchainViews.resize(swapchain.getImagesAmount());

			for (uint32_t i = 0; i < swapchainViews.size(); i++) {
				swapchainViews[i]
					.setCorePtr(&sf::Window::getVk())
					.create(swapchain.swapchainImages[i]);
			}

			depthImage.create({ swapchain.getExtent().width, swapchain.getExtent().height, 1 }, commandPool);

			swapchainFramebuffers.resize(swapchainViews.size());

			for (uint32_t i = 0; i < swapchainFramebuffers.size(); i++) {
				swapchainFramebuffers[i]
					.setCorePtr(&sf::Window::getVk())
					.renderPass(renderPass.getRenderPass())
					.addAttachment(swapchainViews[i].getImageView())
					.addAttachment(depthImage.getView().getImageView())
					.extent(swapchain.getExtent())
					.create();
			}

			continue;
		}

		assert((result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR) && "An error occured while aquireing next image");

		fence.reset();

		VkClearValue clear[2];
		clear[0].color = { {0.1f, 0.1f, 0.1f, 1.0f} };
		clear[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassBegin{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
		renderPassBegin.renderPass = renderPass.getRenderPass();
		renderPassBegin.framebuffer = swapchainFramebuffers[imageIndex].getFramebuffer();
		renderPassBegin.renderArea.offset = { 0, 0 };
		renderPassBegin.renderArea.extent = swapchain.getExtent();
		renderPassBegin.clearValueCount = sizeof(clear) / sizeof(VkClearValue);
		renderPassBegin.pClearValues = clear;

		VkSubpassBeginInfo subpassInfo{ VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO };
		subpassInfo.contents = VK_SUBPASS_CONTENTS_INLINE;

		VkSubpassEndInfo subpassEnd{ VK_STRUCTURE_TYPE_SUBPASS_END_INFO };

		VkViewport viewport{
			0.0f, (float)swapchain.getExtent().height,
			(float)swapchain.getExtent().width, -(float)swapchain.getExtent().width,
			0.0f, 1.0f
		};

		VkRect2D scissor{
			{0, 0},
			swapchain.getExtent()
		};

		VkDeviceSize offset[] = { 0 };

		pvt->projection = sf::FMat4::perspectiveFOV(3.1415f / 2.0f, swapchain.getExtent().width, swapchain.getExtent().height, 0.001f, 1000.0f);
		pvt->view = sf::FMat4::lookAt(sf::FVec3(0.0f, 0.0f, -1.0f), sf::FVec3(0.0f), sf::FVec3(0.0f, 1.0f));
		pvt->transform = sf::FMat4::rotateX(glfwGetTime()) * sf::FMat4::rotateY(glfwGetTime()) * sf::FMat4::rotateZ(glfwGetTime());

		drawCmd
			.begin()
			.cmdBeginRenderPass2(&renderPassBegin, &subpassInfo)
			.cmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, geometryPipeline.pipeline.getPipeline())
			.cmdSetViewport(0, 1, &viewport)
			.cmdSetScissor(0, 1, &scissor)
			.cmdBindVertexBuffers(0, 1, &vertexBuffer.getBuffer().getBuffer(), offset)
			.cmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_COMPUTE, geometryPipeline.pipelineLayout.getLayout(), 0, 1, &geometryPipeline.set.getSet(), 0, nullptr)
			.cmdDraw(36, 1, 0, 0)
			.cmdEndRenderPass2(&subpassEnd)
			.end();

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

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || sf::Window::sWindowInstancePtr->isResized()) {
			sf::Window::getVk()
				.waitForDeviceIdle()
				.refreshSurfaceCapabilities();

			for (vkw::Framebuffer& sfb : swapchainFramebuffers) {
				sfb
					.clearAttachments()
					.destroy();
			}

			for (vkw::ImageView& siv : swapchainViews) {
				siv.destroy();
			}

			swapchainFramebuffers.clear();
			swapchainViews.clear();

			depthImage.destroy();

			swapchain.destroy();

			swapchain.create({ (uint32_t)sf::Window::sWindowInstancePtr->getWidth(), (uint32_t)sf::Window::sWindowInstancePtr->getHeight() });

			swapchainViews.resize(swapchain.getImagesAmount());

			for (uint32_t i = 0; i < swapchainViews.size(); i++) {
				swapchainViews[i]
					.setCorePtr(&sf::Window::getVk())
						.create(swapchain.swapchainImages[i]);
			}

			depthImage.create({ swapchain.getExtent().width, swapchain.getExtent().height, 1 }, commandPool);

			swapchainFramebuffers.resize(swapchainViews.size());

			for (uint32_t i = 0; i < swapchainFramebuffers.size(); i++) {
				swapchainFramebuffers[i]
					.setCorePtr(&sf::Window::getVk())
					.renderPass(renderPass.getRenderPass())
					.addAttachment(swapchainViews[i].getImageView())
					.addAttachment(depthImage.getView().getImageView())
					.extent(swapchain.getExtent())
					.create();
			}
		}

		} while (false);
	}

	virtual void lateUpdate() {
		
	}

	virtual void end() {
		swapchain.destroy();

		SF_CDEBUG("Ended test scene");
	}
} G_TEST_SCENE;
