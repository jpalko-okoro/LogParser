# If configuring with the clang-cl kit doesn't work due to linker library errors 
# then make sure to launch VSCode using Terminal and running vcvars64 first

cmake_minimum_required(VERSION 3.0.0)
project(LogParser VERSION 0.1.0)
message(STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")

# GLFW (Platform Backend)
    set(GLFW_PATH "${CMAKE_CURRENT_LIST_DIR}/glfw")

    # Configure messaging for debugging
    if (EXISTS ${GLFW_PATH})
        message(STATUS "GLFW path found at: ${GLFW_PATH}")
    else()
        message(STATUS "GLFW path not found at: ${GLFW_PATH}")
    endif()

    option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
    option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
    option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
    option(GLFW_INSTALL "Generate installation target" OFF)
    option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

    add_subdirectory(${GLFW_PATH} binary_dir EXCLUDE_FROM_ALL)
    include_directories(${GLFW_PATH}/include)

# Vulkan (Renderer Backend)
    find_package(Vulkan REQUIRED)

# Dear ImGui (Common Backend)
    # Set the Dear ImGui main path
    set(IMGUI_PATH "${CMAKE_CURRENT_LIST_DIR}/imgui")
    
    # Configure messaging for debugging
    if (EXISTS ${IMGUI_PATH})
        message(STATUS "Dear ImGui path found at: ${IMGUI_PATH}")
    else()
        message(STATUS "Dear ImGui path not found at: ${IMGUI_PATH}")
    endif()
    
    # Set the Dear ImGui backends path
    set(IMGUI_BACKENDS_PATH "${CMAKE_CURRENT_LIST_DIR}/imgui/backends")

    # Configure messaging for debugging
    if (EXISTS ${IMGUI_BACKENDS_PATH})
        message(STATUS "Dear ImGui backends path found at: ${IMGUI_BACKENDS_PATH}")
    else()
        message(STATUS "Dear ImGui backends path not found at: ${IMGUI_BACKENDS_PATH}")
    endif()

    # Specifying all the source files to include
    file(GLOB IMGUI_SOURCES 
        # General ImGui files
        ${IMGUI_PATH}/imgui.h
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imconfig.h
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_internal.h
        ${IMGUI_PATH}/imstb_rectpack.h
        ${IMGUI_PATH}/imstb_textedit.h
        ${IMGUI_PATH}/imstb_truetype.h
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp

        # Renderer specific ImGui files (made for GLFW and Vulkan)
        ${IMGUI_BACKENDS_PATH}/imgui_impl_glfw.h
        ${IMGUI_BACKENDS_PATH}/imgui_impl_glfw.cpp
        ${IMGUI_BACKENDS_PATH}/imgui_impl_vulkan.h
        ${IMGUI_BACKENDS_PATH}/imgui_impl_vulkan.cpp
    )

    # Including Dear ImGui library
    add_library("imgui" STATIC ${IMGUI_SOURCES})
    target_include_directories("imgui" PUBLIC ${IMGUI_PATH})
    target_include_directories("imgui" PUBLIC ${IMGUI_BACKENDS_PATH})

# Non-executable library linking
    # Linking DearImGui and other libraries --> Not positive this worked for GLFW and Vulkan
    target_link_libraries("imgui" PRIVATE "glfw")
    target_link_libraries("imgui" PRIVATE "Vulkan::Vulkan")

    # Use Vulkan headers from GLFW:
    include_directories(${GLFW_DIR}/deps)

# Configuring the executable
    set(PROJECT_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/src")

    # Configure messaging for debugging
    if (EXISTS ${PROJECT_SOURCE_PATH})
        message(STATUS "Project source file path found at: ${PROJECT_SOURCE_PATH}")
    else()
        message(STATUS "Project source file path not found at: ${PROJECT_SOURCE_PATH}")
    endif()

    set(PROJECT_INCLUDE_PATH "${CMAKE_CURRENT_LIST_DIR}/include")

    if (EXISTS ${PROJECT_INCLUDE_PATH})
        message(STATUS "Project include file path found at: ${PROJECT_INCLUDE_PATH}")
    else()
        message(STATUS "Project include file path not found at: ${PROJECT_INCLUDE_PATH}")
    endif()

    # Specifying all the source files to include (Specify individual files if needed)
    file(GLOB PROJECT_SOURCES 
        ${PROJECT_SOURCE_PATH}/*.cpp
        ${PROJECT_INCLUDE_PATH}/*.h
    )

    # Adding all the project sources to the project's executable
    add_executable(${CMAKE_PROJECT_NAME}
        ${PROJECT_SOURCES}
    )

# Executable library linking and directory includes 
    # Quotes for GLFW and Vulkan? --> Not positive this worked for GLFW and Vulkan
    target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC "glfw")
    target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC "Vulkan::Vulkan")
    target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC "imgui")

    # Checking for valid args? --> Not positive this worked for GLFW and Vulkan
    target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "glfw")
    target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "Vulkan::Vulkan")
    target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "imgui")

# Dear ImGui specific pre-processor macro definition included in GLFW-Vulkan example
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC -DImTextureID=ImU64)

# Setting variables for future packaging of the project using CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})