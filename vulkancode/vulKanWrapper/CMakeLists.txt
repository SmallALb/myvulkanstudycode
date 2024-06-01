file(GLOB_RECURSE VULKAN ./ *.cpp)

add_library(vulKanLib ${VULKAN} "descriptorLayout.cpp")