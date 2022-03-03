cmake_minimum_required(VERSION 3.12)
project(vulkanLearning-1)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(. DIRSRCS)

include_directories(
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/glm-out/include
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/glfw-out/include
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/Vulkan/Include
)

link_directories(
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/glfw-out/lib
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../thirdParty/Vulkan/Lib
)

add_executable(vulkanLearning-1 ${DIRSRCS} )

target_link_libraries(vulkanLearning-1 vulkan-1.lib glfw3.lib)
