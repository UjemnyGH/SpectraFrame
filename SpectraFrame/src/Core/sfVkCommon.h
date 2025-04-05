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

		vkw::Buffer& getBuffer();

		size_t getBufferSize();

		UniformBuffer& create(const size_t size);

		UniformBuffer& map(void** dataPlace);

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

		vkw::Image& getImage();

		vkw::ImageView& getImageView();

		vkw::Sampler& getSampler();

		VkExtent3D getImageExtent();

		uint32_t getMipmapLevels();

		Sampler& create(const uint8_t* const pixels, const size_t pixelsSize, const VkExtent3D extent, const VkFormat imageFormat, const uint32_t mipLevels);

		void destroy();
	};

	class Descriptor {
	private:
		vkw::DescriptorPool mPool;
		vkw::DescriptorSet mSet;

		std::vector<vkw::WriteDescriptorSet> mWriteSets;

		uint32_t mUniformBufferCount;
		uint32_t mCombinedImageSamplerCount;

	public:
		~Descriptor();

		vkw::DescriptorPool& getPool();

		vkw::DescriptorSet& getSet();

		const std::vector<vkw::WriteDescriptorSet>& getWriteSets();

		Descriptor& addUniform(UniformBuffer& uniformBuffer, const uint32_t binding, const VkShaderStageFlags shaderStage, const uint32_t count = 1);
		
		//Descriptor& addSampler();

		Descriptor& clear();

		Descriptor& create(vkw::DescriptorSetLayout& setLayout);

		void destroy();
	};
}

#endif