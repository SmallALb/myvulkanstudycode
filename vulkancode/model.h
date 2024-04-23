#pragma once
#include "base.h"
#include "vulKanWrapper/buffer.h"
#include "vulKanWrapper/device.h"
namespace FF {
	struct Vertex {
		glm::vec3 mPostion;
		glm::vec3 mColor;
	};

	class Model {
	public:
		using Ptr = std::shared_ptr<Model>;
		
		static Ptr create(const Wrapper::Device::Ptr& device) {
			return std::make_shared<Model>(device);
		}

		Model(const Wrapper::Device::Ptr& device) {
			//mDatas = {
			//	{{0.0f, -0.2f, 0.0f},{1.0f, 0.0f, 0.0f}},
			//	{{0.5f, 0.5f, 0.0f },{0.0f, 1.0f, 0.0f}},
			//	{{-0.5f, 0.5f, 0.0f},{0.0f, 0.0f, 1.0f}},
			//};

			mPosition = {
				0.0f, -0.8, 0.0f,
				0.5f, 0.5f, 0.0f,
				-0.5f, 0.5f, 0.0f,
			};

			mColors = {
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
			};
			mIndexDatas = {
				0, 1, 2
			};

			mPositionBuffer = Wrapper::Buffer::createVertexBuffer(device, mPosition.size() * sizeof(float), mPosition.data());

			mColorBuffer = Wrapper::Buffer::createVertexBuffer(device, mColors.size() * sizeof(float), mColors.data());

			mIndexBuffer = Wrapper::Buffer::createIndexBuffer(device, mIndexDatas.size() * sizeof(unsigned int), mIndexDatas.data());
		}

		~Model() {
			
		}

		std::vector<VkVertexInputBindingDescription> getVertexInputBindingDescription() {
			std::vector<VkVertexInputBindingDescription> bindingDes{};
			bindingDes.resize(2);

			bindingDes[0].binding = 0;
			bindingDes[0].stride = sizeof(float) * 3;
			bindingDes[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			bindingDes[1].binding = 1;
			bindingDes[1].stride = sizeof(float) * 3;
			bindingDes[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDes;
		}

		std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(){
			std::vector<VkVertexInputAttributeDescription> attributeDes{};
			attributeDes.resize(2);

			attributeDes[0].binding = 0;
			attributeDes[0].location = 0;
			attributeDes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDes[0].offset = 0;

			attributeDes[1].binding = 1;
			attributeDes[1].location = 1;
			attributeDes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDes[1].offset = 0;

			return attributeDes;
		}

		void chace() {
			mPosition[1] += 0.1;
		}

		[[nodiscard]] auto getVertexBuffers() const { 
			return std::vector<VkBuffer>{mPositionBuffer->getBuffer(), mColorBuffer->getBuffer()}; 
		}

		[[nodiscard]] auto getIndexBuffer() const { return mIndexBuffer; }

		[[nodiscard]] auto getIndexCount() const { return mIndexDatas.size(); }
	private:
		//std::vector<Vertex> mDatas{}; 
		std::vector<float> mPosition{};
		std::vector<float> mColors{};
		std::vector<unsigned int> mIndexDatas{};
		float h = -0.8f;
		float angle = 0.0f;
		Wrapper::Buffer::Ptr mPositionBuffer{ nullptr };
		Wrapper::Buffer::Ptr mColorBuffer{ nullptr };
		Wrapper::Buffer::Ptr mIndexBuffer{ nullptr };
	};
}