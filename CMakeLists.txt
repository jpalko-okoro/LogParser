cmake_minimum_required(VERSION 3.0.0)
project(LogParser C CXX)
message("CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

# Dear ImGui (Common Backend)
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/DearImGui)
    message(STATUS "DearImGui directory found at CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
    set(DEAR_IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/DearImGui)
    include_directories(${DEAR_IMGUI_DIR} ${DEAR_IMGUI_DIR}/backends ..)
else()
    message(FATAL_ERROR "DearImGui directory not found at: ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# GLFW (Platform Backend)
if (EXISTS ${DEAR_IMGUI_DIR}/glfw)
    message(STATUS "GLFW directory found at DEAR_IMGUI_DIR: ${DEAR_IMGUI_DIR}")
    set(GLFW_DIR ${DEAR_IMGUI_DIR}/glfw) # Set this to point to an up-to-date GLFW repo
else()
    message(FATAL_ERROR "GLFW directory not found at: ${DEAR_IMGUI_DIR}")
endif()

if (EXISTS ${GLFW_DIR}/lib-vc2019)
    message(STATUS "GLFW lib-vc2019 directory found at GLFW directory: ${GLFW_DIR}")
    find_library(GLFW_LIBRARY glfw3 PATHS ${GLFW_DIR}/lib-vc2019) 
    if(GLFW_LIBRARY)
        message("GLFW library found: ${GLFW_LIBRARY}")
    else()
        message(FATAL_ERROR "GLFW library not found at: ${GLFW_DIR}/lib-vc2019")
    endif()
else()
    message(FATAL_ERROR "GLFW library directory not found at: ${GLFW_DIR}")
endif()

option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

if (EXISTS ${GLFW_DIR}/include)
    message(STATUS "GLFW include directory found at GLFW directory: ${GLFW_DIR}")
    include_directories("${GLFW_DIR}/include")
else()
    message(FATAL_ERROR "GLFW include directory not found at: ${GLFW_DIR}")
endif()

# Libraries (Renderer Backend)
find_package(Vulkan REQUIRED)
set(LIBRARIES "${GLFW_LIBRARY};Vulkan::Vulkan")

# Use vulkan headers from glfw:
# include_directories(${GLFW_DIR}/deps)

file(GLOB sources *.cpp)

add_executable(LogParser ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${DEAR_IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${DEAR_IMGUI_DIR}/backends/imgui_impl_vulkan.cpp ${DEAR_IMGUI_DIR}/imgui.cpp ${DEAR_IMGUI_DIR}/imgui_draw.cpp ${DEAR_IMGUI_DIR}/imgui_demo.cpp ${DEAR_IMGUI_DIR}/imgui_tables.cpp ${DEAR_IMGUI_DIR}/imgui_widgets.cpp)

target_link_libraries(LogParser ${LIBRARIES})
target_link_libraries(LogParser ${GLFW})
# Maybe switch LogParser with ${CMAKE_PROJECT_NAME}??

target_compile_definitions(LogParser PUBLIC -DImTextureID=ImU64)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# If configuring with the clang-cl kit doesn't work due to linker library errors 
# then make sure to launch VSCode using Terminal and running vcvars64 first