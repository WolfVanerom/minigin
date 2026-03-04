#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TextureComponent.h"
#include "RotateAroundPivotComponent.h"
#include "Scene.h"

#include <imgui.h>
#include <imgui_plot.h>

#include <filesystem>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
namespace fs = std::filesystem;

struct PerformanceTestResult
{
	std::string testName;
	std::vector<int> steps;
	std::vector<float> timings;
	bool hasData = false;
};

static PerformanceTestResult g_CacheTestResult;
static PerformanceTestResult g_GameObject3DTestResult;
static PerformanceTestResult g_GameObject3DAltTestResult;

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

static void CachePerformanceTest()
{
	constexpr size_t bufferSize = 1 << 26;
	int* buffer = new int[bufferSize];

	for (size_t i = 0; i < bufferSize; ++i)
	{
		buffer[i] = static_cast<int>(i);
	}

	g_CacheTestResult.testName = "Cache Performance Test";
	g_CacheTestResult.steps.clear();
	g_CacheTestResult.timings.clear();

	std::cout << "Cache Performance Test\n";
	std::cout << "Step\tTime (microseconds)\n";

	for (int step = 1; step <= 1024; step *= 2)
	{
		auto start = std::chrono::high_resolution_clock::now();

		for (size_t i = 0; i < bufferSize; i += step)
		{
			buffer[i] *= 2;
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		g_CacheTestResult.steps.push_back(step);
		g_CacheTestResult.timings.push_back(static_cast<float>(duration));

		std::cout << step << "\t" << duration << "\n";
	}

	g_CacheTestResult.hasData = true;
	delete[] buffer;
}

static void GameObject3DPerformanceTest()
{
	constexpr size_t bufferSize = 1 << 24; // Reduced from 1 << 26 to avoid allocation issues with larger GameObject3D
	GameObject3D* buffer = new GameObject3D[bufferSize];

	for (size_t i = 0; i < bufferSize; ++i)
	{
		buffer[i].ID = static_cast<int>(i);
	}

	g_GameObject3DTestResult.testName = "GameObject3D Performance Test";
	g_GameObject3DTestResult.steps.clear();
	g_GameObject3DTestResult.timings.clear();

	std::cout << "GameObject3D Performance Test\n";
	std::cout << "Step\tTime (microseconds)\n";

	for (int step = 1; step <= 1024; step *= 2)
	{
		auto start = std::chrono::high_resolution_clock::now();

		for (size_t i = 0; i < bufferSize; i += step)
		{
			buffer[i].ID *= 2;
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		g_GameObject3DTestResult.steps.push_back(step);
		g_GameObject3DTestResult.timings.push_back(static_cast<float>(duration));

		std::cout << step << "\t" << duration << "\n";
	}

	g_GameObject3DTestResult.hasData = true;
	delete[] buffer;
}

static void GameObject3DAltPerformanceTest()
{
	constexpr size_t bufferSize = 1 << 24; // Reduced from 1 << 26 to avoid allocation issues

	Transform* transforms = new Transform[bufferSize];
	int* IDs = new int[bufferSize];

	for (size_t i = 0; i < bufferSize; ++i)
	{
		IDs[i] = static_cast<int>(i);
	}

	g_GameObject3DAltTestResult.testName = "GameObject3DAlt Performance Test";
	g_GameObject3DAltTestResult.steps.clear();
	g_GameObject3DAltTestResult.timings.clear();

	std::cout << "GameObject3DAlt Performance Test\n";
	std::cout << "Step\tTime (microseconds)\n";

	for (int step = 1; step <= 1024; step *= 2)
	{
		auto start = std::chrono::high_resolution_clock::now();

		for (size_t i = 0; i < bufferSize; i += step)
		{
			IDs[i] *= 2;
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		g_GameObject3DAltTestResult.steps.push_back(step);
		g_GameObject3DAltTestResult.timings.push_back(static_cast<float>(duration));

		std::cout << step << "\t" << duration << "\n";
	}

	g_GameObject3DAltTestResult.hasData = true;
	delete[] transforms;
	delete[] IDs;
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

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	auto textureComponent = std::make_unique<dae::TextureComponent>(go.get());
	textureComponent->SetTexture("background.png");
	go->addComponent(std::move(textureComponent));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
	textureComponent->SetTexture("logo.png");
	go->SetPosition(358, 180);
	go->addComponent(std::move(textureComponent));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto tc = std::make_unique<dae::TextComponent>(go.get(), "Programming 4 Assignment", font.get(), SDL_Color{ 255, 255, 0, 255 });
	tc->SetPosition(292, 20);
	go->addComponent(std::move(tc));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto tcFpscounter = std::make_unique<dae::TextComponent>(go.get(), "FPS: 0", font.get(), SDL_Color{ 255, 255, 255, 255 });
	auto fpsComponent = std::make_unique<dae::FPSComponent>(go.get());
	tcFpscounter->SetPosition(10, 10);
	go->addComponent(std::move(tcFpscounter));
	go->addComponent(std::move(fpsComponent));
	scene.Add(std::move(go));

	// Test for RotateAroundPivotComponent
	/*go = std::make_unique<dae::GameObject>();
	go->SetPosition(180, 200);
	dae::GameObject* parentGo = go.get();
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
	textureComponent->SetTexture("cldig1.png");
	go->SetPosition(10, 10);
	go->addComponent(std::move(textureComponent));
	auto rapComponent = std::make_unique<dae::RotateAroundPivotComponent>(go.get(), glm::radians(360.f));
	go->addComponent(std::move(rapComponent));
	go->SetParent(parentGo, false);
	parentGo = go.get();
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
	textureComponent->SetTexture("cldig1.png");
	go->SetPosition(50, 0);
	go->SetParent(parentGo, false);
	go->addComponent(std::move(textureComponent));
	rapComponent = std::make_unique<dae::RotateAroundPivotComponent>(go.get(), glm::radians(-360.f));
	go->addComponent(std::move(rapComponent));
	scene.Add(std::move(go));*/


}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif

	dae::Minigin engine(data_location);
	engine.Run(load);
	return 0;
}
