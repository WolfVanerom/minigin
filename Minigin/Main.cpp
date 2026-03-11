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
#include <TrashTheCashComponent.h>
#include "InputManager.h"
#ifdef _WIN32
#include <xinput.h>
#endif
namespace fs = std::filesystem;

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

	// Test for Command
	go = std::make_unique<dae::GameObject>();
	go->SetPosition(180, 200);
	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
	textureComponent->SetTexture("cldig1.png");
	dae::InputManager::GetInstance().AddCommand(SDLK_W, std::make_unique<dae::moveCommand>(go.get(), 0.0f, -10.0f));
	dae::InputManager::GetInstance().AddCommand(SDLK_S, std::make_unique<dae::moveCommand>(go.get(), 0.0f, 10.0f));
	dae::InputManager::GetInstance().AddCommand(SDLK_A, std::make_unique<dae::moveCommand>(go.get(), -10.0f, 0.0f));
	dae::InputManager::GetInstance().AddCommand(SDLK_D, std::make_unique<dae::moveCommand>(go.get(), 10.0f, 0.0f));
	go->addComponent(std::move(textureComponent));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->SetPosition(120, 200);
	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
	textureComponent->SetTexture("cldig1.png");
	dae::InputManager::GetInstance().AddCommand(0x0001, std::make_unique<dae::moveCommand>(go.get(), 0.0f, -20.0f));
	dae::InputManager::GetInstance().AddCommand(0x0002, std::make_unique<dae::moveCommand>(go.get(), 0.0f, 20.0f));
	dae::InputManager::GetInstance().AddCommand(0x0004, std::make_unique<dae::moveCommand>(go.get(), -20.0f, 0.0f));
	dae::InputManager::GetInstance().AddCommand(0x0008, std::make_unique<dae::moveCommand>(go.get(), 20.0f, 0.0f));
	go->addComponent(std::move(textureComponent));
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
