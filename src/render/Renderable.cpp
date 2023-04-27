export module render;

namespace render {

	export class IRenderable {
		virtual void render(render::render_helpers::RenderUtils&& renderer) const = 0;
	};

}