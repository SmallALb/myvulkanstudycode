cmake_minimum_required(VERSION 3.8)
project(vulkanLearning-1)

set(CMAKE_CXX_STANDARD 23)

aux_source_directory(. DIRSRCS)

include_directories(
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/include/glm
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/include/glfw
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/vulkan/Include
)

link_directories(
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/lib
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/vulkan/Lib
)

add_subdirectory(vulkanWrapper)

add_subdirectory(Application)

add_executable(vulkanLearning  ${DIRSRCS})

target_link_libraries(vulkanLearning vulKanLib vulkan-1.lib glfw3.lib app)