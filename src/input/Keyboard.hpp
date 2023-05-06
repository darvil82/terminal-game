#pragma once

#define INPUT_KEY(ks) \
    [] () constexpr {                 \
        using namespace input::keys; \
        return ks; \
    }()

#define __DEFINE_INPUT_KEY(type, name, code) \
	constexpr const type name = type{code}


namespace input {
	namespace keys {

		class KeyboardKey {
		protected:
			char key;
		public:
			explicit constexpr KeyboardKey(char key) : key(key) { }

			constexpr ~KeyboardKey() = default;

			constexpr char get_char_code() const { return this->key; }

			virtual constexpr bool matches(std::array<char, 2> key_buff) const {
				return key_buff[0] == this->key;
			}
		};


		class ModifierKey : public KeyboardKey {
		public:
			using KeyboardKey::KeyboardKey;

			constexpr const KeyboardKey operator+(KeyboardKey key) const {
				return KeyboardKey(key.get_char_code() - this->key);
			}
		};

		class SpecialKey : public KeyboardKey {
		public:
			enum : char {
				UP,
				DOWN,
				LEFT,
				RIGHT,
				ENTER,
				SPACE = 32,
				ESCAPE = 27
			};
		};

		__DEFINE_INPUT_KEY(KeyboardKey, A, 97);
		__DEFINE_INPUT_KEY(KeyboardKey, B, 98);
		__DEFINE_INPUT_KEY(KeyboardKey, C, 99);
		__DEFINE_INPUT_KEY(KeyboardKey, D, 100);
		__DEFINE_INPUT_KEY(KeyboardKey, E, 101);
		__DEFINE_INPUT_KEY(KeyboardKey, F, 102);
		__DEFINE_INPUT_KEY(KeyboardKey, G, 103);
		__DEFINE_INPUT_KEY(KeyboardKey, H, 104);
		__DEFINE_INPUT_KEY(KeyboardKey, I, 105);
		__DEFINE_INPUT_KEY(KeyboardKey, J, 106);
		__DEFINE_INPUT_KEY(KeyboardKey, K, 107);
		__DEFINE_INPUT_KEY(KeyboardKey, L, 108);
		__DEFINE_INPUT_KEY(KeyboardKey, M, 109);
		__DEFINE_INPUT_KEY(KeyboardKey, N, 110);
		__DEFINE_INPUT_KEY(KeyboardKey, O, 111);
		__DEFINE_INPUT_KEY(KeyboardKey, P, 112);
		__DEFINE_INPUT_KEY(KeyboardKey, Q, 113);
		__DEFINE_INPUT_KEY(KeyboardKey, R, 114);
		__DEFINE_INPUT_KEY(KeyboardKey, S, 115);
		__DEFINE_INPUT_KEY(KeyboardKey, T, 116);
		__DEFINE_INPUT_KEY(KeyboardKey, U, 117);
		__DEFINE_INPUT_KEY(KeyboardKey, V, 118);
		__DEFINE_INPUT_KEY(KeyboardKey, W, 119);
		__DEFINE_INPUT_KEY(KeyboardKey, X, 120);
		__DEFINE_INPUT_KEY(KeyboardKey, Y, 121);
		__DEFINE_INPUT_KEY(KeyboardKey, Z, 122);


		// values here represent the number of ascii places to offset from the key
		__DEFINE_INPUT_KEY(ModifierKey, SHIFT, 32);
		__DEFINE_INPUT_KEY(ModifierKey, CTRL, 96);
	}
}