#include "sfResourceManager.h"

// All data.destroy() functions handles checking if object exist on their own
sf::ResourceManagerVertexBuffer::~ResourceManagerVertexBuffer() {
	destroy();
}

void sf::ResourceManagerVertexBuffer::destroy() {
	data.destroy();
}

sf::ResourceManagerSampler::~ResourceManagerSampler() {
	destroy();
}

void sf::ResourceManagerSampler::destroy() {
	data.destroy();
}

sf::ResourceManagerImage::~ResourceManagerImage() {
	destroy();
}

void sf::ResourceManagerImage::destroy() {
	data.destroy();
}

sf::ResourceManagerBuffer::~ResourceManagerBuffer() {
	destroy();
}

void sf::ResourceManagerBuffer::destroy() {
	data.destroy();
}

sf::ResourceManagerDataBase* sf::ResourceManagerResource::getResource() {
	return std::visit([](auto& res) -> ResourceManagerDataBase* { return &res; }, resource);
}

template<class T>
T* sf::ResourceManagerResource::getTypedResource() {
	return std::get_if<T>(&resource);
}

sf::ResourceManager::~ResourceManager() {
	// Out of scope freeing GPU memory
	freeAllResources();
}

sf::ResourceManager& sf::ResourceManager::addResource(const std::string& name, const ResourceManagerType resourceType, std::function<void(ResourceManagerResource&)> resourceCreationFunction, std::function<void(ResourceManagerResource&)> resourceFreeFunction, const uint32_t priority) {
	mResources.push_back(ResourceManagerResource{
		.description = {
			.resourceName = name,
			.resourceType = resourceType,
			.priority = priority,
			.createResource = resourceCreationFunction,
			.freeResource = resourceFreeFunction
		}
	});

	mResourcesMap[name] = &(*mResources.end());

	switch (mResourcesMap[name]->description.resourceType) {
	case ResourceManagerType_VertexBuffer:
		mResourcesMap[name]->resource = ResourceManagerVertexBuffer{};
		break;

	case ResourceManagerType_Sampler:
		mResourcesMap[name]->resource = ResourceManagerSampler{};
		break;

	case ResourceManagerType_Image:
		mResourcesMap[name]->resource = ResourceManagerImage{};
		break;

	case ResourceManagerType_Buffer:
		mResourcesMap[name]->resource = ResourceManagerBuffer{};
		break;

	default: 
		break;
	}

	return *this;
}

sf::ResourceManager& sf::ResourceManager::freeResource(const std::string& name, const ResourceManagerType resourceType) {
	if (!mResourcesMap.contains(name)) 
		return *this;

	// Free resourece in user defined way
	mResourcesMap[name]->description.freeResource(*mResourcesMap[name]);

	/*auto iter = mResourcesMap.find(name);

	if (iter->second->description.resourceType == resourceType) {
		std::visit([](auto& res) { res.destroy(); }, iter->second->resource);
		mResourcesMap.erase({ iter });
	}*/

	return *this;
}

sf::ResourceManager& sf::ResourceManager::clearResources() {
	freeAllResources();

	mResourcesMap.clear();

	return *this;
}

sf::ResourceManagerResource* sf::ResourceManager::getResource(const std::string& name) {
	if (!mResourcesMap.contains(name)) return nullptr;

	return mResourcesMap[name];
}

void sf::ResourceManager::freeAllResources() {
	for (auto& [_, res] : mResourcesMap) {
		std::visit([](auto& r) { r.destroy(); }, res->resource);
	}
}