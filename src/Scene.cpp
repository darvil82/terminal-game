#include <cstdio>
#include <stdexcept>

#include "Scene.hpp"


Scene::EntitiesIterator Scene::begin() const {
	return EntitiesIterator(*this);
}

Scene::EntitiesIterator Scene::end() const {
	return EntitiesIterator(*this, this->num_entities);
}

entities::BaseEntity& Scene::EntitiesIterator::operator*() const {
	return *this->scene.entities[this->index];
}

Scene::EntitiesIterator& Scene::EntitiesIterator::operator++() {
	this->index++;
	return *this;
}

bool Scene::EntitiesIterator::operator!=(const Scene::EntitiesIterator& other) const {
	return this->index != other.index;
}

std::vector<entities::BaseEntity*> Scene::get_entities_filtered(Predicate<entities::BaseEntity&> filter) {
	std::vector<entities::BaseEntity*> vec;

	for (auto& ent : *this) {
		if (filter(ent)) {
			vec.push_back(&ent);
		}
	}

	return vec;
}

std::vector<entities::BaseEntity*> Scene::get_entities() {
	std::vector<entities::BaseEntity*> vec(this->num_entities);

	for (auto& ent : *this) {
		vec.push_back(&ent);
	}

	return vec;
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
	for (auto& ent : *this) {
		ent.tick(delta);
	}
}

void Scene::render(render::Renderer& renderer) const {
	for (auto& ent : *this) {
		ent.render(renderer);
	}
}