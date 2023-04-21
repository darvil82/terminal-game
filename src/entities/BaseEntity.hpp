#pragma once

#include "../utils/Point.hpp"
#include "../render/Renderable.hpp"
#include "../Tickable.hpp"
#include "../Scene.hpp"
#include "../Tickable.hpp"
#include "EntityFactory.hpp"


class Scene;


#define DEFINE_ENTITY_FACTORY(type) \
    inline EntityFactory<type> classname(type::classname);

#define DEFINE_ENTITY_CLASS(clsname) \
    public:                               \
		static constexpr const char* classname = #clsname; \
                                       \
        virtual constexpr const char* get_classname() override { \
            return classname;    \
        }                              \
                                       \
	private:

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

		virtual constexpr const char* get_classname() = 0;

		Scene* get_scene();
	};
}