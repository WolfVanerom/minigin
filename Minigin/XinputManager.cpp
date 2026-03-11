#include "XinputManager.h"
#include <SDL3/SDL.h>

#ifdef _WIN32
	#include <Windows.h>
	#include <Xinput.h>
	#define USE_XINPUT
#else
	#define USE_SDL_GAMEPAD
#endif

namespace dae
{
	class XinputManager::Impl
	{
	public:
#ifdef USE_XINPUT
		XINPUT_STATE m_state[4];

		Impl()
		{
			for (int i = 0; i < 4; ++i)
			{
				m_state[i] = {};
			}
		}

		void Update()
		{
			for (DWORD i = 0; i < 4; ++i) {
				XInputGetState(i, &m_state[i]);
			}
		}

		bool IsConnected(unsigned int dwUserIndex) const
		{
			if (dwUserIndex >= 4) return false;
			DWORD dwResult = XInputGetState(dwUserIndex, const_cast<XINPUT_STATE*>(&m_state[dwUserIndex]));
			return dwResult == ERROR_SUCCESS;
		}

		GamepadState GetGamepadState(unsigned int dwUserIndex) const
		{
			GamepadState state = {};
			if (dwUserIndex >= 4) return state;

			const XINPUT_GAMEPAD& gamepad = m_state[dwUserIndex].Gamepad;
			state.buttons = gamepad.wButtons;
			state.leftTrigger = gamepad.bLeftTrigger;
			state.rightTrigger = gamepad.bRightTrigger;
			state.leftThumbX = gamepad.sThumbLX;
			state.leftThumbY = gamepad.sThumbLY;
			state.rightThumbX = gamepad.sThumbRX;
			state.rightThumbY = gamepad.sThumbRY;

			return state;
		}
#else
		SDL_Gamepad* m_gamepad[4];

		Impl()
		{
			for (int i = 0; i < 4; ++i)
			{
				m_gamepad[i] = nullptr;
			}
		}

		void Update()
		{
			for (int i = 0; i < 4; ++i)
			{
				int num_joysticks = 0;
				const int* joysticks = SDL_GetJoysticks(&num_joysticks);

				if (i < num_joysticks)
				{
					if (m_gamepad[i] == nullptr)
					{
						m_gamepad[i] = SDL_OpenGamepad(joysticks[i]);
					}
				}
				else if (m_gamepad[i] != nullptr)
				{
					SDL_CloseGamepad(m_gamepad[i]);
					m_gamepad[i] = nullptr;
				}
			}
		}

		bool IsConnected(unsigned int dwUserIndex) const
		{
			if (dwUserIndex >= 4) return false;
			return m_gamepad[dwUserIndex] != nullptr;
		}

		GamepadState GetGamepadState(unsigned int dwUserIndex) const
		{
			GamepadState state = {};
			if (dwUserIndex >= 4) return state;
			if (m_gamepad[dwUserIndex] == nullptr) return state;

			unsigned short buttons = 0;

			if (SDL_GetGamepadButton(m_gamepad[dwUserIndex], SDL_GAMEPAD_BUTTON_DPAD_UP))
				buttons |= 0x0001;
			if (SDL_GetGamepadButton(m_gamepad[dwUserIndex], SDL_GAMEPAD_BUTTON_DPAD_DOWN))
				buttons |= 0x0002;
			if (SDL_GetGamepadButton(m_gamepad[dwUserIndex], SDL_GAMEPAD_BUTTON_DPAD_LEFT))
				buttons |= 0x0004;
			if (SDL_GetGamepadButton(m_gamepad[dwUserIndex], SDL_GAMEPAD_BUTTON_DPAD_RIGHT))
				buttons |= 0x0008; 

			state.buttons = buttons;
			state.leftTrigger = static_cast<unsigned char>(SDL_GetGamepadAxis(m_gamepad[dwUserIndex], SDL_GAMEPAD_AXIS_LEFT_TRIGGER) / 257);
			state.rightTrigger = static_cast<unsigned char>(SDL_GetGamepadAxis(m_gamepad[dwUserIndex], SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) / 257);
			state.leftThumbX = SDL_GetGamepadAxis(m_gamepad[dwUserIndex], SDL_GAMEPAD_AXIS_LEFTX);
			state.leftThumbY = SDL_GetGamepadAxis(m_gamepad[dwUserIndex], SDL_GAMEPAD_AXIS_LEFTY);
			state.rightThumbX = SDL_GetGamepadAxis(m_gamepad[dwUserIndex], SDL_GAMEPAD_AXIS_RIGHTX);
			state.rightThumbY = SDL_GetGamepadAxis(m_gamepad[dwUserIndex], SDL_GAMEPAD_AXIS_RIGHTY);

			return state;
		}
#endif
	};

	XinputManager::XinputManager()
		: m_pImpl(std::make_unique<Impl>())
	{
	}

	XinputManager::~XinputManager() = default;

	void XinputManager::Update()
	{
		m_pImpl->Update();
	}

	bool XinputManager::IsConnected(unsigned int dwUserIndex) const
	{
		return m_pImpl->IsConnected(dwUserIndex);
	}

	GamepadState XinputManager::GetGamepadState(unsigned int dwUserIndex) const
	{
		return m_pImpl->GetGamepadState(dwUserIndex);
	}
}

