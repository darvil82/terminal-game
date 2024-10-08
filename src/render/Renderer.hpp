#pragma once

#include <inttypes.h>
#include <string>
#include <sstream>
#include <optional>
#include <functional>
#include <tuple>
#include <thread>

#include "Character.hpp"
#include "../utils/Point.hpp"
#include "Pixel.hpp"
#include "../utils/Concepts.hpp"
#include "IRenderHelper.hpp"


namespace render {
	class Renderer {
		using buff_size_t = uint16_t;

		std::stringstream output_stream;
		std::thread render_thread;

		buff_size_t buffer_width, buffer_height;
		Pixel background_pixel = { default_characters::SPACE, utils::default_colors::WHITE, utils::default_colors::BLACK };

		Pixel* global_buffer = nullptr; // heap allocated buffer containing both current and aux matrices
		Pixel* current_buffer = nullptr; // points to start of current relative to global
		Pixel* aux_buffer = nullptr; // points to start of aux relative to global

		bool force_render_next_frame = true; // usually just used for first frame
		bool is_rendering = false;
		bool use_alternate_buffer = false;
		bool enabled_optimization = false; // terminal emulators struggle with lots data per frame

		utils::Point<buff_size_t> last_pixel_position = {0, 0};
		Pixel last_pixel;

		uint8_t max_fps = 50, current_fps = max_fps;
		uint16_t changed_pixels = 0;


		void set_current_fps(uint8_t fps);
		Pixel& get_pixel_from_buffer(const utils::SPoint& pos, Pixel* buff);
		const Pixel& get_pixel_from_buffer(const utils::SPoint& pos, Pixel* buff) const;
		void free_buff();
		bool is_in_bounds(const utils::SPoint& pos) const;
		void push_stream();
		void clear_buffer();
		uint16_t push_buffer(bool force_render = false);
		void render(std::function<void(Renderer&)> func);

	public:
		Renderer(buff_size_t width, buff_size_t height, bool alternate_buffer = false);
		~Renderer();

		std::tuple<buff_size_t, buff_size_t> get_size() const;

		void insert(render_helpers::IRenderHelper&& helper);
		Renderer& operator<<(render_helpers::IRenderHelper&& helper);

		void set_pixel(const Pixel& pixel, const utils::SPoint& position);
		const Pixel& get_pixel(const utils::SPoint& pos) const;

		void set_background_pixel(const Pixel& pixel);

		void set_max_fps(uint8_t fps);
		uint8_t get_max_fps() const;

		uint8_t get_current_fps() const;
		uint16_t get_changed_pixels() const;

		void resize(buff_size_t new_width, buff_size_t new_height);
		void start_render_loop(std::function<void(Renderer&)> func);
		void stop_render_loop();
	};




} // render