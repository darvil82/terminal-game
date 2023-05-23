#pragma once

#include "Renderer.hpp"
#include "RenderHelpers.hpp"

namespace render {

	class IRenderable {
		virtual void render(render::Renderer& renderer) const = 0;
	};

}