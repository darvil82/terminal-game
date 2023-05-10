#include "Keyboard.hpp"


namespace input {
	namespace keys {
		constexpr const key_buff_t& KeyboardKey::get_seq() const {
			return this->key;
		}

		constexpr bool KeyboardKey::operator==(const input::keys::KeyboardKey& other) const {
			return this->matches(other.key);
		}

		constexpr const KeyboardKey ModifierKey::operator+(input::keys::AlphaKey key) const {
			return KeyboardKey({static_cast<char>(key.get_seq()[0] - this->key[0])});
		}

		constexpr const KeyboardKey ModifierKey::operator+(input::keys::NumericKey key) const {
			const key_buff_t seq = key.get_seq();

			// we'll just handle shift for now
			if (*this != SHIFT) return key;

			if (key == ZERO) return KeyboardKey({')'});

			return KeyboardKey({static_cast<char>(seq[0] - 16)});
		}
	}
}