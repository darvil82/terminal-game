#include <functional>
#include "BaseEntity.hpp"

namespace entities {
	BaseEntity::~BaseEntity() {
		if (this->scene)
			this->scene->detach_entity(*this);
	}

	Scene* BaseEntity::get_scene() {
		return this->scene;
	}

	void BaseEntity::set_position(const utils::FPoint& new_pos) {
		this->position = new_pos;
	}

	void BaseEntity::set_velocity(const utils::FPoint& new_vel) {
		this->velocity = new_vel;
	}

	void BaseEntity::tick(float delta) {
		this->position += this->velocity * delta;
	}

	void BaseEntity::render(render::Renderer& renderer) const { }

	void BaseEntity::kill() {
		delete this;
	}

	Predicate<const BaseEntity&> ent_is_classname(const std::string& classname) {
		return [classname](const BaseEntity& ent) -> bool {
			return ent.get_classname() == classname;
		};
	}

}