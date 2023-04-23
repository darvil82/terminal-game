#pragma once

#include <functional>
#include "entities/BaseEntity.hpp"
#include "utils/Typedefs.hpp"

namespace entities {
	class BaseEntity;
}

#define SCENE_MAX_ENTITIES 750

class Scene;



class Scene : public ITickable, public render::IRenderable {
	size_t num_entities = 0;
	entities::BaseEntity* entities[SCENE_MAX_ENTITIES] { };

	class EntitiesIterator {
		const Scene& scene;
		size_t index = 0;

	public:
		EntitiesIterator(const Scene& scene, size_t index = 0): scene{scene}, index{index} { };
		entities::BaseEntity& operator*() const;
		EntitiesIterator& operator++();
		bool operator!=(const EntitiesIterator& other) const;
	};

public:
	const std::vector<entities::BaseEntity*> get_entities_filtered(Predicate<entities::BaseEntity&> filter) const;
	const std::vector<entities::BaseEntity*> get_entities() const;
	EntitiesIterator begin() const;
	EntitiesIterator end() const;

	void attach_entity(entities::BaseEntity& entity);
	void detach_entity(entities::BaseEntity& entity);
	void tick(float delta) override;
	void render(render::Renderer& renderer) const override;
};

