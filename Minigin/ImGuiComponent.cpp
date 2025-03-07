#include "ImGuiComponent.h"
#include "Renderer.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

dae::ImGuiComponent::ImGuiComponent(dae::GameObject& object)
	: RenderComponent(object)
{
}

void dae::ImGuiComponent::HandleExcercise()
{
	ImGui::Begin("Excercise1", nullptr, ImGuiWindowFlags_None);
	
	static int amountOfTestsEx1{ 10 };
	ImGui::InputInt("amount of tests", &amountOfTestsEx1);

	AddImguiPlotButton<int>(amountOfTestsEx1, "Trash the cache (int)");

	ImGui::End();

	ImGui::Begin("Excercise2", nullptr, ImGuiWindowFlags_None);
	
	static int amountOfTestsEx2{ 10 };
	ImGui::InputInt("amount of tests", &amountOfTestsEx2);

    ImGui::PushID(0);
	Trash::Graph gameObjectGraph = AddImguiPlotButton<Trash::GameObjectTrashCache>(amountOfTestsEx2, "Trash the cache (GameObject)");
    ImGui::PopID();
    ImGui::PushID(1);
	Trash::Graph gameObjectGraphAlt = AddImguiPlotButton<Trash::GameObjectTrashCacheAlt>(amountOfTestsEx2, "Trash the cache (GameObject alt)");
    ImGui::PopID();

    DrawMultiPlot(gameObjectGraph, gameObjectGraphAlt);

	ImGui::End();
}

void dae::ImGuiComponent::DrawMultiPlot(Trash::Graph& plot1, Trash::Graph& plot2)
{
    if (plot1.StepSizes.empty() || plot2.StepSizes.empty()) return;

    size_t dataSize = std::min(plot1.StepSizes.size(), plot2.StepSizes.size());
    if (dataSize == 0) return;

    static float xData[512];
    static float yData1[512];
    static float yData2[512];
    static const float* yData[] = { yData1, yData2 };
    static ImU32 colors[2] = { ImColor(0, 255, 0), ImColor(255, 0, 0) };

    for (size_t i = 0; i < dataSize && i < 512; ++i)
    {
        xData[i] = plot1.StepSizes[i];
        yData1[i] = plot1.Timings[i];
        yData2[i] = plot2.Timings[i];
    }

    ImGui::PlotConfig conf;
    conf.values.xs = xData;
    conf.values.ys_list = yData;
    conf.values.ys_count = 2;
    conf.values.count = static_cast<int>(dataSize);
    conf.values.colors = colors;
    conf.scale.min = 0;
    conf.scale.max = std::max(*std::max_element(plot1.Timings.begin(), plot1.Timings.end()), *std::max_element(plot2.Timings.begin(), plot2.Timings.end()));
    conf.tooltip.show = true;
    conf.tooltip.format = "x=%.0f, y=%.2f";
    conf.grid_x.show = true;
    conf.grid_y.show = true;
    conf.grid_y.size = conf.scale.max / 5;
    conf.frame_size = ImVec2(200, 100);
    conf.line_thickness = 2.f;

    ImGui::Plot("GameObject vs GameObject Alt", conf);
}

void dae::ImGuiComponent::RenderImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	ImGui::NewFrame();

	//ImGui::Begin("DemoWindow", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	//ImGui::ShowDemoWindow();
	//ImGui::End();

	HandleExcercise();


	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
