cmake_minimum_required(VERSION 3.0.0)
project(LogParser C CXX)
message("CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

# Dear ImGui (Common Backend)
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/DearImGui)
    set(DEAR_IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/DearImGui)
    message(STATUS "DearImGui directory found at: ${DEAR_IMGUI_DIR}")
    include_directories(${DEAR_IMGUI_DIR})
    include_directories(${DEAR_IMGUI_DIR}/backends)
else()
    message(FATAL_ERROR "DearImGui directory not found at: ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# GLFW (Platform Backend)
if (EXISTS ../glfw_repo)
    set(GLFW_DIR ../glfw_repo) # Set this to point to an up-to-date GLFW repo
    message(STATUS "GLFW directory found at: ${GLFW_DIR}")
else()
    message(FATAL_ERROR "GLFW directory not found at: ../glfw_repo")
endif()

option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)

# Libraries (Renderer Backend)
find_package(Vulkan REQUIRED)

# Use vulkan headers from glfw:
include_directories(${GLFW_DIR}/deps)

file(GLOB sources *.cpp *.h)

add_executable(LogParser
    ${sources}
)

target_link_libraries(LogParser "glfw")
target_link_libraries(LogParser "Vulkan::Vulkan")

target_include_directories(LogParser PUBLIC ${DEAR_IMGUI_DIR})

target_compile_definitions(LogParser PUBLIC -DImTextureID=ImU64)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# If configuring with the clang-cl kit doesn't work due to linker library errors 
# then make sure to launch VSCode using Terminal and running vcvars64 first