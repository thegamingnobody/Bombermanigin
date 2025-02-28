#include "ImGuiComponent.h"
#include "Renderer.h"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>


dae::ImGuiComponent::ImGuiComponent(dae::GameObject* object)
	: RenderComponent(object)
{
}

void dae::ImGuiComponent::RenderImGui() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	ImGui::NewFrame();

	//ImGui::Begin("DemoWindow", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::ShowDemoWindow();
	//ImGui::End();

	ImGui::Begin("Excercise1", nullptr, ImGuiWindowFlags_None);
	ImGui::Button("TestButton", ImVec2(0, 0));
	ImGui::End();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
