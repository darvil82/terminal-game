#include <iostream>

#include "src/render/Renderer.hpp"
#include "src/utils/Cleanup.hpp"

int main() {
	std::string prev_loc = std::setlocale(LC_ALL, nullptr);
	std::setlocale(LC_ALL, "en_US.utf8");

	const auto cleanup = utils::Cleanup([&prev_loc] {
		std::setlocale(LC_ALL, prev_loc.c_str());
	});

	render::Renderer r(50, 20);

	render::Pixel pixel = {
		{ 20, 5 },
		{ 255, 150, 0 },
		render::characters::blocks::FULL
	};

	render::Pixel pixel2 = pixel;
	pixel2.pos = { 25, 6 };
	pixel2.color.b = 200;

	r.set_pixel(pixel);
	r.set_pixel(pixel2);
	r.push_buffer();
}

