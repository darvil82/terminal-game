#pragma once

#include <functional>
#include "entities/bases/BaseEntity.hpp"
#include "utils/Typedefs.hpp"

namespace entities {
	class BaseEntity;
}

#define SCENE_MAX_ENTITIES 1500


class Scene : public ITickable, public render::IRenderable {
	size_t num_entities = 0;
	entities::BaseEntity* entities[SCENE_MAX_ENTITIES] { };


	class EntitiesIterator {
		const Scene& scene;
		size_t index = 0;

	public:
		EntitiesIterator(const Scene& scene, size_t index = 0) : scene {scene}, index {index} { };
		entities::BaseEntity& operator*() const;
		EntitiesIterator& operator++();
		bool operator!=(const EntitiesIterator& other) const;
	};


public:
	template<Extends<entities::BaseEntity> T>
	const std::vector<T*> get_entities_filtered(
		std::invocable<entities::BaseEntity&> auto... filters
	) const {
		std::vector<T*> vec;

		for (auto& ent: *this) {
			if ((filters(ent) && ...)) {
				vec.push_back(static_cast<T*>(&ent));
			}
		}

		return vec;
	}

	const std::vector<entities::BaseEntity*> get_entities_filtered(
		std::invocable<entities::BaseEntity&> auto... filters
	) const {
		return this->get_entities_filtered<entities::BaseEntity>(filters...);
	}

	const std::vector<entities::BaseEntity*> get_entities() const;
	EntitiesIterator begin() const;
	EntitiesIterator end() const;

	void attach_entity(entities::BaseEntity& entity);
	void detach_entity(entities::BaseEntity& entity);
	size_t get_entity_count() const;
	void tick(float delta) override;
	void render(const render::render_helpers::RenderUtils& render_utils) const override;
};

