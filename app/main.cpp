// ============================================================================
// main.cpp — window, GL context, frame loop
// ----------------------------------------------------------------------------
// Everything the user sees is drawn by Dashboard (dashboard.hpp). This file
// only opens the window and runs the loop.
// ============================================================================
#define GL_SILENCE_DEPRECATION  // macOS marks the fixed-function GL calls deprecated
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstdio>

#include "dashboard.hpp"

int main() {
    glfwSetErrorCallback([](int code, const char* msg) {
        std::fprintf(stderr, "GLFW error %d: %s\n", code, msg);
    });
    if (!glfwInit()) { std::fprintf(stderr, "glfwInit failed\n"); return 1; }

    // macOS only offers OpenGL 2.1 or a 3.2+ core profile; everywhere else
    // a 3.0 context is the most compatible choice for the ImGui GL3 backend.
#ifdef __APPLE__
    const char* glslVersion = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    const char* glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
    GLFWwindow* window = glfwCreateWindow(1280, 800, "Poker Tab", nullptr, nullptr);
    if (!window) {
        std::fprintf(stderr, "Could not create the Poker Tab window (see GLFW error above).\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;  // no imgui.ini next to the exe
    ImGui::StyleColorsLight();
    ImGui::GetStyle().FrameRounding = 3;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    pokertab::Dashboard dashboard;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(vp->WorkPos);
        ImGui::SetNextWindowSize(vp->WorkSize);
        ImGui::Begin("Poker Tab", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
        dashboard.draw(glfwGetTime());
        ImGui::End();

        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.96f, 0.96f, 0.95f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
