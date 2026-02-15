#include "Component.h"
#include "GameObject.h"

dae::Component::Component(std::shared_ptr<dae::GameObject> parent) : m_parent(std::move(parent)) {

}

void dae::Component::Update() {}

void dae::Component::FixedUpdate() {}

void dae::Component::Render() const {}
