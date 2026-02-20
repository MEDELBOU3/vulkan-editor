#pragma once

#include "VulkanContext.h"
#include <vector>

class Renderer {
public:
    void init(VulkanContext* context);
    void cleanup();

    void beginFrame();
    void endFrame();

    VkRenderPass renderPass() { return _renderPass; }
    VkCommandBuffer currentCommandBuffer();

private:
    VulkanContext* _vkContext;
    VkRenderPass _renderPass;
    std::vector<VkFramebuffer> _swapChainFramebuffers;
    VkCommandPool _commandPool;
    std::vector<VkCommandBuffer> _commandBuffers;

    std::vector<VkSemaphore> _imageAvailableSemaphores;
    std::vector<VkSemaphore> _renderFinishedSemaphores;
    std::vector<VkFence> _inFlightFences;
    uint32_t _currentFrame = 0;
    uint32_t _imageIndex = 0;

    void createRenderPass();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();
};
