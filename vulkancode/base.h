#pragma once
#include <glm/glm.hpp>

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
const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};