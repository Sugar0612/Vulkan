#pragma once
#include<iostream>
#include<memory>
#include<glm/glm.hpp>
#include<map>
#include<set>
#include<fstream>


#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
#include<vulkan/vulkan.h>
#include <vector>
#include <array>

// …Ë÷√ Layers..
const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#include<optional>

