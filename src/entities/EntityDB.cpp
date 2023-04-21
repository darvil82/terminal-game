#include "EntityDB.hpp"

namespace entities {
	EntityDB& EntityDB::get_instance() {
		static EntityDB db;
		return db;
	}

	void EntityDB::define_factory(const entities::IEntityFactory& factory, const std::string& classname) {
		this->db[classname] = &factory;
	}

	template<Extends<BaseEntity> T>
	T& EntityDB::create_entity(const std::string& classname) {
		return this->db[classname]->create();
	}
} // entities