export module entities.bases.health;

import <cstdint>;
import entities.bases.base;


namespace entities {

	export class BaseHealthEntity : public BaseEntity {
		uint16_t health = 100;
		uint16_t max_health = 100;

		void set_health(uint16_t new_health) {
			this->health = new_health > this->max_health
				? this->max_health
				: new_health;

			if (this->health == 0)
				this->kill();
		}

	protected:
		BaseHealthEntity() = default;

	public:
		void damage(uint16_t amount) {
			// make sure we prevent health overflow when subtracting
			this->set_health(this->health - (amount > this->health ? this->health : amount));
		}

		void heal(uint16_t amount) {
			this->set_health(this->health + amount);

		}

		uint16_t get_health() const {
			return this->health;
		}

		void set_max_health(uint16_t new_max_health) {
			this->max_health = new_max_health;
			this->set_health(this->health); // clamp health
		}

		uint16_t get_max_health() const {
			return this->max_health;
		}
	};

} // entities