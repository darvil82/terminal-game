export module entities.factory;

import entities.db;
import entities.bases.base;
import utils.concepts;

namespace entities {

	struct IEntityFactory {
		virtual BaseEntity* create() const = 0;
	};


	export template<Extends<BaseEntity> T>
	struct EntityFactory : public IEntityFactory {
		EntityFactory(const char* classname) {
			EntityDB::instance().define_factory(*this, classname);
		}

		BaseEntity* create() const override {
			return new T();
		}
	};


} // entities