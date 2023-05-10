#pragma once

#include "Renderer.hpp"

namespace render {

	class IRenderable {
		virtual void render(const render::render_helpers::RenderUtils& render_utils) const = 0;
	};

}