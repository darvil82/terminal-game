#include <cstdio>
#include <stdexcept>

#include "Scene.hpp"


void Scene::attach_entity(entities::BaseEntity& entity) {
	if (this->num_entities >= SCENE_MAX_ENTITIES) {
		throw std::runtime_error("Scene::attach_entity: Scene is full");
	}

	if (entity.get_scene()) {
		throw std::runtime_error("Scene::attach_entity: Entity already attached to a scene");
	}

	this->entities[this->num_entities++] = &entity;
	entity.scene = this;
}

void Scene::detach_entity(entities::BaseEntity& entity) {
	for (uint16_t i = 0; i < this->num_entities; i++) {
		if (this->entities[i] == &entity) {
			this->entities[i] = this->entities[--this->num_entities]; // swap with last
			entity.scene = nullptr;
			return;
		}
	}
	throw std::runtime_error("Scene::detach_entity: Entity not found in scene");
}

