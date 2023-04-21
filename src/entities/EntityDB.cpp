#include "EntityDB.hpp"

namespace entities {
	EntityDB& EntityDB::instance() {
		static EntityDB db;
		return db;
	}

	void EntityDB::define_factory(const entities::IEntityFactory& factory, const std::string& classname) {
		this->db[classname] = &factory;
	}

} // entities