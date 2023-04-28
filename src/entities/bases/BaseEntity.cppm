module;

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

export module entities.bases.base;

import <string>;

import utils.point;
import utils.concepts;
import utils.typedefs;

import entities.factory;
import game.scene;
import render;
import base;




namespace entities {
	export class BaseEntity : public ITickable, public render::IRenderable {
		friend Scene;

		Scene* scene = nullptr;

	protected:
		utils::Point<> position {0, 0};
		utils::Point<> velocity {0, 0};

		void tick(float delta) override {
			this->position += this->velocity * delta;
		}

		void render(render::render_helpers::RenderUtils&& renderer) const override {}

	public:
		BaseEntity() = default;
		virtual ~BaseEntity() {
			if (this->scene)
				this->scene->detach_entity(*this);
		}

		virtual constexpr const char* get_classname() = 0;

		virtual void kill() {
			delete this;
		}

		Scene* get_scene() {
			return this->scene;
		}
	};


	template<Extends<BaseEntity> T>
	bool ent_is_subclass_of(BaseEntity& ent) {
		return dynamic_cast<T*>(&ent) != nullptr;
	}

	Predicate<BaseEntity&> ent_is_classname(const std::string& classname) {
		return [classname](BaseEntity& ent) -> bool {
			return ent.get_classname() == classname;
		};
	}
}