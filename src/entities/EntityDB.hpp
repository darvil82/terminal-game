#pragma once

#include <unordered_map>
#include "EntityFactory.hpp"
#include "EntityDB.hpp"

#define CREATE_ENTITY(type, classname) \
	entities::EntityDB::instance().create_entity<type>(#classname)

namespace entities {

	struct IEntityFactory;

	class EntityDB {
		std::unordered_map<std::string, const IEntityFactory*> db;

		EntityDB() = default;

	public:
		static EntityDB& instance();
		void define_factory(const entities::IEntityFactory& factory, const std::string& classname);

		template<Extends<BaseEntity> T>
		T& create_entity(const std::string& classname) {
			auto* factory = this->db[classname];

			if (!factory)
				throw std::runtime_error("EntityDB::create_entity: entity not found");

			T* ent = dynamic_cast<T*>(factory->create());

			if (!ent)
				throw std::runtime_error("EntityDB::create_entity: entity is not of the requested type");

			return *ent;
		}
	};

} // entities