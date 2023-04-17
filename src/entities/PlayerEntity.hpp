#pragma once

#include <iostream>
#include "BaseEntity.hpp"

namespace entities {

	class PlayerEntity : public BaseEntity {
		DEFINE_ENTITY(player)

		std::string player_name;

	public:
		PlayerEntity() {
		}
	};

}