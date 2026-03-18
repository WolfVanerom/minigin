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
#include "TrashTheCashComponent.h"
#include "InputManager.h"
#include "LevelManager.h"
#include "PlayerComponent.h"
#include "Observer.h"
namespace fs = std::filesystem;


static void load() {
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto levelManager = &dae::LevelManager::GetInstance();

    #if __EMSCRIPTEN__
	levelManager->LoadLevel("levelData/1.txt", &scene);
	#else
	levelManager->LoadLevel("Data/levelData/1.txt", &scene);
	#endif

	//player 1

	auto go = std::make_unique<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textComponent = std::make_unique<dae::TextComponent>(go.get(), "#Lives = 5", font.get(), SDL_Color{ 255, 255, 255, 255 });
	textComponent->SetPosition(0, 0);
	auto* livesTextComponentPtr = textComponent.get();
	go->addComponent(std::move(textComponent));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	textComponent = std::make_unique<dae::TextComponent>(go.get(), "#Score = 0", font.get(), SDL_Color{ 255, 255, 255, 255 });
	textComponent->SetPosition(0, 50);
	auto* scoreTextComponentPtr = textComponent.get();
	go->addComponent(std::move(textComponent));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->SetPosition(180, 200);
	auto textureComponent = std::make_unique<dae::TextureComponent>(go.get());
	textureComponent->SetTexture("cldig1.png");
	dae::InputManager::GetInstance().AddCommand(SDLK_W, std::make_unique<dae::moveCommand>(go.get(), 0.0f, -10.0f));
	dae::InputManager::GetInstance().AddCommand(SDLK_S, std::make_unique<dae::moveCommand>(go.get(), 0.0f, 10.0f));
	dae::InputManager::GetInstance().AddCommand(SDLK_A, std::make_unique<dae::moveCommand>(go.get(), -10.0f, 0.0f));
	dae::InputManager::GetInstance().AddCommand(SDLK_D, std::make_unique<dae::moveCommand>(go.get(), 10.0f, 0.0f));
	go->addComponent(std::move(textureComponent));
	auto playerComponent = std::make_unique<dae::PlayerComponent>(go.get());
	auto* playerComponentPtr = playerComponent.get();
	dae::InputManager::GetInstance().AddCommand(SDLK_SPACE, std::make_unique<dae::damageCommand>(go.get(), playerComponentPtr));
	go->addComponent(std::move(playerComponent));
	scene.Add(std::move(go));

	static std::unique_ptr<dae::remainingLivesObserver> livesObserver{};
	livesObserver = std::make_unique<dae::remainingLivesObserver>(livesTextComponentPtr, playerComponentPtr);
	playerComponentPtr->AddLivesObserver(livesObserver.get());

	static std::unique_ptr<dae::scoreObserver> scoreObserver{};
	scoreObserver = std::make_unique<dae::scoreObserver>(scoreTextComponentPtr, playerComponentPtr);
	playerComponentPtr->AddScoreObserver(scoreObserver.get());

	//player 2

	go = std::make_unique<dae::GameObject>();
	textComponent = std::make_unique<dae::TextComponent>(go.get(), "#Lives = 5", font.get(), SDL_Color{ 255, 255, 255, 255 });
	textComponent->SetPosition(0, 100);
	livesTextComponentPtr = textComponent.get();
	go->addComponent(std::move(textComponent));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	textComponent = std::make_unique<dae::TextComponent>(go.get(), "#Score = 0", font.get(), SDL_Color{ 255, 255, 255, 255 });
	textComponent->SetPosition(0, 150);
	scoreTextComponentPtr = textComponent.get();
	go->addComponent(std::move(textComponent));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->SetPosition(220, 200);
	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
	textureComponent->SetTexture("cldig1.png");
	dae::InputManager::GetInstance().AddCommand(SDLK_UP, std::make_unique<dae::moveCommand>(go.get(), 0.0f, -10.0f));
	dae::InputManager::GetInstance().AddCommand(SDLK_DOWN, std::make_unique<dae::moveCommand>(go.get(), 0.0f, 10.0f));
	dae::InputManager::GetInstance().AddCommand(SDLK_LEFT, std::make_unique<dae::moveCommand>(go.get(), -10.0f, 0.0f));
	dae::InputManager::GetInstance().AddCommand(SDLK_RIGHT, std::make_unique<dae::moveCommand>(go.get(), 10.0f, 0.0f));
	go->addComponent(std::move(textureComponent));
	playerComponent = std::make_unique<dae::PlayerComponent>(go.get());
	playerComponentPtr = playerComponent.get();
	dae::InputManager::GetInstance().AddCommand(SDLK_RSHIFT, std::make_unique<dae::damageCommand>(go.get(), playerComponentPtr));
	go->addComponent(std::move(playerComponent));
	scene.Add(std::move(go));

	static std::unique_ptr<dae::remainingLivesObserver> livesObserver1{};
	livesObserver1 = std::make_unique<dae::remainingLivesObserver>(livesTextComponentPtr, playerComponentPtr);
	playerComponentPtr->AddLivesObserver(livesObserver1.get());

	static std::unique_ptr<dae::scoreObserver> scoreObserver1{};
	scoreObserver1 = std::make_unique<dae::scoreObserver>(scoreTextComponentPtr, playerComponentPtr);
	playerComponentPtr->AddScoreObserver(scoreObserver1.get());

	//fps
	go = std::make_unique<dae::GameObject>();
	auto tcFpscounter = std::make_unique<dae::TextComponent>(go.get(), "FPS: 0", font.get(), SDL_Color{ 255, 255, 255, 255 });
	auto fpsComponent = std::make_unique<dae::FPSComponent>(go.get());
	tcFpscounter->SetPosition(900, 10);
	go->addComponent(std::move(tcFpscounter));
	go->addComponent(std::move(fpsComponent));
	scene.Add(std::move(go));

	//explanation
	go = std::make_unique<dae::GameObject>();
	textComponent = std::make_unique<dae::TextComponent>(go.get(), "Use wasd and arrows to move.", font.get(), SDL_Color{ 255, 255, 255, 255 });
	textComponent->SetPosition(0, 200);
	textComponent->SetScale(0.5f);
	go->addComponent(std::move(textComponent));
	textComponent = std::make_unique<dae::TextComponent>(go.get(), "Use space and Rshift to damage", font.get(), SDL_Color{ 255, 255, 255, 255 });
	textComponent->SetPosition(0, 230);
	textComponent->SetScale(0.5f);
	go->addComponent(std::move(textComponent));
	scene.Add(std::move(go));
}


//static void load()
//{
//	auto& scene = dae::SceneManager::GetInstance().CreateScene();
//
//
//	auto go = std::make_unique<dae::GameObject>();
//	auto textureComponent = std::make_unique<dae::TextureComponent>(go.get());
//	textureComponent->SetTexture("background.png");
//	go->addComponent(std::move(textureComponent));
//	scene.Add(std::move(go));
//
//	go = std::make_unique<dae::GameObject>();
//	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
//	textureComponent->SetTexture("logo.png");
//	go->SetPosition(358, 180);
//	go->addComponent(std::move(textureComponent));
//	scene.Add(std::move(go));
//
//	go = std::make_unique<dae::GameObject>();
//	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
//	auto tc = std::make_unique<dae::TextComponent>(go.get(), "Programming 4 Assignment", font.get(), SDL_Color{ 255, 255, 0, 255 });
//	tc->SetPosition(292, 20);
//	go->addComponent(std::move(tc));
//	scene.Add(std::move(go));
//
//	go = std::make_unique<dae::GameObject>();
//	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
//	auto tcFpscounter = std::make_unique<dae::TextComponent>(go.get(), "FPS: 0", font.get(), SDL_Color{ 255, 255, 255, 255 });
//	auto fpsComponent = std::make_unique<dae::FPSComponent>(go.get());
//	tcFpscounter->SetPosition(10, 10);
//	go->addComponent(std::move(tcFpscounter));
//	go->addComponent(std::move(fpsComponent));
//	scene.Add(std::move(go));
//
//	 Test for Command
//	go = std::make_unique<dae::GameObject>();
//	go->SetPosition(180, 200);
//	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
//	textureComponent->SetTexture("cldig1.png");
//	dae::InputManager::GetInstance().AddCommand(SDLK_W, std::make_unique<dae::moveCommand>(go.get(), 0.0f, -10.0f));
//	dae::InputManager::GetInstance().AddCommand(SDLK_S, std::make_unique<dae::moveCommand>(go.get(), 0.0f, 10.0f));
//	dae::InputManager::GetInstance().AddCommand(SDLK_A, std::make_unique<dae::moveCommand>(go.get(), -10.0f, 0.0f));
//	dae::InputManager::GetInstance().AddCommand(SDLK_D, std::make_unique<dae::moveCommand>(go.get(), 10.0f, 0.0f));
//	go->addComponent(std::move(textureComponent));
//	scene.Add(std::move(go));
//
//	 Test for Command with controller input (D-Pad)
//	go = std::make_unique<dae::GameObject>();
//	go->SetPosition(120, 200);
//	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
//	textureComponent->SetTexture("cldig1.png");
//	dae::InputManager::GetInstance().AddCommand(0x0001, std::make_unique<dae::moveCommand>(go.get(), 0.0f, -20.0f));
//	dae::InputManager::GetInstance().AddCommand(0x0002, std::make_unique<dae::moveCommand>(go.get(), 0.0f, 20.0f));
//	dae::InputManager::GetInstance().AddCommand(0x0004, std::make_unique<dae::moveCommand>(go.get(), -20.0f, 0.0f));
//	dae::InputManager::GetInstance().AddCommand(0x0008, std::make_unique<dae::moveCommand>(go.get(), 20.0f, 0.0f));
//	go->addComponent(std::move(textureComponent));
//	scene.Add(std::move(go));*/
//
//	Test for RotateAroundPivotComponent
//	go = std::make_unique<dae::GameObject>();
//	go->SetPosition(180, 200);
//	dae::GameObject* parentGo = go.get();
//	scene.Add(std::move(go));
//
//	go = std::make_unique<dae::GameObject>();
//	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
//	textureComponent->SetTexture("cldig1.png");
//	go->SetPosition(10, 10);
//	go->addComponent(std::move(textureComponent));
//	auto rapComponent = std::make_unique<dae::RotateAroundPivotComponent>(go.get(), glm::radians(360.f));
//	go->addComponent(std::move(rapComponent));
//	go->SetParent(parentGo, false);
//	parentGo = go.get();
//	scene.Add(std::move(go));
//
//	go = std::make_unique<dae::GameObject>();
//	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
//	textureComponent->SetTexture("cldig1.png");
//	go->SetPosition(50, 0);
//	go->SetParent(parentGo, false);
//	go->addComponent(std::move(textureComponent));
//	rapComponent = std::make_unique<dae::RotateAroundPivotComponent>(go.get(), glm::radians(-360.f));
//	go->addComponent(std::move(rapComponent));
//	scene.Add(std::move(go));
//}

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
