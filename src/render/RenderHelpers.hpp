#pragma once

#include <vector>
#include <functional>

#include "Renderer.hpp"

namespace render {
	class Renderer;


	namespace render_helpers {

		class RenderHelper {
			using ActionFn = std::function<void(Renderer&)>;

			std::vector<ActionFn> actions;

		protected:
			utils::SPoint position;

			void add_action(const ActionFn& action);

		public:
			RenderHelper(const utils::SPoint& position) : position(position) {}

			void operator()(Renderer& renderer);
		};



	}
}