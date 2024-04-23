#include "buffer.h"

namespace FF::Wrapper {
	Buffer::Ptr Buffer::createVertexBuffer(const Device::Ptr& device, VkDeviceSize size, void* pData) {
		auto buffer = Buffer::create(device, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		buffer->updateBufferByStage(pData, size);
		return buffer;
	}
	Buffer::Ptr Buffer::createIndexBuffer(const Device::Ptr& device, VkDeviceSize size, void* pData) {
		auto buffer = Buffer::create(device, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		buffer->updateBufferByStage(pData, size);
		return buffer;
	}
	Buffer::Buffer(const Device::Ptr& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
		mDevice = device;
		VkBufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = size;
		createInfo.usage = usage;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(mDevice->getDevice(), &createInfo, nullptr, &mBuffer) != VK_SUCCESS) {
			throw std::runtime_error("Error failed to create buffer");
		}

		VkMemoryRequirements memReq{};
		vkGetBufferMemoryRequirements(mDevice->getDevice(), mBuffer, &memReq);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memReq.size;
		allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, properties);

		if (vkAllocateMemory(mDevice->getDevice(), &allocInfo, nullptr, &mBufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("Error failed to allocate memory");
		}


		vkBindBufferMemory(mDevice->getDevice(), mBuffer, mBufferMemory, 0);
	}
	Buffer::~Buffer() {
		if (mBuffer != VK_NULL_HANDLE) {
			vkDestroyBuffer(mDevice->getDevice(), mBuffer, nullptr);
		}

		if (mBufferMemory != VK_NULL_HANDLE) {
			vkFreeMemory(mDevice->getDevice(), mBufferMemory, nullptr);
		}
	}
	void Buffer::updateBufferByMap(void* data, size_t size) {
		void* memPtr = nullptr;
		vkMapMemory(mDevice->getDevice(), mBufferMemory, 0, size, 0, &memPtr);
		memcpy(memPtr, data, size);
		vkUnmapMemory(mDevice->getDevice(), mBufferMemory);
	}

	void Buffer::updateBufferByStage(void* data, size_t size) {
		auto stageBuffer = Buffer::create(mDevice, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		stageBuffer->updateBufferByMap(data, size);

		copyBuffer(stageBuffer->getBuffer(), mBuffer, static_cast<VkDeviceSize>(size));
	}

	void Buffer::copyBuffer(const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, VkDeviceSize size) {
		auto commandPool = CommandPool::create(mDevice);
		auto commandBuffer = CommandBuffer::create(mDevice, commandPool);

		commandBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VkBufferCopy copyInfo{};
		copyInfo.size = size;
		commandBuffer->copyBuffer(srcBuffer, dstBuffer, 1, { copyInfo });

		commandBuffer->end();

		commandBuffer->submitSync(mDevice->getGraphicQueue(), VK_NULL_HANDLE);
	}

	uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memPros;
		vkGetPhysicalDeviceMemoryProperties(mDevice->getPhysicalDevice(), &memPros);

		for (uint32_t i = 0; i < memPros.memoryTypeCount; i++) {
			if (typeFilter & (1 << i) && ((memPros.memoryTypes[i].propertyFlags & properties) == properties)) {
				return i;
			}
		}
		throw std::runtime_error("Error: cannot find the property memory type!");
	}
}