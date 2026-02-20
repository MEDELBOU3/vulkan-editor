#pragma once

#include "VulkanContext.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

class EditorApp {
public:
    void run();

private:
    GLFWwindow* _window;
    VulkanContext _vkContext;
    VkDescriptorPool _imguiPool;

    void initWindow();
    void initVulkan();
    void initImGui();
    void mainLoop();
    void drawFrame();
    void cleanup();

    void renderUI();
    void setupDockspace();
};
