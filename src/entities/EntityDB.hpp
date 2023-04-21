#pragma once

#include <unordered_map>
#include "EntityFactory.hpp"
#include "EntityDB.hpp"

#define ENTITY_CREATE(type, classname) \
	entities::EntityDB::instance().create_entity<type>(#classname)
#define ENTITY_EXISTS(classname) \
	entities::EntityDB::instance().entity_exists(#classname)

namespace entities {

	struct IEntityFactory;

	class EntityDB {
		std::unordered_map<std::string, const IEntityFactory*> db;

		EntityDB() = default;

	public:
		static EntityDB& instance();

		void define_factory(const entities::IEntityFactory& factory, const std::string& classname);
		bool entity_exists(const std::string& classname);

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