#pragma once

#include "entities/Entity.hpp"

namespace entities {
	class BaseEntity;
}

#define SCENE_MAX_ENTITIES 500

class Scene {
	size_t num_entities = 0;
	entities::BaseEntity* entities[SCENE_MAX_ENTITIES] {};

public:
	void attach_entity(entities::BaseEntity& entity);
	void detach_entity(entities::BaseEntity& entity);
};

