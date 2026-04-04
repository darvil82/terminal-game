#pragma once

#include "../../utils/Concepts.hpp"
#include "../BaseEntity.hpp"

namespace entities::filter {
	template<Extends<BaseEntity> T> bool ent_is_subclass_of(const BaseEntity& ent);
	Predicate<const BaseEntity&> ent_is_classname(const std::string& classname);
}
