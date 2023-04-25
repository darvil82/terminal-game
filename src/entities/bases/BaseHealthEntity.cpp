#include "BaseHealthEntity.hpp"

namespace entities {

	void BaseHealthEntity::set_health(uint16_t new_health) {
		this->health = new_health > this->max_health
			? this->max_health
			: new_health;

		if (this->health == 0)
			this->kill();
	}

	void BaseHealthEntity::damage(uint16_t amount) {
		// make sure we prevent health overflow when subtracting
		this->set_health(this->health - (amount > this->health ? this->health : amount));
	}

	void BaseHealthEntity::heal(uint16_t amount) {
		this->set_health(this->health + amount);
	}

	uint16_t BaseHealthEntity::get_health() const {
		return this->health;
	}

	void BaseHealthEntity::set_max_health(uint16_t new_max_health) {
		this->max_health = new_max_health;
		this->set_health(this->health); // clamp health
	}

	uint16_t BaseHealthEntity::get_max_health() const {
		return this->max_health;
	}

} // entities