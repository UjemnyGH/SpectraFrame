#pragma once
#ifndef _SPECTRAFRAME_PIPELINE_MANAGER_
#define _SPECTRAFRAME_PIPELINE_MANAGER_

#include "../Core/sfVkCommon.h"
#include <unordered_set>

namespace sf {
	struct PipelineDescription {
		std::string pipelineName;

		std::unordered_map<std::string, VkShaderStageFlags> shaders;

		std::unordered_set<std::string> pipelineReads;
		std::unordered_set<std::string> pipelineWrites;

		std::unordered_set<std::string> resourceReads;


	};

	struct PipelineResource {
		PipelineDescription description;

		vkw::Pipeline pipeline;
		vkw::PipelineLayout pipelineLayout;
	};

	class PipelineManager {
	private:
		std::vector<PipelineResource> mPipelines;
		std::unordered_map<std::string, PipelineResource*> mPipelineMap;

	private:
		~PipelineManager();


	};
}

#endif