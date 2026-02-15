#pragma once
#include <memory>

namespace dae
{
	class GameObject;

	class Component
	{
	protected:
		std::shared_ptr<GameObject> m_parent{};
	public:
		Component(std::shared_ptr<GameObject> parent);
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render() const;
		virtual ~Component() = default;
	};
}
