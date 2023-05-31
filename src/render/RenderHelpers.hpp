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

		enum class Alignment : uint8_t {
			LEFT,
			CENTER,
			RIGHT
		};

		template<class D>
		class RenderHelper : public IRenderHelper {
		protected:
			using ActionFn = std::function<void()>;
			using This = D; // used for subclasses
			using BaseRenderHelper = RenderHelper<D>; // used for subclasses

			wchar_t character = render::default_characters::blocks::FULL;
			utils::SPoint position;
			utils::Color fg = utils::default_colors::WHITE;
			utils::Color bg = utils::default_colors::BLACK;

			void add_action(const ActionFn& action) {
				this->actions.push_back(action);
			}

			void set_pixel(const utils::SPoint& pos) {
				renderer->set_pixel({
					this->character,
					this->fg,
					this->bg
				}, pos);
			}

			virtual void push_changes() {
				this->set_pixel(this->position);
			}

			RenderHelper(const utils::SPoint& position) : position(position) { }

		public:
			This&& set_color_fg(const utils::Color& color) {
				this->add_action([&, this] () {
					this->fg = color;
				});
				return static_cast<This&&>(*this);
			}

			This&& set_color_bg(const utils::Color& color) {
				this->add_action([&, this] () {
					this->bg = color;
				});
				return static_cast<This&&>(*this);
			}

			void operator()(Renderer& r) override {
				this->renderer = &r;

				for (auto& action : this->actions) {
					action();
				}
			}

		private:
			std::deque<ActionFn> actions;
			Renderer* renderer = nullptr; // set when operator() is called
		};


		class DrawRenderHelper : public RenderHelper<DrawRenderHelper> {
			bool is_drawing = false;
			uint8_t thickness = 1;
		public:
			DrawRenderHelper(const utils::SPoint& position) : RenderHelper(position) { }

			void push_changes() override;
			This&& set_character(wchar_t chr);
			This&& start();
			This&& put();
			This&& stop();
			This&& move_x(int16_t dist);
			This&& move_y(int16_t dist);
			This&& set_thickness(uint8_t new_thickness);
		};


		class TextRenderHelper : public RenderHelper<TextRenderHelper> {
			Alignment alignment = Alignment::LEFT;
			utils::SPoint initial_pos = this->position;

			size_t get_line_x_offset(const std::wstring& text) const;
		public:
			TextRenderHelper(const utils::SPoint& position) : RenderHelper(position) { }

			This&& set_alignment(Alignment new_alignment);
			This&& put(const std::wstring& text);
			This&& put_line(const std::wstring& text);
		};
	}
}