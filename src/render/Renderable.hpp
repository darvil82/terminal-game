#pragma once

#include "Renderer.hpp"

namespace render {

	class IRenderable {
		virtual void render(render::RenderHelper& renderer) const = 0;
	};

}