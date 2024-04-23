#pragma once

#include "device.h"
#include "../base.h"
#include "commandBuffer.h"
#include "commandPool.h"

namespace FF::Wrapper {
	class Buffer {
	public:
		using Ptr = std::shared_ptr<Buffer>;
		static Ptr create(const Device::Ptr& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
			return std::make_shared<Buffer>(device, size, usage, properties);
		}

		static Ptr createVertexBuffer(const Device::Ptr& device, VkDeviceSize size, void* pData);

		static Ptr createIndexBuffer(const Device::Ptr& device, VkDeviceSize size, void * pData);

		Buffer(const Device::Ptr &device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

		~Buffer();

		void updateBufferByMap(void* data, size_t size);

		void updateBufferByStage(void* data, size_t size);

		void copyBuffer(const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, VkDeviceSize size);

		[[nodiscard]] auto getBuffer() const { return mBuffer; }
	private:
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	private:
		VkBuffer mBuffer{VK_NULL_HANDLE};
		VkDeviceMemory mBufferMemory{ VK_NULL_HANDLE };
		Device::Ptr mDevice{ nullptr };

	};
}