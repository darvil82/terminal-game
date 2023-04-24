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

	void BaseEntity::tick(float delta) {
		this->position += this->velocity * delta;
	}

	void BaseEntity::render(render::render_helpers::RenderUtils&& renderer) const { }

	Predicate<BaseEntity&> ent_is_classname(const std::string& classname) {
		return [classname](BaseEntity& ent) -> bool {
			return ent.get_classname() == classname;
		};
	}

}