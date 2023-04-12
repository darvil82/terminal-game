#pragma once

#include "Renderer.hpp"

namespace render {

	class IRenderable {
		virtual void draw(Renderer& renderer) = 0;
	};

}