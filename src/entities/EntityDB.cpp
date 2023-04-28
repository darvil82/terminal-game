#include "EntityDB.hpp"

namespace entities {
	namespace definition {
		EntityDB& EntityDB::instance() {
			static EntityDB db;
			return db;
		}

		bool EntityDB::entity_exists(const std::string& classname) {
			return this->db.contains(classname);
		}

		void EntityDB::define_factory(const IEntityFactory& factory, const std::string& classname) {
			this->db[classname] = &factory;
		}
	}

} // entities