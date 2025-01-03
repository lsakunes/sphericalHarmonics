#ifndef SIMPLE_RENDER_SYSTEM_HPP
#define SIMPLE_RENDER_SYSTEM_HPP

#include "sve_game_object.hpp"
#include "sve_pipeline.hpp"
#include "sve_device.hpp"
#include "sve_camera.hpp"
#include "sve_frame_info.hpp"

#include <memory>
#include <vector>

namespace sve {
	class SimpleRenderSystem {
	public:

		SimpleRenderSystem(SveDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;


		void renderGameObjects(FrameInfo &frameInfo, std::vector<SveGameObject> &gameObjects);

	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		SveDevice& sveDevice;
		std::unique_ptr<SvePipeline> svePipeline;
		VkPipelineLayout pipelineLayout;
	};
}

#endif