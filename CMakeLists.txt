cmake_minimum_required( VERSION 3.7.1 )
set( CMAKE_C_STANDARD 99 )

project( vlayer )

# PROJECT DIRECTORIES
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

if(WIN32)
    message("Platform Detected: WINDOWS")
    set(VULKAN_DIR ${LIB_DIR}/win/vulkan)
    set(GLFW_DIR ${LIB_DIR}/win/glfw)

elseif(UNIX)
    message("Platform Detected: LINUX")
    set(VULKAN_DIR ${LIB_DIR}/linux/vulkan)
    set(GLFW_DIR ${LIB_DIR}/linux/glfw)

endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# PROJECT SOURCE FILES
file(GLOB SOURCES ${SRC_DIR}/*.c)

# INCLUDE DIRECTORIES
include_directories(${CMAKE_SOURCE_DIR}/include ${VULKAN_DIR}/include ${GLFW_DIR}/include)

# EXECUTABLE
add_executable(${PROJECT_NAME} ${SOURCES})

message(STATUS "Attempting auto locate Vulkan using CMake......")
find_package(Vulkan REQUIRED FATAL_ERROR)
message(STATUS "Successfully located the Vulkan SDK: ${Vulkan_INCLUDE_DIRS}")

if(WIN32)
    
    target_link_libraries(
        ${PROJECT_NAME} 
        Vulkan::Vulkan
        ${CMAKE_SOURCE_DIR}/lib/win/glfw/bin/glfw3dll.lib
    )

    # COPY GLFW DLL TO BUILD/BIN
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy C:/Users/jo3f/Repos/RenderAPI/lib/win/glfw/bin/glfw3.dll ${CMAKE_SOURCE_DIR}/build/bin/Debug/.
        COMMENT "Copy dll file to ${CMAKE_BINARY_DIR}/bin directory" VERBATIM
    )
    
elseif(UNIX)

    target_link_libraries(
        ${PROJECT_NAME} 
        Vulkan::Vulkan
        "glfw" 
        ${GLFW_LIBRARIES}
    )

endif()