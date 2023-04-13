#pragma once

#include "../utils/Point.hpp"
#include "../render/Renderable.hpp"
#include "../Tickable.hpp"
#include "../Scene.hpp"
#include "../Tickable.hpp"

class Scene;

namespace entities {
	class BaseEntity : public ITickable {
		friend Scene;

		Scene* scene;

	protected:
		utils::Point<> position;
		utils::Point<> velocity;

		void tick(float delta) override {
			this->position += this->velocity * delta;
		}

	public:
		BaseEntity() = delete;
		~BaseEntity();

		Scene* get_scene();
	};
}