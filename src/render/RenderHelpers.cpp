#include "RenderHelpers.hpp"

namespace render {
	namespace render_helpers {

		void RenderHelper::add_action(const ActionFn& action) {
			this->actions.push_back(action);
		}

		void RenderHelper::operator()(Renderer& renderer) {
			for (auto& action : this->actions) {
				action(renderer);
			}
		}
	}
}