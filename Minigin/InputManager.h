#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include <memory>
#include <unordered_map>

namespace dae
{
	class Command {
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	class CommandWithValue : public Command {
	private:
		std::unique_ptr<Command> m_command;
		float m_value;
	public:
		CommandWithValue(std::unique_ptr<Command> command, float value)
			: m_command(std::move(command)), m_value(value) {}

		void Execute() override {
		}
	};

	class GameObjectCommand : public dae::Command {
	public:
		dae::GameObject* m_gameObject;
	protected:
		dae::GameObject* GetGameObject() const { return m_gameObject; }
		GameObjectCommand(dae::GameObject* gameObject) : m_gameObject(gameObject) {}
		virtual ~GameObjectCommand() = default;
	};

	class moveCommand : public GameObjectCommand {
	private:
		float m_offsetX;
		float m_offsetY;
	public:
		moveCommand(dae::GameObject* gameObject, float offsetX, float offsetY)
			: GameObjectCommand(gameObject), m_offsetX(offsetX), m_offsetY(offsetY) {
		}

		void Execute() override {
			auto pos = GetGameObject()->GetWorldPosition();
			GetGameObject()->SetPosition(pos.x + m_offsetX, pos.y + m_offsetY);
		}
	};


	class InputManager final : public Singleton<InputManager>
	{
	private:
		std::unordered_map<int, std::unique_ptr<Command>> m_commands;
	public:
		bool ProcessInput();
		void AddCommand(int key, std::unique_ptr<Command> command) {
			m_commands[key] = std::move(command);
		}
		void RemoveCommand(int key) {
			m_commands.erase(key);
		}
	};

}
