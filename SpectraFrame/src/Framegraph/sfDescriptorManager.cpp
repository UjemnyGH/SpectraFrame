#include "sfDescriptorManager.h"
#include "../Core/sfWindow.h"

bool sf::DescriptorDescription::isImage() {
	return
		descriptorType == DescriptorManagerType_Sampler ||
		descriptorType == DescriptorManagerType_CombinedSamplerImage ||
		descriptorType == DescriptorManagerType_StorageImage ||\
		descriptorType == DescriptorManagerType_SampledImage ||
		descriptorType == DescriptorManagerType_UniformTexel ||
		descriptorType == DescriptorManagerType_StorageTexel;
}

sf::DescriptorManager::~DescriptorManager() {
	destroy();
}

sf::DescriptorManager& sf::DescriptorManager::addDescriptor(const DescriptorDescription& description) {
	mResources.push_back({
		.description = description
	});

	mResourcesMap[description.name] = &(*mResources.end());
	mDescriptorTypeCount[description.descriptorType]++;

	mResourcesMap[description.name]->writeSet = {
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = VK_NULL_HANDLE,
		.dstBinding = mResourcesMap[description.name]->description.binding,
		.dstArrayElement = 0,
		.descriptorCount = 1,
		.descriptorType = (VkDescriptorType)mResourcesMap[description.name]->description.descriptorType,
		.pImageInfo = nullptr,
		.pBufferInfo = nullptr,
		.pTexelBufferView = nullptr
	};

	return *this;
}

sf::DescriptorResource* sf::DescriptorManager::getDescriptor(std::string name) {
	return mResourcesMap[name];
}

sf::DescriptorManager& sf::DescriptorManager::createDescriptors() {


	std::vector<VkWriteDescriptorSet> writeDescriptorSets;

	for (DescriptorResource& desc : mResources) {
		writeDescriptorSets.push_back(desc.writeSet);
	}

	mDescriptorSet.update(writeDescriptorSets);

	return *this;
}

void sf::DescriptorManager::destroy() {
	mDescriptorSet.destroy();
	mDescriptorPool.destroy();
	mDescriptorSetLayout.destroy();
	mDescriptorTypeCount.clear();
}