#include <functional>
#include "BaseEntity.hpp"
#include "RenderUtils.hpp"

namespace entities {
	BaseEntity::~BaseEntity() {
		if (this->scene)
			this->scene->detach_entity(*this);
	}

	Scene* BaseEntity::get_scene() {
		return this->scene;
	}

	void BaseEntity::set_position(const utils::Point<>& new_pos) {
		this->position = new_pos;
	}

	void BaseEntity::set_velocity(const utils::Point<>& new_vel) {
		this->velocity = new_vel;
	}

	void BaseEntity::tick(float delta) {
		this->position += this->velocity * delta;
	}

	void BaseEntity::render(const RenderUtils& render_utils) const { }

	void BaseEntity::kill() {
		delete this;
	}

	Predicate<BaseEntity&> ent_is_classname(const std::string& classname) {
		return [classname](BaseEntity& ent) -> bool {
			return ent.get_classname() == classname;
		};
	}

}