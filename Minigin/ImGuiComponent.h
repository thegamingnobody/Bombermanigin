#ifndef IMGUICOMPONENT
#define IMGUICOMPONENT

#include "RenderComponent.h"
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <functional>
#include <future>
#include <imgui_plot.h>


namespace Trash
{
	struct TransformTrashCache
	{
		float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
		};
	};

	class GameObjectTrashCache
	{
	public:
		TransformTrashCache local;
		int id;

		GameObjectTrashCache operator*=(int value)
		{
			id *= value;
			return *this;
		}
	};

	class GameObjectTrashCacheAlt
	{
	public:
		TransformTrashCache* local;
		int id;

		GameObjectTrashCacheAlt operator*=(int value)
		{
			id *= value;
			return *this;
		}
	};

	struct Graph
	{
		std::vector<float> StepSizes{};
		std::vector<float> Timings{};
	};

	template<class T>
	Trash::Graph TrashTheCache(int const nrOfTestsPerStep)
	{
		Trash::Graph result{};

		int const arrSize{ 100'000'000 };
		std::vector<T> arr{};
		arr.resize(arrSize);

		for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
		{
			std::vector<float> currentStepTimings;

			for (int amountOfTests = 0; amountOfTests < nrOfTestsPerStep; amountOfTests++)
			{
				auto const startTime = std::chrono::high_resolution_clock::now();

				for (int i = 0; i < arrSize; i += stepSize)
				{
					arr[i] *= 2;
				}

				auto const endTime = std::chrono::high_resolution_clock::now();

				auto elapsedTime = static_cast<float>((std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime)).count());
				//std::cout << static_cast<float>((std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime)).count()) << "\n";
				currentStepTimings.push_back(elapsedTime);
			}

			std::sort(currentStepTimings.begin(), currentStepTimings.end());
			currentStepTimings.erase(currentStepTimings.begin());
			currentStepTimings.pop_back();

			auto sum = std::accumulate(currentStepTimings.begin(), currentStepTimings.end(), 0.0f);
			auto average = sum / currentStepTimings.size();

			result.StepSizes.push_back(static_cast<float>(stepSize));
			result.Timings.push_back(average);
		}

		return result;
	}

}

namespace dae
{
	class ImGuiComponent : public RenderComponent
	{
	public:
		void RenderImGui() override;

		ImGuiComponent(dae::GameObject& object);
		virtual ~ImGuiComponent() = default;
		ImGuiComponent(const ImGuiComponent& other) = delete;
		ImGuiComponent(ImGuiComponent&& other) = delete;
		ImGuiComponent& operator= (const ImGuiComponent& other) = delete;
		ImGuiComponent& operator=(ImGuiComponent&& other) = delete;

	private:
		void HandleExcercise();
		void DrawMultiPlot(Trash::Graph& plot1, Trash::Graph& plot2);

		template<class T>
		Trash::Graph AddImguiPlotButton(int numberOfTests, std::string buttonContent)
		{
			static bool isCalculating{ false };
			static bool displayGraph{ false };
			static std::future<Trash::Graph> futureTimings{};
			static Trash::Graph finishedTimings{};

			if (isCalculating)
			{
				ImGui::Text("Calculating...");

				if (futureTimings.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
				{
					finishedTimings = futureTimings.get();
					isCalculating = false;
					displayGraph = true;
				}

			}
			else if (ImGui::Button(buttonContent.c_str()))
			{
				isCalculating = true;

				futureTimings = std::async(std::launch::async, &Trash::TrashTheCache<int>, numberOfTests);
			}

			if (displayGraph)
			{
				int const valuesCount{ 11 };

				float xData[valuesCount]{};
				float yData[valuesCount]{};

				std::copy(finishedTimings.StepSizes.begin(), finishedTimings.StepSizes.end(), xData);
				std::copy(finishedTimings.Timings.begin(), finishedTimings.Timings.end(), yData);

				float maxTime{};

				for (auto& timing : finishedTimings.Timings)
				{
					if (timing > maxTime) maxTime = timing;
				}

				static ImGui::PlotConfig intPlot;
				intPlot.values.xs = xData; // this line is optional
				intPlot.values.ys = yData;
				intPlot.values.count = valuesCount;
				intPlot.scale.min = 0;
				intPlot.scale.max = maxTime;
				intPlot.tooltip.show = true;
				intPlot.tooltip.format = "x=%.0f, y=%.2f";
				intPlot.grid_x.show = true;
				intPlot.grid_y.show = true;
				intPlot.frame_size = ImVec2(200, 100);
				intPlot.line_thickness = 2.f;

				ImGui::Plot("plot", intPlot);
			}

			return finishedTimings;
		}

	};
}

#endif