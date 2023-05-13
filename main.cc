#include <iostream>

int hello_window();
int hello_triangle();
int shaders();
int textures();
int transformations();
int coordinate_systems();

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	// auto const err {hello_window()};
	// auto const err {hello_triangle()};
	// auto const err {shaders()};
	// auto const err {textures()};
	// auto const err {transformations()};
	auto const err {coordinate_systems()};

	std::cout << "Hello, world!\n";
	return err;
}
