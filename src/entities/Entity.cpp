#include "Entity.hpp"


namespace entities {
	BaseEntity::~BaseEntity() {
		if (this->scene)
			this->scene->detach_entity(*this);
	}

	Scene* BaseEntity::get_scene() {
		return this->scene;
	}
}