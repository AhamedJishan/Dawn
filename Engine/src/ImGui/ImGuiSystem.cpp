#include "ImGuiSystem.h"

#include <Vendor/imgui/imgui.h>
#include <Vendor/imgui/imgui_impl_glfw.h>
#include <Vendor/imgui/imgui_impl_opengl3.h>
// glfw header just for glfwGetPrimaryMonitor()
#include <GLFW/glfw3.h>
#include <Core/Application.h>
#include <Core/Window.h>

namespace Dawn
{
	ImGuiSystem::ImGuiSystem()
	{
		GLFWwindow* window = reinterpret_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow());


		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();

		/* I'll check what difference it makes, then remove if unneeded
		float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(main_scale);
		style.FontScaleDpi = main_scale;*/

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	ImGuiSystem::~ImGuiSystem()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	
	void ImGuiSystem::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Temporary just to check
		ImGui::ShowDemoWindow();
	}
	
	void ImGuiSystem::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}