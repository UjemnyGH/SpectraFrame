#pragma once
#ifndef _SPECTRAFRAME_DESCRIPTOR_MANAGER_
#define _SPECTRAFRAME_DESCRIPTOR_MANAGER_

#include "../Core/sfVkCommon.h"
#include <variant>

namespace sf {
	enum DescriptorManagerTypeEnum {
		DescriptorManagerType_Sampler			= VK_DESCRIPTOR_TYPE_SAMPLER,
		DescriptorManagerType_CombinedSamplerImage = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		DescriptorManagerType_SampledImage		= VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
		DescriptorManagerType_StorageImage		= VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
		DescriptorManagerType_UniformTexel		= VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
		DescriptorManagerType_StorageTexel		= VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
		DescriptorManagerType_Uniform			= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		DescriptorManagerType_Storage			= VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		DescriptorManagerType_UniformDynamic	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
		DescriptorManagerType_StorageDynamic	= VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
		DescriptorManagerType_InputAttachment	= VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
	};

	typedef uint32_t DescriptorManagerType;

	struct DescriptorDescription {
		std::string name;
		DescriptorManagerType descriptorType;
		uint32_t binding;

		bool isImage();
	};

	struct DescriptorResource {
		DescriptorDescription description;

		std::variant<VkDescriptorBufferInfo, VkDescriptorImageInfo> descriptorInfo;

		VkWriteDescriptorSet writeSet;
	};
	
	class DescriptorManager {
	private:
		vkw::DescriptorPool mDescriptorPool;
		vkw::DescriptorSet mDescriptorSet;
		vkw::DescriptorSetLayout mDescriptorSetLayout;

		std::vector<DescriptorResource> mResources;
		std::unordered_map<std::string, DescriptorResource*> mResourcesMap;

		std::unordered_map<DescriptorManagerType, uint32_t> mDescriptorTypeCount;

	public:
		~DescriptorManager();

		DescriptorManager& addDescriptor(const DescriptorDescription& description);

		DescriptorResource* getDescriptor(std::string name);

		DescriptorManager& createDescriptors();

		void destroy();
	};
}

#endif