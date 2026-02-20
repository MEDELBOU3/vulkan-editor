cmake_minimum_required(VERSION 3.20)
project(VulkanEditor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Vulkan
find_package(Vulkan)

if(NOT Vulkan_FOUND)
    # Manual search as fallback for MSYS2
    set(Vulkan_INCLUDE_DIR "C:/msys64/mingw64/include" CACHE PATH "")
    set(Vulkan_LIBRARY "C:/msys64/mingw64/lib/libvulkan.dll.a" CACHE FILEPATH "")
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(Vulkan DEFAULT_MSG Vulkan_LIBRARY Vulkan_INCLUDE_DIR)
endif()

# Dependencies via FetchContent
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/refs/tags/3.3.8.zip
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.zip
)
FetchContent_MakeAvailable(glm)

# ImGui
FetchContent_Declare(
    imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.89.9.zip
)
FetchContent_MakeAvailable(imgui)

# ImGuizmo
FetchContent_Declare(
    imguizmo
    URL https://github.com/CedricGuillemet/ImGuizmo/archive/master.zip
)
FetchContent_MakeAvailable(imguizmo)

# STB
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# Source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE INC_FILES include/*.h)

# Add executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${INC_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${Vulkan_INCLUDE_DIRS}
    include
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${imguizmo_SOURCE_DIR}
    ${stb_SOURCE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Vulkan::Vulkan
    glfw
    glm::glm
)

# Special handling for ImGui backends (we'll copy/compile them manually in our project)
# We will create a small object library for ImGui
add_library(imgui_lib OBJECT 
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
    ${imguizmo_SOURCE_DIR}/ImGuizmo.cpp
)
target_include_directories(imgui_lib PRIVATE 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${Vulkan_INCLUDE_DIRS}
    ${glfw_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE imgui_lib)

# Copy shaders to build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
