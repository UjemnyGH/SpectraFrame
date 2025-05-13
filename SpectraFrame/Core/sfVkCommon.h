#pragma once
#ifndef _SPECTRAFRAME_VULKAN_COMMON_
#define _SPECTRAFRAME_VULKAN_COMMON_

#include <vulkan/vulkan.hpp>

namespace sf {
	void commandBufferOneTimeBegin(vk::CommandBuffer& commandBuffer, vk::CommandPool& commandPool);

	void commandBufferOneTimeEnd(vk::CommandBuffer& commandBuffer, vk::CommandPool& commandPool, const vk::Queue submitQueue);

	struct CopyBuffer {
		vk::CommandBuffer* pCommandBuffer;

		CopyBuffer& setCommandBuffer(vk::CommandBuffer& commandBuffer);

		CopyBuffer& copyBufferToBuffer(vk::Buffer& src, vk::Buffer& dst, const vk::DeviceSize srcDataSize);

		CopyBuffer& copyBufferToImage(vk::Buffer& src, vk::Image& dst, const vk::Extent3D imageExtent, const vk::ImageSubresourceLayers subresources);

		/**
		 * @brief Images need to have same extent, layout and subresources to copy succesfully, otherwise use full version frtom command buffer
		 * @param src 
		 * @param dst 
		 * @param imageExtent 
		 * @param imageLayout 
		 * @param subresources 
		 * @return 
		 */
		CopyBuffer& copyImageToImage(vk::Image& src, vk::Image& dst, const vk::Extent3D imageExtent, const vk::ImageLayout imageLayout, const vk::ImageSubresourceLayers subresources);
	};

	struct TransitionImage {
		vk::CommandBuffer* pCommandBuffer;
		vk::ImageMemoryBarrier imageMemoryBarrier;

		TransitionImage();

		TransitionImage& setCommandBuffer(vk::CommandBuffer& commandBuffer);

		TransitionImage& layouts(const vk::ImageLayout oldL, const vk::ImageLayout newL);

		TransitionImage& accessMask(const vk::AccessFlags srcMask, const vk::AccessFlags dstMask);

		TransitionImage& subresources(const vk::ImageSubresourceRange subresources);

		TransitionImage& transition(vk::Image& image, const vk::PipelineStageFlags srcStageMask, const vk::PipelineStageFlags dstStageMask);
	};

	class VertexBuffer {
	private:
		vk::Buffer mBuffer;
		size_t mBufferSize;

	public:
		~VertexBuffer();

		vk::Buffer& getBuffer();

		size_t getBufferSize();

		/**
		 * @brief If vertex buffer is not created or buffer sizes does not match provided data then destroys old buffer (if exist) and creates new updated one
		 * @param commandPool 
		 * @param submitQueue 
		 * @param data 
		 * @return 
		 */
		template <typename T>
		VertexBuffer& update(vk::CommandPool& commandPool, const vk::Queue submitQueue, const std::vector<T>& data);

		void destroy();
	};

	class UniformBuffer {
	private:
		vk::Buffer mBuffer;
		size_t mBufferSize;

	public:
		~UniformBuffer();

		static vk::DescriptorType getVkType();

		vk::Buffer& getBuffer();

		size_t getBufferSize();

		UniformBuffer& create(const size_t size);

		UniformBuffer& map(void** ppDataPlace);

		UniformBuffer& unmap();

		void destroy();
	};

	class Sampler {
	private:
		vk::Image mImage;
		vk::ImageView mImageView;
		vk::Sampler mSampler;

		uint32_t mMipmapLevels;
		vk::Extent3D mImageExtent;

	public:
		~Sampler();

		static vk::DescriptorType getVkType();

		vk::Image& getImage();

		vk::ImageView& getImageView();

		vk::Sampler& getSampler();

		vk::Extent3D getImageExtent();

		uint32_t getMipmapLevels();

		Sampler& create(vk::CommandPool& commandPool, const vk::Queue submitQueue, const uint8_t* const pixels, const size_t pixelsSize, const vk::Extent3D extent, const vk::Format imageFormat, const uint32_t mipLevels);

		void destroy();
	};

	class ShaderStorageBuffer {
	private:
		vk::Buffer mBuffer;
		size_t mBufferSize;

	public:
		~ShaderStorageBuffer();

		static vk::DescriptorType getVkType();

		vk::Buffer& getBuffer();

		uint32_t getBufferSize();

		ShaderStorageBuffer& update(void* pData, const size_t size);

		ShaderStorageBuffer& map(void** ppDataPlace);

		ShaderStorageBuffer& unmap();

		void destroy();
	};

	class Descriptor {
	private:
		vk::DescriptorSetLayout mLayout;
		vk::DescriptorPool mPool;
		vk::DescriptorSet mSet;

		std::vector<vk::WriteDescriptorSet> mWriteSets;

		uint32_t mUniformBufferCount;
		uint32_t mCombinedImageSamplerCount;
		uint32_t mShaderStorageBufferCount;

	public:
		~Descriptor();

		vk::DescriptorPool& getPool();

		vk::DescriptorSet& getSet();

		vk::DescriptorSetLayout& getLayout();

		const std::vector<vk::WriteDescriptorSet>& getWriteSets();

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

		Descriptor& addUniform(UniformBuffer& uniformBuffer, const uint32_t binding, const vk::ShaderStageFlags shaderStage);
		
		Descriptor& addSampler(Sampler& sampler, const uint32_t binding, const vk::ShaderStageFlags shaderStage);

		Descriptor& addShaderStorage(ShaderStorageBuffer& shaderStorage, const uint32_t binding, const vk::ShaderStageFlags shaderStage);

		Descriptor& clear();

		Descriptor& create();

		void destroy();
	};

	class SwapchainHandler {
	private:
		vk::SwapchainKHR mSwapchain;
		std::vector<vk::ImageView> mSwapchainImageViews;
		std::vector<vk::Framebuffer> mSwapchainFramebuffers;

		vk::Image mDepthImage;
		vk::ImageView mDepthImageView;

		vk::Semaphore mImageAvailable;
		vk::Semaphore mQueueSubmitFinished;
		vk::Fence mFlight;

		uint32_t mNextImageIndex;

		bool mSkipFrame;

	public:
		~SwapchainHandler();

		vk::SwapchainKHR& getSwapchain();

		std::vector<vk::ImageView>& getImageViews();

		std::vector<vk::Framebuffer>& getFramebuffers();

		vk::Image& getDepthImage();

		vk::ImageView& getDepthImageView();

		uint32_t getNextImageIndex();

		bool shouldSkipFrame();

		vk::Semaphore& getImageAvailableSemaphore();

		vk::Semaphore& getQueueSubmitFinishedSemaphore();

		vk::Fence& getFlightFence();

		SwapchainHandler& createSwapchain(vk::CommandPool& commandPool, const vk::Queue submitQueue, const vk::RenderPass renderPass, const vk::Extent2D extent);

		SwapchainHandler& createSyncObejcts();

		SwapchainHandler& aquireNextImage(vk::CommandPool& commandPool, const vk::Queue submitQueue, const vk::RenderPass renderPass, const vk::Extent2D extent);

		SwapchainHandler& presentSwapchain(vk::CommandPool& commandPool, const vk::Queue submitQueue, const vk::RenderPass renderPass, const vk::Extent2D extent);

		void destroy();

		void destroySyncObjects();
	};

	class Shader {
	private:
		vk::ShaderModule mShaderModule;
		vk::ShaderStageFlags mShaderStage;

	public:
		~Shader();

		vk::ShaderModule& getShaderModule();

		vk::ShaderStageFlags getShaderStage();

		Shader& create(const std::vector<uint8_t>& shaderBinary, const vk::ShaderStageFlags shaderStage);

		void destroy();
	};

	class Pipeline : public vk::Pipeline {
	private:
		vk::PipelineLayout mPipelineLayout;

	public:
		~Pipeline();

		vk::PipelineLayout& getPipelineLayout();

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
		Pipeline& addDataAttribute(const uint32_t location, const vk::Format format, const uint32_t dataOffset, const uint32_t binding = 0);

		Pipeline& enableDepth(const vk::CompareOp op = vk::CompareOp::eLess, float minDepth = 0.0f, float maxDepth = 1.0f);

		Pipeline& createGraphicsPipeline(const vk::RenderPass renderPass, Descriptor& descriptor);

		void destroyAll();
	};

	class DepthImage {
	private:
		vk::Image mDepthImage;
		vk::ImageView mDepthImageView;

	public:
		~DepthImage();

		vk::Image& getImage();

		vk::ImageView& getView();

		DepthImage& create(const vk::Extent3D extent, vk::CommandPool& pool);

		void destroy();
	};
}

#endif
