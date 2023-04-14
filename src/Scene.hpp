#pragma once

#include <functional>
#include "entities/BaseEntity.hpp"

namespace entities {
	class BaseEntity;
}

#define SCENE_MAX_ENTITIES 500


class Scene : public ITickable, public render::IRenderable {
	size_t num_entities = 0;
	entities::BaseEntity* entities[SCENE_MAX_ENTITIES]{ };

	void for_each_entity(std::function<void(entities::BaseEntity&)> consumer);
	void for_each_entity(std::function<void(entities::BaseEntity&)> consumer) const;

public:
	void attach_entity(entities::BaseEntity& entity);
	void detach_entity(entities::BaseEntity& entity);
	void tick(float delta) override;
	void render(render::Renderer& renderer) const override;
};

