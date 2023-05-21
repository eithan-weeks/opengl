#include <iostream>

namespace getting_started {
	int hello_window();
	int hello_triangle();
	int shaders();
	int textures();
	int transformations();
	int coordinate_systems();
	int camera();
};

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	// auto const err {getting_started::hello_window()};
	// auto const err {getting_started::hello_triangle()};
	// auto const err {getting_started::shaders()};
	// auto const err {getting_started::textures()};
	// auto const err {getting_started::transformations()};
	// auto const err {getting_started::coordinate_systems()};
	auto const err {getting_started::camera()};

	std::cout << "Hello, world!\n";
	return err;
}
