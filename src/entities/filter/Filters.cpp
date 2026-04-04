#include "Filters.hpp"

namespace entities::filter {
	Predicate<const BaseEntity&> ent_is_classname(const std::string& classname) {
		return [classname](const BaseEntity& ent) -> bool {
			return ent.get_classname() == classname;
		};
	}

	template<Extends<BaseEntity> T>
	bool ent_is_subclass_of(const BaseEntity& ent) {
		return dynamic_cast<T*>(&ent) != nullptr;
	}
}