#include <cstdio>
#include <stdexcept>

#include "Scene.hpp"


void Scene::for_each_entity(std::function<void(entities::BaseEntity&)> consumer) {
	for (size_t i = 0; i < this->num_entities; i++) {
		consumer(*this->entities[i]);
	}
}

void Scene::for_each_entity(std::function<void(entities::BaseEntity&)> consumer) const {
	for (size_t i = 0; i < this->num_entities; i++) {
		consumer(*this->entities[i]);
	}
}

void Scene::attach_entity(entities::BaseEntity& entity) {
	if (this->num_entities >= SCENE_MAX_ENTITIES) {
		throw std::runtime_error("Scene is full");
	}

	if (entity.get_scene()) {
		throw std::runtime_error("Entity already attached to a scene");
	}

	this->entities[this->num_entities++] = &entity;
	entity.scene = this;
}

void Scene::detach_entity(entities::BaseEntity& entity) {
	for (size_t i = 0; i < this->num_entities; i++) {
		if (this->entities[i] == &entity) {
			this->entities[i] = this->entities[--this->num_entities]; // swap with last
			entity.scene = nullptr; // clear entity's scene
			return;
		}
	}
	throw std::runtime_error("Entity not found in scene");
}

void Scene::tick(float delta) {
	this->for_each_entity([=](auto& ent) {
		ent.tick(delta);
	});
}

void Scene::render(render::Renderer& renderer) const {
	this->for_each_entity([&](auto& ent) {
		ent.render(renderer);
	});
}