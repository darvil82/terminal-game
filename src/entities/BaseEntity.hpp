#pragma once

#include "../utils/Point.hpp"
#include "../render/Renderable.hpp"
#include "../Tickable.hpp"
#include "../Scene.hpp"
#include "../Tickable.hpp"


class Scene;

#define DEFINE_ENTITY(entName) \
	public:                       \
	constexpr const char* get_class_name() override { \
        return #entName;       \
    }                          \
	private:\

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
		virtual constexpr const char* get_class_name() = 0;
	};
}