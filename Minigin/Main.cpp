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

	go = std::make_unique<dae::GameObject>();
	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
	textureComponent->SetTexture("cldig1.png");
	go->SetPosition(200, 200);
	go->addComponent(std::move(textureComponent));
	auto rapComponent = std::make_unique<dae::RotateAroundPivotComponent>(go.get(), glm::vec3(190, 190, 0), glm::radians(360.f));
	go->addComponent(std::move(rapComponent));
	dae::GameObject* parentGo = go.get();
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	textureComponent = std::make_unique<dae::TextureComponent>(go.get());
	textureComponent->SetTexture("cldig1.png");
	go->SetPosition(50, 0);
	go->SetParent(parentGo, false);
	go->addComponent(std::move(textureComponent));
	rapComponent = std::make_unique<dae::RotateAroundPivotComponent>(go.get(), glm::vec3(0, 0, 0), glm::radians(-360.f));
	go->addComponent(std::move(rapComponent));
	scene.Add(std::move(go));
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
