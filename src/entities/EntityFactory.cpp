#include "EntityFactory.hpp"
#include "EntityDB.hpp"


namespace entities {

	void register_entity_factory(const IEntityFactory& factory, const char* classname) {
		EntityDB::get_instance().define_factory(factory, classname);
	}

} // entities