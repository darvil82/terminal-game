#pragma once

#include <unordered_map>
#include "EntityFactory.hpp"
#include "EntityDB.hpp"

#define CREATE_ENTITY(classname) \
	entities::EntityDB::get_instance().create_entity<>(#classname)

namespace entities {

	struct IEntityFactory;

	class EntityDB {
		std::unordered_map<std::string, const IEntityFactory*> db;

		EntityDB() = default;

	public:
		static EntityDB& get_instance();
		void define_factory(const entities::IEntityFactory& factory, const std::string& classname);

		template<Extends<BaseEntity> T>
		T& create_entity(const std::string& classname);
	};

} // entities