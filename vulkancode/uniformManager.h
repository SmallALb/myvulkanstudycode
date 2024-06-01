#pragma once

#include "vulKanWrapper/buffer.h"
#include "vulKanWrapper/descriptorLayout.h"
#include "vulKanWrapper/descriptorPool.h"
#include "vulKanWrapper/descriptorSet.h"
#include "vulKanWrapper/description.h"
#include "base.h"
using namespace FF;

class UniformManager {
public: 
	using Ptr = std::shared_ptr<UniformManager>;
	static Ptr create() { return std::make_shared<UniformManager>(); }

	UniformManager();

	~UniformManager();

	void init(const Wrapper::Device::Ptr device, int frameCount);

	void update(const VPMatrices& vpMatrices, const ObjectUniform& objectUniform, const int& frameCount);

	[[nodiscard]] auto getDescriptorLayout() const { return mDescriptorSetLayout; }

	[[nodiscard]] auto getDescriptorset(int frameCount) const { return mDescriptorset->getDescriptorSet(frameCount); }

private:
	std::vector<Wrapper::UniformParamenter::Ptr> mUniformParams;
	Wrapper::DescriptorSetLayout::Ptr mDescriptorSetLayout{ nullptr };
	Wrapper::DescriptorPool::Ptr mDescriptorPool{ nullptr };
	Wrapper::DescriptorSet::Ptr mDescriptorset{ nullptr };
};