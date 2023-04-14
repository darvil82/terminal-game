#include "BaseEntity.hpp"


namespace entities {
	BaseEntity::~BaseEntity() {
		if (this->scene)
			this->scene->detach_entity(*this);
	}

	Scene* BaseEntity::get_scene() {
		return this->scene;
	}

	void BaseEntity::tick(float delta) { }

	void BaseEntity::render(render::Renderer& renderer) const { }
}