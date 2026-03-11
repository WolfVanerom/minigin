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

#include <filesystem>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
namespace fs = std::filesystem;


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
	constexpr size_t bufferSize = 1 << 24;
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
	constexpr size_t bufferSize = 1 << 24;

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
