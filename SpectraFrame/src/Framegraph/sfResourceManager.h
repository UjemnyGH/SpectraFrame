#pragma once
#ifndef _SPECTRAFRAME_RESOURCE_MANAGER_
#define _SPECTRAFRAME_RESOURCE_MANAGER_

#include "../Core/sfVkCommon.h"
#include <variant>
#include <functional>

namespace sf {
	// Types of available resources handled by manager
	enum ResourceManagerTypeEnum {
		ResourceManagerType_VertexBuffer = 1,
		ResourceManagerType_Sampler,
		ResourceManagerType_Image,
		ResourceManagerType_Buffer,
	};
	
	typedef uint32_t ResourceManagerType;

	// Resource manager base/parent data
	struct ResourceManagerDataBase {
		virtual void destroy() {}
	};

	// Structs for handling diffrent resources
	struct ResourceManagerVertexBuffer : public ResourceManagerDataBase {
		sf::VertexBuffer data;
		
		~ResourceManagerVertexBuffer();
		
		virtual void destroy();
	};

	struct ResourceManagerSampler : public ResourceManagerDataBase {
		sf::Sampler data;
		
		~ResourceManagerSampler();

		virtual void destroy();
	};

	struct ResourceManagerImage : public ResourceManagerDataBase {
		vkw::Image data;
		
		~ResourceManagerImage();

		virtual void destroy();
	};

	struct ResourceManagerBuffer : public ResourceManagerDataBase {
		vkw::Buffer data;
		
		~ResourceManagerBuffer();

		virtual void destroy();
	};

	struct ResourceManagerResource;

	// Description of resource
	struct ResourceManagerDescription {
		std::string resourceName;

		ResourceManagerType resourceType;

		// For later use to delete lower priority resources
		uint32_t priority;

		std::function<void(ResourceManagerResource&)> createResource;
		std::function<void(ResourceManagerResource&)> freeResource;
	};

	// Full resource data
	struct ResourceManagerResource {
		ResourceManagerDescription description;

		std::variant<ResourceManagerVertexBuffer, ResourceManagerSampler, ResourceManagerImage, ResourceManagerBuffer> resource;

		/**
		 * @brief Gets resource according to selected resource type in resource description
		 * @return pointer to resource struct (need casting to type)
		 */
		ResourceManagerDataBase* getResource();

		template<class T>
		T* getTypedResource();
	};

	class ResourceManager {
	private:
		std::vector<ResourceManagerResource> mResources;
		std::unordered_map<std::string, ResourceManagerResource*> mResourcesMap;

	public:
		~ResourceManager();

		/**
		 * @brief Adds new resource with predetermined type
		 * @param name 
		 * @param resourceType 
		 * @param resourceCreationFunction
		 * @param resourceFreeFunction
		 * @param priority
		 * @return 
		 */
		ResourceManager& addResource(const std::string& name, const ResourceManagerType resourceType, std::function<void(ResourceManagerResource&)> resourceCreationFunction, std::function<void(ResourceManagerResource&)> resourceFreeFunction, const uint32_t priority = 1);

		/**
		 * @brief Frees and removes resource selected by name and resource type
		 * @param name 
		 * @param resourceType 
		 * @return 
		 */
		ResourceManager& freeResource(const std::string& name, const ResourceManagerType resourceType);

		/**
		 * @brief Clear all resources from this resource manager
		 * @return 
		 */
		ResourceManager& clearResources();

		/**
		 * @brief Gets resource data by name
		 * @param name 
		 * @return 
		 */
		ResourceManagerResource* getResource(const std::string& name);

		/**
		 * @brief Frees all resources
		 */
		void freeAllResources();
	};
}

#endif