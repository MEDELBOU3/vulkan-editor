#include "EditorApp.h"
#include "Renderer.h"
#include "Camera.h"
#include <ImGuizmo.h>
#include <stdexcept>
#include <array>
#include <glm/gtc/type_ptr.hpp>

Renderer _renderer;
Camera _camera;
glm::mat4 _modelMatrix = glm::mat4(1.0f) {
    initWindow();
    initVulkan();
    initImGui();
    mainLoop();
    cleanup();
}

void EditorApp::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    _window = glfwCreateWindow(1600, 900, "Vulkan Editor - Pro Edition", nullptr, nullptr);
}

void EditorApp::initVulkan() {
    _vkContext.init(_window);
    _renderer.init(&_vkContext);

    // Create Descriptor Pool for ImGui
    VkDescriptorPoolSize pool_sizes[] = {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;
    if (vkCreateDescriptorPool(_vkContext.device(), &pool_info, nullptr, &_imguiPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void EditorApp::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    // Setup style for "Professional" look
    auto& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0.0f;
    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);

    ImGui_ImplGlfw_InitForVulkan(_window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = _vkContext.instance();
    init_info.PhysicalDevice = _vkContext.physicalDevice();
    init_info.Device = _vkContext.device();
    init_info.QueueFamily = _vkContext.graphicsFamilyIndex();
    init_info.Queue = _vkContext.graphicsQueue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = _imguiPool;
    init_info.Subpass = 0;
    init_info.MinImageCount = 2;
    init_info.ImageCount = 2;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = nullptr;
    init_info.CheckVkResultFn = nullptr;
    
    ImGui_ImplVulkan_Init(&init_info, _renderer.renderPass());

    // Upload fonts
    VkCommandBuffer commandBuffer = _renderer.currentCommandBuffer();
    // In a real app we'd need a separate one-time command buffer here, 
    // but we'll skip font upload for now as ImGui handles it.
}

void EditorApp::mainLoop() {
    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
        drawFrame();
    }
    vkDeviceWaitIdle(_vkContext.device());
}

void EditorApp::drawFrame() {
    _renderer.beginFrame();

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    setupDockspace();
    renderUI();

    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), _renderer.currentCommandBuffer());

    _renderer.endFrame();
}

void EditorApp::setupDockspace() {
    static bool opt_fullscreen = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    if (opt_fullscreen) ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) glfwSetWindowShouldClose(_window, true);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::MenuItem("Undo");
            ImGui::MenuItem("Redo");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Create")) {
            if (ImGui::MenuItem("Cube")) { /* logic to add cube */ }
            if (ImGui::MenuItem("Sphere")) { /* logic to add sphere */ }
            if (ImGui::MenuItem("Plane")) { /* logic to add plane */ }
            if (ImGui::MenuItem("Point Light")) { /* logic to add light */ }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void EditorApp::renderUI() {
    ImGui::Begin("Hierarchy");
    if (ImGui::TreeNodeEx("Untitled Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::Selectable("Main Camera", false)) {}
        if (ImGui::Selectable("Directional Light", false)) {}
        
        static bool selected[3] = {true, false, false};
        if (ImGui::Selectable("Cube #0", selected[0])) { selected[0] = true; selected[1] = false; selected[2] = false; }
        if (ImGui::Selectable("Sphere #1", selected[1])) { selected[0] = false; selected[1] = true; selected[2] = false; }
        if (ImGui::Selectable("Plane #2", selected[2])) { selected[0] = false; selected[1] = false; selected[2] = true; }
        
        ImGui::TreePop();
    }
    ImGui::End();

    ImGui::Begin("Properties");
    ImGui::Text("Transform");
    float pos[3], rot[3], scale[3];
    ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(_modelMatrix), pos, rot, scale);
    if (ImGui::DragFloat3("Position", pos, 0.1f)) {
        ImGuizmo::RecomposeMatrixFromComponents(pos, rot, scale, glm::value_ptr(_modelMatrix));
    }
    if (ImGui::DragFloat3("Rotation", rot, 0.1f)) {
        ImGuizmo::RecomposeMatrixFromComponents(pos, rot, scale, glm::value_ptr(_modelMatrix));
    }
    if (ImGui::DragFloat3("Scale", scale, 0.1f)) {
        ImGuizmo::RecomposeMatrixFromComponents(pos, rot, scale, glm::value_ptr(_modelMatrix));
    }
    
    ImGui::Separator();
    ImGui::Text("Material");
    static float color[3] = {0.8f, 0.1f, 0.1f};
    ImGui::ColorEdit3("Base Color", color);
    ImGui::End();

    ImGui::Begin("Viewport");
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    
    // Setup camera
    _camera.setPerspective(45.0f, viewportSize.x / viewportSize.y, 0.1f, 100.0f);
    _camera.lookAt(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0,0,0), glm::vec3(0,1,0));

    ImGui::Text("3D Scene Viewport");
    
    // ImGuizmo Setup
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
    
    static ImGuizmo::OPERATION currentOp = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_W)) currentOp = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_E)) currentOp = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R)) currentOp = ImGuizmo::SCALE;

    ImGuizmo::Manipulate(glm::value_ptr(_camera.view()), glm::value_ptr(_camera.projection()), 
                        currentOp, ImGuizmo::LOCAL, glm::value_ptr(_modelMatrix));

    ImGui::End();
    
    // Status Bar
    ImGui::BeginMainMenuBar();
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Separator();
    ImGui::Text("Vulkan 1.2 | NVIDIA RTX 3080 (Simulated)");
    ImGui::EndMainMenuBar();
}

void EditorApp::cleanup() {
    vkDeviceWaitIdle(_vkContext.device());
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    _renderer.cleanup();
    vkDestroyDescriptorPool(_vkContext.device(), _imguiPool, nullptr);
    _vkContext.cleanup();
    glfwDestroyWindow(_window);
    glfwTerminate();
}
