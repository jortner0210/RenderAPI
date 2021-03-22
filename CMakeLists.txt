cmake_minimum_required( VERSION 3.7.1 )
set (CMAKE_CXX_STANDARD 11)

project( vlayer )

# PROJECT DIRECTORIES
set(SRC_DIR ${CMAKE_SOURCE_DIR}/AR/AR)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

set(GLFW_DIR ${CMAKE_SOURCE_DIR}/lib/glfw)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# VULKAN AND GLFW
find_package(Vulkan REQUIRED FATAL_ERROR)
add_subdirectory(${GLFW_DIR})

# PROJECT SOURCE FILES
file(GLOB SOURCES ${SRC_DIR}/*.cpp)

# INCLUDE DIRECTORIES
include_directories(${CMAKE_SOURCE_DIR}/AR ${Vulkan_INCLUDE_DIRS} ${GLFW_DIR}/include)

# EXECUTABLE
add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/AR/main.cpp ${SOURCES})

# LINKAGE
target_link_libraries(
    ${PROJECT_NAME} 
    Vulkan::Vulkan
    glfw
)