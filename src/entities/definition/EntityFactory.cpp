#include "EntityFactory.hpp"
#include "EntityDB.hpp"


namespace entities {
	namespace definition {
		void register_entity_factory(const IEntityFactory& factory, const char* classname) {
			EntityDB::instance().define_factory(factory, classname);
		}
	}
} // entities