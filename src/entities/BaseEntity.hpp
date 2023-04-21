#pragma once

#include "../utils/Point.hpp"
#include "../render/Renderable.hpp"
#include "../Tickable.hpp"
#include "../Scene.hpp"
#include "../Tickable.hpp"
#include "EntityFactory.hpp"

class Scene;


#define DEFINE_ENTITY(type, classname) \
	inline EntityFactory<type> classname(#classname);

namespace entities {
	class BaseEntity : public ITickable, public render::IRenderable {
		friend Scene;

		Scene* scene = nullptr;

	protected:
		utils::Point<> position;
		utils::Point<> velocity;

		void tick(float delta) override;
		void render(render::Renderer& renderer) const override;

	public:
		BaseEntity() = default;
		~BaseEntity();

		Scene* get_scene();
	};
}