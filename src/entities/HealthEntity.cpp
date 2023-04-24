#include "HealthEntity.hpp"

namespace entities {

	void HealthEntity::damage(uint16_t amount) {
		auto new_health = this->health - amount;

		if (new_health <= 0)
			this->kill();
		else
			this->health = new_health;
	}

	void HealthEntity::heal(uint16_t amount) {
		this->health += amount;
	}

	uint16_t HealthEntity::get_health() const {
		return this->health;
	}

} // entities