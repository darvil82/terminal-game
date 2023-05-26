#pragma once

#include "Renderer.hpp"

namespace render {
	class Renderer;

	namespace render_helpers {
		class IRenderHelper {
		public:
			virtual void operator()(Renderer& renderer) = 0;
		};
	}
}