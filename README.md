# Vulkan 3D Editor Pro

A professional 3D editor built with C++20 and Vulkan.

## Features
- **Professional UI**: Powered by Dear ImGui with docking support.
- **Scene Management**: Hierarchy tree for object selection and management.
- **Transform Gizmos**: Industry-standard Translate/Rotate/Scale gizmos via ImGuizmo.
- **Vulkan Renderer**: Clean, modern Vulkan implementation with double buffering and proper synchronization.
- **Shaders**: Support for lighting and basic materials.
- **Camera**: Perspective camera for 3D navigation.

## Prerequisites
- **Vulkan SDK**: [Download here](https://vulkan.lunarg.com/sdk/home)
- **CMake**: Version 3.20+
- **C++ Compiler**: Support for C++20 (MSVC 2019+, GCC 10+, Clang 10+)

## Building
Run the following commands in the project root:

```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Controls
- **Docking**: Drag windows by their titles to dock/undock.
- **Gizmos**:
  - `W`: Translate mode
  - `E`: Rotate mode
  - `R`: Scale mode
- **Hierarchy**: Select objects to view their properties.
- **Properties**: Use the sliders or gizmos to change object transforms.

## Project Structure
- `src/`: C++ implementation files.
- `include/`: Header files.
- `shaders/`: GLSL shader source code.
- `CMakeLists.txt`: Project configuration and dependency management.
