#pragma once

#include "Renderer.hpp"

namespace render {

	class IRenderable {
		virtual void render(render::Renderer& renderer) = 0;
	};

}