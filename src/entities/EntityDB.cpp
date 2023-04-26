module;

#define ENTITY_CREATE(type, classname) \
    entities::EntityDB::instance().create_entity<type>(#classname)
#define ENTITY_EXISTS(classname) \
    entities::EntityDB::instance().entity_exists(#classname)

export module entities.db;

import <unordered_map>;
import <string>;
import <stdexcept>;

import entities.factory;
import entities.bases.base;
import utils.concepts;



namespace entities {

	export class EntityDB {
		std::unordered_map<std::string, const IEntityFactory*> db;

		EntityDB() = default;

	public:
		static EntityDB& instance() {
			static EntityDB db;
			return db;
		}

		void define_factory(const entities::IEntityFactory& factory, const std::string& classname) {
			this->db[classname] = &factory;
		}

		bool entity_exists(const std::string& classname) {
			return this->db.contains(classname);
		}

		template<Extends<BaseEntity> T>
		T& create_entity(const std::string& classname) {
			const auto* factory = this->db[classname];

			if (!factory)
				throw std::runtime_error("entity not found");

			T* ent = dynamic_cast<T*>(factory->create());

			if (!ent)
				throw std::runtime_error("entity is not of the requested type");

			return *ent;
		}
	};

} // entities