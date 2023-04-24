#pragma once

#include "BaseEntity.hpp"

namespace entities {

	class HealthEntity : public BaseEntity {
		uint16_t health = 100;

	protected:
		HealthEntity() = default;

	public:
		void damage(uint16_t amount);
		void heal(uint16_t amount);
		uint16_t get_health() const;
	};

} // entities