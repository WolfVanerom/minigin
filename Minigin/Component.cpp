#include "Component.h"
#include "GameObject.h"

dae::Component::Component(GameObject* parent) : m_parent(parent) {

}

void dae::Component::Update(float) {}

void dae::Component::FixedUpdate() {}

void dae::Component::Render() const {}
