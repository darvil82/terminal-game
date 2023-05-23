#pragma once

#include "Renderer.hpp"
#include "RenderHelpers.hpp"

namespace render {

	class IRenderable {
		virtual void render(const RenderUtils& render_utils) const = 0;
	};

}