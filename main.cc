#include <iostream>

int hello_window();
int hello_triangle();
int shaders();

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	// auto const err = hello_window();
	// auto const err = hello_triangle();
	auto const err = shaders();

	std::cout << "Hello, world!\n";
	return err;
}
