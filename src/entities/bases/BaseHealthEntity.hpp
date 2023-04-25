#pragma once

#include "BaseEntity.hpp"

namespace entities {

	class BaseHealthEntity : public BaseEntity {
		uint16_t health = 100;
		uint16_t max_health = 100;

		void set_health(uint16_t new_health);

	protected:
		BaseHealthEntity() = default;

	public:
		void damage(uint16_t amount);
		void heal(uint16_t amount);
		uint16_t get_health() const;
		void set_max_health(uint16_t new_max_health);
		uint16_t get_max_health() const;
	};

} // entities