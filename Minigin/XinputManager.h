#pragma once
#include "Singleton.h"
#include <memory>

namespace dae
{
	// Platform-agnostic gamepad state structure
	struct GamepadState
	{
		unsigned short buttons;
		unsigned char leftTrigger;
		unsigned char rightTrigger;
		short leftThumbX;
		short leftThumbY;
		short rightThumbX;
		short rightThumbY;
	};

	class XinputManager final : public Singleton<XinputManager>
	{
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	public:
		XinputManager();
		~XinputManager();
		void Update();
		bool IsConnected(unsigned int dwUserIndex) const;
		GamepadState GetGamepadState(unsigned int dwUserIndex) const;
	};
}