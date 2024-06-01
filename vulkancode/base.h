#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <set>
#include <fstream>
#include <array>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

struct VPMatrices {
	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;

	VPMatrices() {
		mViewMatrix = glm::mat4(1.0);
		mProjectionMatrix = glm::mat4(1.0);
	}
};

struct ObjectUniform {
	glm::mat4 mModelMatrix;

	ObjectUniform() {
		mModelMatrix = glm::mat4(1.0);
	}
};