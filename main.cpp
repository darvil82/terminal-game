#include <iostream>

#include "src/render/Renderer.hpp"
#include "src/utils/Cleanup.hpp"
#include "src/Scene.hpp"
#include "src/entities/Entity.hpp"

int main() {
	std::string prev_loc = std::setlocale(LC_ALL, nullptr);
	std::setlocale(LC_ALL, "en_US.utf8");

	const auto cleanup = utils::Cleanup([&prev_loc] {
		std::setlocale(LC_ALL, prev_loc.c_str());
	});

	render::Renderer r(50, 10);

	render::Pixel pixel = {
		render::default_characters::blocks::FULL,
		{ 255, 150, 0 },
	};

	render::Pixel pixel2 = pixel;
	pixel2.color_fg.b = 200;

	r.set_pixel(pixel, { 20, 5 });
	r.set_pixel(pixel2, { 25, 6 });
	r.push_buffer();

	Scene scene;
	entities::BaseEntity ent1;
	entities::BaseEntity ent2;
	entities::BaseEntity ent3;

	scene.attach_entity(ent1);
	scene.attach_entity(ent2);
	scene.attach_entity(ent3);

	scene.detach_entity(ent2);
}

