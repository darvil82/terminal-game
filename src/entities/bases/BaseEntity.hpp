#pragma once

#include "../../utils/Point.hpp"
#include "../../render/Renderable.hpp"
#include "../../Tickable.hpp"
#include "../../Scene.hpp"
#include "../../Tickable.hpp"
#include "../definition/EntityFactory.hpp"
#include "../../utils/Concepts.hpp"
#include "../../utils/Typedefs.hpp"


class Scene;


#define DEFINE_ENTITY_FACTORY(type) \
    inline definition::EntityFactory<type> classname(type::classname);

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
		utils::Point<> position {0, 0};
		utils::Point<> velocity {0, 0};

		void tick(float delta) override;
		void render(const render::render_helpers::RenderUtils& render_utils) const override;

	public:
		BaseEntity() = default;
		virtual ~BaseEntity();

		virtual constexpr const char* get_classname() = 0;

		virtual void kill();
		Scene* get_scene();
		void set_position(const utils::Point<>& new_pos);
		void set_velocity(const utils::Point<>& new_vel);
	};


	template<Extends<BaseEntity> T>
	bool ent_is_subclass_of(BaseEntity& ent) {
		return dynamic_cast<T*>(&ent) != nullptr;
	}

	Predicate<BaseEntity&> ent_is_classname(const std::string& classname);
}