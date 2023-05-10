#pragma once

#include "InputTypes.hpp"

#define INPUT_KEY(ks) \
    [] () constexpr {                 \
        using namespace input::keys; \
        return ks; \
    }()


namespace input {

	namespace keys {

		class KeyboardKey {
		protected:
			key_buff_t key;
		public:
			explicit constexpr KeyboardKey(key_buff_t key) : key(key) { }

			constexpr ~KeyboardKey() = default;

			constexpr const key_buff_t& get_seq() const {
				return this->key;
			}

			virtual constexpr bool matches(const key_buff_t& key_buff) const {
				return this->key == key_buff;
			}

			constexpr bool operator==(const KeyboardKey& other) const {
				return this->matches(other.key);
			}
		};


		class AlphaKey : public KeyboardKey {
		public:
			using KeyboardKey::KeyboardKey;
		};

		class NumericKey : public KeyboardKey {
		public:
			using KeyboardKey::KeyboardKey;
		};


		class ModifierKey : public KeyboardKey {
		public:
			using KeyboardKey::KeyboardKey;

			constexpr const KeyboardKey operator+(const AlphaKey& key) const {
				return KeyboardKey({static_cast<char>(key.get_seq()[0] - this->key[0])});
			}
		};


		class SpecialKey : public KeyboardKey {
		public:
			using KeyboardKey::KeyboardKey;
		};

		constexpr const auto A = AlphaKey({97});
		constexpr const auto B = AlphaKey({98});
		constexpr const auto C = AlphaKey({99});
		constexpr const auto D = AlphaKey({100});
		constexpr const auto E = AlphaKey({101});
		constexpr const auto F = AlphaKey({102});
		constexpr const auto G = AlphaKey({103});
		constexpr const auto H = AlphaKey({104});
		constexpr const auto I = AlphaKey({105});
		constexpr const auto J = AlphaKey({106});
		constexpr const auto K = AlphaKey({107});
		constexpr const auto L = AlphaKey({108});
		constexpr const auto M = AlphaKey({109});
		constexpr const auto N = AlphaKey({110});
		constexpr const auto O = AlphaKey({111});
		constexpr const auto P = AlphaKey({112});
		constexpr const auto Q = AlphaKey({113});
		constexpr const auto R = AlphaKey({114});
		constexpr const auto S = AlphaKey({115});
		constexpr const auto T = AlphaKey({116});
		constexpr const auto U = AlphaKey({117});
		constexpr const auto V = AlphaKey({118});
		constexpr const auto W = AlphaKey({119});
		constexpr const auto X = AlphaKey({120});
		constexpr const auto Y = AlphaKey({121});
		constexpr const auto Z = AlphaKey({122});

		constexpr const auto ZERO = NumericKey({48});
		constexpr const auto ONE = NumericKey({49});
		constexpr const auto TWO = NumericKey({50});
		constexpr const auto THREE = NumericKey({51});
		constexpr const auto FOUR = NumericKey({52});
		constexpr const auto FIVE = NumericKey({53});
		constexpr const auto SIX = NumericKey({54});
		constexpr const auto SEVEN = NumericKey({55});
		constexpr const auto EIGHT = NumericKey({56});
		constexpr const auto NINE = NumericKey({57});

		constexpr const auto RIGHT = SpecialKey({27, 91, 67});
		constexpr const auto LEFT = SpecialKey({27, 91, 68});
		constexpr const auto UP = SpecialKey({27, 91, 65});
		constexpr const auto DOWN = SpecialKey({27, 91, 66});
		constexpr const auto ENTER = SpecialKey({10});
		constexpr const auto SPACE = SpecialKey({32});
		constexpr const auto ESCAPE = SpecialKey({27});

		// values here represent the number of ascii places to offset from the key
		constexpr const auto SHIFT = ModifierKey({32});
		constexpr const auto CTRL = ModifierKey({96});
	}
}