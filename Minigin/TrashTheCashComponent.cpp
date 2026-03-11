#include "TrashTheCashComponent.h"
#include <imgui.h>
#include <imgui_plot.h>
#include <chrono>
#include <iostream>

struct Transform
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
};

class GameObject3D
{
public:
	Transform transform;
	int ID{};
};

class GameObject3DAlt
{
public:
	Transform* transform;
	int ID{};
};

void dae::TrashTheCashComponent::Update(float deltaTime)
{

}

void dae::TrashTheCashComponent::Render() const
{

}

void dae::TrashTheCashComponent::RunCashePerformanceTest(TestType testType, int newBufferSize)
{
	size_t bufferSize = static_cast<size_t>(1) << newBufferSize;
	int* buffer = new int[bufferSize];

	for (size_t i = 0; i < bufferSize; ++i)
	{
		buffer[i] = static_cast<int>(i);
	}

	switch (testType) {
		case TestType::CacheTest:
			m_cacheTestResult.testName = "Cache Performance Test";
			m_cacheTestResult.steps.clear();
			m_cacheTestResult.timings.clear();
			break;
		case TestType::GameObject3DTest:
			m_cacheTestResult.testName = "GameObject3D Performance Test";
			m_cacheTestResult.steps.clear();
			m_cacheTestResult.timings.clear();
			break;
		case TestType::GameObject3DAltTest:
			m_cacheTestResult.testName = "GameObject3DAlt Performance Test";
			m_cacheTestResult.steps.clear();
			m_cacheTestResult.timings.clear();
			break;
	}

	for (int step = 1; step <= 1024; step *= 2)
	{
		auto start = std::chrono::high_resolution_clock::now();

		switch (testType) {
		case TestType::CacheTest:
			for (size_t i = 0; i < bufferSize; i += step)
			{
				buffer[i] *= 2;
			}
			break;
		case TestType::GameObject3DTest:
			for (size_t i = 0; i < bufferSize; i += step)
			{
				GameObject3D obj;
				obj.ID = buffer[i];
				obj.transform.matrix[0] *= 2;
			}
			break;
		case TestType::GameObject3DAltTest:
			for (size_t i = 0; i < bufferSize; i += step)
			{
				GameObject3DAlt obj;
				obj.ID = buffer[i];
				Transform transform;
				transform.matrix[0] *= 2;
			}
			break;
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		m_cacheTestResult.steps.push_back(step);
		m_cacheTestResult.timings.push_back(static_cast<float>(duration));

		std::cout << step << "\t" << duration << "\n";
	}

	m_cacheTestResult.hasData = true;
	delete[] buffer;
}

void RenderExercise1TestWindow()
{
	ImGui::Begin("Exersise 1", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Click button to run performance test:");
	ImGui::Separator();

	if (ImGui::Button("Run Cache Test", ImVec2(200, 30)))
	{
		CachePerformanceTest();
	}

	ImGui::Separator();

	if (g_CacheTestResult.hasData || g_GameObject3DTestResult.hasData || g_GameObject3DAltTestResult.hasData)
	{
		ImGui::Text("Test Results:");

		// Prepare data for plotting
		std::vector<float> xValues;
		std::vector<const float*> allTimings;
		std::vector<ImU32> colors;
		std::vector<std::string> legendLabels;

		if (g_CacheTestResult.hasData && !g_CacheTestResult.steps.empty())
		{
			if (xValues.empty())
			{
				for (int step : g_CacheTestResult.steps)
				{
					xValues.push_back(static_cast<float>(step));
				}
			}
			allTimings.push_back(g_CacheTestResult.timings.data());
			colors.push_back(IM_COL32(255, 0, 0, 255));
			legendLabels.push_back("Cache Test");
		}



		// Create plot configuration
		if (!xValues.empty() && !allTimings.empty())
		{
			ImGui::PlotConfig conf;
			conf.values.xs = xValues.data();
			conf.values.ys_list = allTimings.data();
			conf.values.ys_count = static_cast<int>(allTimings.size());
			conf.values.count = static_cast<int>(xValues.size());
			conf.values.colors = colors.data();
			conf.scale.min = 0;

			float maxVal = 0;
			for (const auto* timings : allTimings)
			{
				for (int i = 0; i < static_cast<int>(xValues.size()); ++i)
				{
					if (timings[i] > maxVal)
						maxVal = timings[i];
				}
			}
			conf.scale.max = maxVal * 1.1f;

			conf.tooltip.show = true;
			conf.tooltip.format = "Step %g: %.2f μs";
			conf.grid_x.show = true;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2(600, 400);
			conf.line_thickness = 2.0f;

			ImGui::Plot("Performance Comparison", conf);

			ImGui::Separator();
			for (size_t i = 0; i < legendLabels.size(); ++i)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, colors[i]);
				ImGui::Text("%s", legendLabels[i].c_str());
				ImGui::PopStyleColor();
			}
		}
	}

	ImGui::End();
}

void RenderExercise2TestWindow()
{
	ImGui::Begin("Exersise 2", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Click button to run performance test:");
	ImGui::Separator();

	if (ImGui::Button("Run GameObject3D Test", ImVec2(200, 30)))
	{
		GameObject3DPerformanceTest();
	}

	if (ImGui::Button("Run GameObject3DAlt Test", ImVec2(200, 30)))
	{
		GameObject3DAltPerformanceTest();
	}

	ImGui::Separator();

	if (g_CacheTestResult.hasData || g_GameObject3DTestResult.hasData || g_GameObject3DAltTestResult.hasData)
	{
		ImGui::Text("Test Results:");

		// Prepare data for plotting
		std::vector<float> xValues;
		std::vector<const float*> allTimings;
		std::vector<ImU32> colors;
		std::vector<std::string> legendLabels;

		if (g_GameObject3DTestResult.hasData && !g_GameObject3DTestResult.steps.empty())
		{
			if (xValues.empty())
			{
				for (int step : g_GameObject3DTestResult.steps)
				{
					xValues.push_back(static_cast<float>(step));
				}
			}
			allTimings.push_back(g_GameObject3DTestResult.timings.data());
			colors.push_back(IM_COL32(0, 255, 0, 255)); // Green
			legendLabels.push_back("GameObject3D Test");
		}

		if (g_GameObject3DAltTestResult.hasData && !g_GameObject3DAltTestResult.steps.empty())
		{
			if (xValues.empty())
			{
				for (int step : g_GameObject3DAltTestResult.steps)
				{
					xValues.push_back(static_cast<float>(step));
				}
			}
			allTimings.push_back(g_GameObject3DAltTestResult.timings.data());
			colors.push_back(IM_COL32(0, 0, 255, 255)); // Blue
			legendLabels.push_back("GameObject3DAlt Test");
		}

		// Create plot configuration
		if (!xValues.empty() && !allTimings.empty())
		{
			ImGui::PlotConfig conf;
			conf.values.xs = xValues.data();
			conf.values.ys_list = allTimings.data();
			conf.values.ys_count = static_cast<int>(allTimings.size());
			conf.values.count = static_cast<int>(xValues.size());
			conf.values.colors = colors.data();
			conf.scale.min = 0;

			float maxVal = 0;
			for (const auto* timings : allTimings)
			{
				for (int i = 0; i < static_cast<int>(xValues.size()); ++i)
				{
					if (timings[i] > maxVal)
						maxVal = timings[i];
				}
			}
			conf.scale.max = maxVal * 1.1f;

			conf.tooltip.show = true;
			conf.tooltip.format = "Step %g: %.2f μs";
			conf.grid_x.show = true;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2(600, 400);
			conf.line_thickness = 2.0f;

			ImGui::Plot("Performance Comparison", conf);

			ImGui::Separator();
			for (size_t i = 0; i < legendLabels.size(); ++i)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, colors[i]);
				ImGui::Text("%s", legendLabels[i].c_str());
				ImGui::PopStyleColor();
			}
		}
	}

	ImGui::End();
}


