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
#include "Scene.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load(dae::Minigin& engine)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_shared<dae::GameObject>();
	go->SetTexture("background.png");
	scene.Add(std::move(go));

	go = std::make_shared<dae::GameObject>();
	go->SetTexture("logo.png");
	go->SetPosition(358, 180);
	scene.Add(std::move(go));

	go = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto tc = std::make_shared<dae::TextComponent>(go, "Programming 4 Assignment", font, SDL_Color{ 255, 255, 0, 255 });
	tc->SetPosition(292, 20);
	go->addComponent(tc);
	scene.Add(std::move(go));

	go = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto tcFpscounter = std::make_shared<dae::TextComponent>(go, "FPS: 0", font, SDL_Color{ 255, 255, 255, 255 });
	tcFpscounter->SetPosition(10, 10);
	auto fpsCounter = std::make_shared<dae::FPSComponent>(go, tcFpscounter, &engine);
	go->addComponent(tcFpscounter);
	go->addComponent(fpsCounter);
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
