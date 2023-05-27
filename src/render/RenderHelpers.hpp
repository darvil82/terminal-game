#pragma once

#include <functional>
#include <deque>

#include "Renderer.hpp"
#include "IRenderHelper.hpp"
#include "../utils/Concepts.hpp"
#include "../utils/Math.hpp"


namespace render {
	class Renderer;

	namespace render_helpers {
		template<class D>
		class RenderHelper : public IRenderHelper {
		protected:
			using ActionFn = std::function<void(Renderer&)>;
			using This = D; // used for subclasses
			using BaseRenderHelper = RenderHelper<D>; // used for subclasses

			wchar_t character = render::default_characters::blocks::FULL;
			utils::SPoint position;
			utils::Color fg = utils::default_colors::WHITE;
			utils::Color bg = utils::default_colors::BLACK;

			void add_action(const ActionFn& action) {
				this->actions.push_back(action);
			}

			void push_pixel(Renderer& renderer) {
				renderer.set_pixel({
					this->character,
					this->fg,
					this->bg
				}, this->position);
			}

			RenderHelper(const utils::SPoint& position) : position(position) { }
		private:
			std::deque<ActionFn> actions;

		public:
			This&& set_color_fg(const utils::Color& color) {
				this->add_action([&, this] (Renderer&) {
					this->fg = color;
				});
				return static_cast<This&&>(*this);
			}

			This&& set_color_bg(const utils::Color& color) {
				this->add_action([&, this] (Renderer&) {
					this->bg = color;
				});
				return static_cast<This&&>(*this);
			}

			void operator()(Renderer& renderer) override {
				for (auto& action : this->actions) {
					action(renderer);
				}
			}
		};


		class DrawRenderHelper : public RenderHelper<DrawRenderHelper> {
			bool is_drawing = false;
		public:
			DrawRenderHelper(const utils::SPoint& position) : RenderHelper(position) { }

			This&& set_character(wchar_t chr);
			This&& start();
			This&& stop();
			This&& move_x(int16_t dist);
			This&& move_y(int16_t dist);
		};


		struct TextRenderHelper : public RenderHelper<TextRenderHelper> {
			TextRenderHelper(const utils::SPoint& position) : RenderHelper(position) { }

			This&& put(const std::wstring& text);
			This&& put_line(const std::wstring& text);
		};
	}
}