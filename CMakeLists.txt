cmake_minimum_required( VERSION 3.7.1 )
set( CMAKE_C_STANDARD 99 )

project( vlayer )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# PROJECT DIRECTORIES
set( SRC_DIR ${CMAKE_SOURCE_DIR}/src )
set( LIB_DIR ${CMAKE_SOURCE_DIR}/lib )

# PROJECT SOURCE FILES
file( GLOB SOURCES ${SRC_DIR}/*.c )

# LIBRARY DIRECTORIES
set(VULKAN_DIR ${LIB_DIR}/win/vulkan)
set(GLFW_DIR ${LIB_DIR}/win/glfw)

# INCLUDE DIRECTORIES
include_directories(${CMAKE_SOURCE_DIR}/include ${VULKAN_DIR}/Include ${GLFW_DIR}/include)

# EXE
add_executable(${PROJECT_NAME} ${SOURCES})

# LINK VULKAN AND GLFW
target_link_directories(
    ${PROJECT_NAME} PRIVATE 
    ${GLFW_DIR}/bin
    ${VULKAN_DIR}/Lib
)

target_link_libraries(
    ${PROJECT_NAME} 
    C:/Users/jo3f/Repos/RenderAPI/Test/lib/win/glfw/bin/glfw3dll.lib
    C:/Users/jo3f/Repos/RenderAPI/Test/lib/win/vulkan/Lib/vulkan-1.lib
)

# COPY GLFW DLL TO BUILD/BIN
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${GLFW_DIR}/bin/glfw3.dll "${CMAKE_BINARY_DIR}/bin/Debug"
    COMMENT "Copy dll file to ${CMAKE_BINARY_DIR}/bin directory" VERBATIM
)