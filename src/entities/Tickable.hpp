#pragma once

namespace entities {
	class ITickable {
		virtual void tick(float delta) = 0;
	};
}