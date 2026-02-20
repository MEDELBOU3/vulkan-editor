#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanContext {
public:
    void init(GLFWwindow* window);
    void cleanup();

    VkInstance instance() { return _instance; }
    VkDevice device() { return _device; }
    VkPhysicalDevice physicalDevice() { return _physicalDevice; }
    VkQueue graphicsQueue() { return _graphicsQueue; }
    VkQueue presentQueue() { return _presentQueue; }
    VkSurfaceKHR surface() { return _surface; }
    VkSwapchainKHR swapChain() { return _swapChain; }
    VkExtent2D swapChainExtent() { return _swapChainExtent; }
    VkFormat swapChainImageFormat() { return _swapChainImageFormat; }
    std::vector<VkImageView> swapChainImageViews() { return _swapChainImageViews; }
    uint32_t graphicsFamilyIndex() { return findQueueFamilies(_physicalDevice).graphicsFamily.value(); }

    void recreateSwapChain(GLFWwindow* window);

private:
    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debugMessenger;
    VkSurfaceKHR _surface;

    VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
    VkDevice _device;

    VkQueue _graphicsQueue;
    VkQueue _presentQueue;

    VkSwapchainKHR _swapChain;
    std::vector<VkImage> _swapChainImages;
    VkFormat _swapChainImageFormat;
    VkExtent2D _swapChainExtent;
    std::vector<VkImageView> _swapChainImageViews;

    void createInstance();
    void setupDebugMessenger();
    void createSurface(GLFWwindow* window);
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain(GLFWwindow* window);
    void createImageViews();

    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
};
