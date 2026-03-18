#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"
#include "GameObject.h"

constexpr unsigned short GAMEPAD_DPAD_UP = 0x0001;
constexpr unsigned short GAMEPAD_DPAD_DOWN = 0x0002;
constexpr unsigned short GAMEPAD_DPAD_LEFT = 0x0004;
constexpr unsigned short GAMEPAD_DPAD_RIGHT = 0x0008;

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;


	while (SDL_PollEvent(&e)) {

		if (e.type == SDL_EVENT_QUIT) {
			return false;
		}
		if (e.type == SDL_EVENT_KEY_DOWN) {
			if (m_commands.find(e.key.key) != m_commands.end()) {
				m_commands[e.key.key]->Execute();
			}
		}
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {

		}

		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	return true;
}