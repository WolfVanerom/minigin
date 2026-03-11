#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"
#include "GameObject.h"
#include "XinputManager.h"

constexpr unsigned short GAMEPAD_DPAD_UP = 0x0001;
constexpr unsigned short GAMEPAD_DPAD_DOWN = 0x0002;
constexpr unsigned short GAMEPAD_DPAD_LEFT = 0x0004;
constexpr unsigned short GAMEPAD_DPAD_RIGHT = 0x0008;

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	XinputManager& xinputManager = XinputManager::GetInstance();

	// Update XInput state first
	xinputManager.Update();

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

	if (xinputManager.IsConnected(0)) {
		GamepadState gamepad = xinputManager.GetGamepadState(0);
		if (gamepad.buttons & GAMEPAD_DPAD_UP) {
			if (m_commands.find(GAMEPAD_DPAD_UP) != m_commands.end()) {
				m_commands[GAMEPAD_DPAD_UP]->Execute();
			}
		}
		if (gamepad.buttons & GAMEPAD_DPAD_DOWN) {
			if (m_commands.find(GAMEPAD_DPAD_DOWN) != m_commands.end()) {
				m_commands[GAMEPAD_DPAD_DOWN]->Execute();
			}
		}
		if (gamepad.buttons & GAMEPAD_DPAD_LEFT) {
			if (m_commands.find(GAMEPAD_DPAD_LEFT) != m_commands.end()) {
				m_commands[GAMEPAD_DPAD_LEFT]->Execute();
			}
		}
		if (gamepad.buttons & GAMEPAD_DPAD_RIGHT) {
			if (m_commands.find(GAMEPAD_DPAD_RIGHT) != m_commands.end()) {
				m_commands[GAMEPAD_DPAD_RIGHT]->Execute();
			}
		}
	}

	return true;
}