#pragma once
#ifndef _SPECTRAFRAME_VULKAN_COMMON_
#define _SPECTRAFRAME_VULKAN_COMMON_

#include "vulkanWrap.h"

namespace sf {
	void commandBufferOneTimeBegin(vkw::CommandBuffer& commandBuffer, vkw::CommandPool& commandPool);

	void commandBufferOneTimeEnd(vkw::CommandBuffer& commandBuffer, const VkQueue submitQueue);

	struct CopyBuffer {
		vkw::CommandBuffer* pCommandBuffer;

		CopyBuffer& setCommandBuffer(vkw::CommandBuffer& commandBuffer);

		CopyBuffer& copyBufferToBuffer(vkw::Buffer& src, vkw::Buffer& dst, const VkDeviceSize srcDataSize);

		CopyBuffer& copyBufferToImage(vkw::Buffer& src, vkw::Image& dst, const VkExtent3D imageExtent, const VkImageSubresourceLayers subresources);

		/**
		 * @brief Images need to have same extent, layout and subresources to copy succesfully, otherwise use full version frtom command buffer
		 * @param src 
		 * @param dst 
		 * @param imageExtent 
		 * @param imageLayout 
		 * @param subresources 
		 * @return 
		 */
		CopyBuffer& copyImageToImage(vkw::Image& src, vkw::Image& dst, const VkExtent3D imageExtent, const VkImageLayout imageLayout, const VkImageSubresourceLayers subresources);
	};

	struct TransitionImage {
		vkw::CommandBuffer* pCommandBuffer;
		VkImageMemoryBarrier imageMemoryBarrier;

		TransitionImage();

		TransitionImage& setCommandBuffer(vkw::CommandBuffer& commandBuffer);

		TransitionImage& layouts(const VkImageLayout oldL, const VkImageLayout newL);

		TransitionImage& accessMask(const VkAccessFlags srcMask, const VkAccessFlags dstMask);

		TransitionImage& subresources(const VkImageSubresourceRange subresources);

		TransitionImage& transition(vkw::Image& image, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask);
	};

	class VertexBuffer {
	private:
		vkw::Buffer mBuffer;
		size_t mBufferSize;

	public:
		~VertexBuffer();

		vkw::Buffer& getBuffer();

		size_t getBufferSize();

		/**
		 * @brief If vertex buffer is not created or buffer sizes does not match provided data then destroys old buffer (if exist) and creates new updated one
		 * @param commandPool 
		 * @param submitQueue 
		 * @param data 
		 * @return 
		 */
		VertexBuffer& update(vkw::CommandPool& commandPool, const VkQueue submitQueue, const std::vector<float>& data);

		void destroy();
	};

	class UniformBuffer {
	private:
		vkw::Buffer mBuffer;
		size_t mBufferSize;

	public:
		~UniformBuffer();

		static VkDescriptorType getVkType();

		vkw::Buffer& getBuffer();

		size_t getBufferSize();

		UniformBuffer& create(const size_t size);

		UniformBuffer& map(void** ppDataPlace);

		UniformBuffer& unmap();

		void destroy();
	};

	class Sampler {
	private:
		vkw::Image mImage;
		vkw::ImageView mImageView;
		vkw::Sampler mSampler;

		uint32_t mMipmapLevels;
		VkExtent3D mImageExtent;

	public:
		~Sampler();

		static VkDescriptorType getVkType();

		vkw::Image& getImage();

		vkw::ImageView& getImageView();

		vkw::Sampler& getSampler();

		VkExtent3D getImageExtent();

		uint32_t getMipmapLevels();

		Sampler& create(vkw::CommandPool& commandPool, const VkQueue submitQueue, const uint8_t* const pixels, const size_t pixelsSize, const VkExtent3D extent, const VkFormat imageFormat, const uint32_t mipLevels);

		void destroy();
	};

	class ShaderStorageBuffer {
	private:
		vkw::Buffer mBuffer;
		size_t mBufferSize;

	public:
		~ShaderStorageBuffer();

		static VkDescriptorType getVkType();

		vkw::Buffer& getBuffer();

		uint32_t getBufferSize();

		ShaderStorageBuffer& update(void* pData, const size_t size);

		ShaderStorageBuffer& map(void** ppDataPlace);

		ShaderStorageBuffer& unmap();

		void destroy();
	};

	class Descriptor {
	private:
		vkw::DescriptorSetLayout mLayout;
		vkw::DescriptorPool mPool;
		vkw::DescriptorSet mSet;

		std::vector<vkw::WriteDescriptorSet> mWriteSets;

		uint32_t mUniformBufferCount;
		uint32_t mCombinedImageSamplerCount;
		uint32_t mShaderStorageBufferCount;

	public:
		~Descriptor();

		vkw::DescriptorPool& getPool();

		vkw::DescriptorSet& getSet();

		vkw::DescriptorSetLayout& getLayout();

		const std::vector<vkw::WriteDescriptorSet>& getWriteSets();

		/**
		 * @brief Gets amount of uniforms added to descriptor with addUniform() method
		 * @return 
		 */
		uint32_t getUniformBufferAmount();

		/**
		 * @brief Gets amount of combined image samplers added to descriptor with addSampler() method
		 * @return 
		 */
		uint32_t getCombinedImageSamplerAmount();

		/**
		 * @brief Gets amount of shader storage buffers added to descriptor with addShaderStorage() method
		 * @return 
		 */
		uint32_t getShaderStorageBufferAmount();

		Descriptor& addUniform(UniformBuffer& uniformBuffer, const uint32_t binding, const VkShaderStageFlags shaderStage);
		
		Descriptor& addSampler(Sampler& sampler, const uint32_t binding, const VkShaderStageFlags shaderStage);

		Descriptor& addShaderStorage(ShaderStorageBuffer& shaderStorage, const uint32_t binding, const VkShaderStageFlags shaderStage);

		Descriptor& clear();

		Descriptor& create();

		void destroy();
	};

	class SwapchainHandler {
	private:
		vkw::Swapchain mSwapchain;
		std::vector<vkw::ImageView> mSwapchainImageViews;
		std::vector<vkw::Framebuffer> mSwapchainFramebuffers;

		vkw::Image mDepthImage;
		vkw::ImageView mDepthImageView;

		vkw::Semaphore mImageAvailable;
		vkw::Semaphore mQueueSubmitFinished;
		vkw::Fence mFlight;

		uint32_t mNextImageIndex;

		bool mSkipFrame;

	public:
		~SwapchainHandler();

		vkw::Swapchain& getSwapchain();

		std::vector<vkw::ImageView>& getImageViews();

		std::vector<vkw::Framebuffer>& getFramebuffers();

		vkw::Image& getDepthImage();

		vkw::ImageView& getDepthImageView();

		uint32_t getNextImageIndex();

		bool shouldSkipFrame();

		vkw::Semaphore& getImageAvailableSemaphore();

		vkw::Semaphore& getQueueSubmitFinishedSemaphore();

		vkw::Fence& getFlightFence();

		SwapchainHandler& createSwapchain(vkw::CommandPool& commandPool, const VkQueue submitQueue, const VkRenderPass renderPass, const VkExtent2D extent);

		SwapchainHandler& createSyncObejcts();

		SwapchainHandler& aquireNextImage(vkw::CommandPool& commandPool, const VkQueue submitQueue, const VkRenderPass renderPass, const VkExtent2D extent);

		SwapchainHandler& presentSwapchain(vkw::CommandPool& commandPool, const VkQueue submitQueue, const VkRenderPass renderPass, const VkExtent2D extent);

		void destroy();

		void destroySyncObjects();
	};

	class Shader {
	private:
		vkw::ShaderModule mShaderModule;
		VkShaderStageFlags mShaderStage;

	public:
		~Shader();

		vkw::ShaderModule& getShaderModule();

		VkShaderStageFlags getShaderStage();

		Shader& create(const std::vector<uint8_t>& shaderBinary, const VkShaderStageFlags shaderStage);

		void destroy();
	};

	class Pipeline : public vkw::Pipeline {
	private:
		vkw::PipelineLayout mPipelineLayout;

	public:
		~Pipeline();

		vkw::PipelineLayout& getPipelineLayout();

		Pipeline& addShader(Shader& shader);

		/**
		 * @brief Adds binding to pipeline for describing how much data is in single vertex pass
		 * @param dataSizePerVertex amount of data per vertex in bytes (eg. struct Vertex { vec3 position; vec4 color; }  would be 7 * sizeof(float) = sizeof(Vertex))
		 * @param binding binding in shader (default = 0)
		 * @return 
		 */
		Pipeline& addDataBinding(const uint32_t dataSizePerVertex, const uint32_t binding = 0);

		/**
		 * @brief Adds attribute to bound size of data
		 * @param location location of data in shader
		 * @param binding binding in shader (default = 0)
		 * @param format data format taken directly from clear Vulkan API (use for example VK_FORMAT_R32_SFLOAT to pass single float value per vertex)
		 * @param dataOffset offset to data in all joined data (dont use more than 1 buffer per rendered data for VRAM efficiency)
		 * @return 
		 */
		Pipeline& addDataAttribute(const uint32_t location, const VkFormat format, const uint32_t dataOffset, const uint32_t binding = 0);

		Pipeline& enableDepth(const VkCompareOp op = VK_COMPARE_OP_LESS, float minDepth = 0.0f, float maxDepth = 1.0f);

		Pipeline& createGraphicsPipeline(const VkRenderPass renderPass, Descriptor& descriptor);

		void destroyAll();
	};
}

#endif