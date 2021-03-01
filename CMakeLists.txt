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

# PROJECT SOURCE FILES
file(GLOB SOURCES ${SRC_DIR}/*.c)

# INCLUDE DIRECTORIES
include_directories(${CMAKE_SOURCE_DIR}/include ${VULKAN_DIR}/include ${GLFW_DIR}/include)

# EXECUTABLE
add_executable(${PROJECT_NAME} ${SOURCES})


# LINK VULKAN AND GLFW
#target_link_directories(
#    ${PROJECT_NAME} PRIVATE 
#    ${GLFW_DIR}/bin
#    ${VULKAN_DIR}/lib
#)

message(STATUS "Attempting auto locate Vulkan using CMake......")
find_package(Vulkan REQUIRED FATAL_ERROR)
message(STATUS "Successfully located the Vulkan SDK: ${Vulkan_INCLUDE_DIRS}")

target_link_libraries(
    ${PROJECT_NAME} 
    Vulkan::Vulkan
    "glfw" 
    ${GLFW_LIBRARIES}
    #${CMAKE_SOURCE_DIR}/lib/linux/glfw/bin/libglfw.a
    #${CMAKE_SOURCE_DIR}/lib/win/glfw/bin/glfw3dll.lib
    #C:/Users/jo3f/Repos/RenderAPI/Test/lib/win/vulkan/Lib/vulkan-1.lib
)
message("GLFW:::::::${GLFW_DIR}")
if(WIN32)
    # COPY GLFW DLL TO BUILD/BIN
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${GLFW_DIR}/bin/glfw3.dll "${CMAKE_BINARY_DIR}/bin/Debug"
        COMMENT "Copy dll file to ${CMAKE_BINARY_DIR}/bin directory" VERBATIM
    )
    
endif()