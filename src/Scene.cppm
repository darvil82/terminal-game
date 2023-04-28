export module game.scene;

import <vector>;
import <concepts>;
import <cstdint>;
import <stdexcept>;
import render.renderer;
import render;
import entities.bases.base;

import game.tickable;

constexpr const size_t SCENE_MAX_ENTITIES = 750;


export class Scene : public ITickable, public render::IRenderable {
	size_t num_entities = 0;
	entities::BaseEntity* entities[SCENE_MAX_ENTITIES] { };


	class EntitiesIterator {
		const Scene& scene;
		size_t index = 0;

	public:
		EntitiesIterator(const Scene& scene, size_t index = 0) : scene {scene}, index {index} { };
		entities::BaseEntity& operator*() const {
			return *this->scene.entities[this->index];
		}
		EntitiesIterator& operator++() {
			this->index++;
			return *this;
		}
		bool operator!=(const EntitiesIterator& other) const {
			return this->index != other.index;
		}
	};


public:
	const std::vector<entities::BaseEntity*> get_entities_filtered(
		std::invocable<entities::BaseEntity&> auto... filters
	) const {
		std::vector<entities::BaseEntity*> vec;

		for (auto& ent: *this) {
			if ((filters(ent) && ...)) {
				vec.push_back(&ent);
			}
		}

		return vec;
	}

	const std::vector<entities::BaseEntity*> get_entities() const {
		std::vector<entities::BaseEntity*> vec(this->num_entities);

		for (auto& ent: *this) {
			vec.push_back(&ent);
		}

		return vec;
	}
	EntitiesIterator begin() const {
		return EntitiesIterator(*this);
	}
	EntitiesIterator end() const {
		return EntitiesIterator(*this, this->num_entities);
	}

	void attach_entity(entities::BaseEntity& entity) {
		if (this->num_entities >= SCENE_MAX_ENTITIES) {
			throw std::runtime_error("Scene is full");
		}

		if (entity.get_scene()) {
			throw std::runtime_error("Entity already attached to a scene");
		}

		this->entities[this->num_entities++] = &entity;
		entity.scene = this;
	}
	void detach_entity(entities::BaseEntity& entity) {
		for (size_t i = 0; i < this->num_entities; i++) {
			if (this->entities[i] == &entity) {
				this->entities[i] = this->entities[--this->num_entities]; // swap with last
				entity.scene = nullptr; // clear entity's scene
				return;
			}
		}
		throw std::runtime_error("Entity not found in scene");
	}
	void tick(float delta) override {
		for (auto& ent: *this) {
			ent.tick(delta);
		}
	}
	void render(render::render_helpers::RenderUtils&& renderer) const override {
		for (auto& ent: *this) {
			ent.render(std::move(renderer));
		}
	}
};

