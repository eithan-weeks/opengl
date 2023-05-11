#include <iostream>

int hello_window();
int hello_triangle();

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	// auto const err = hello_window();
	auto const err = hello_triangle();

	std::cout << "Hello, world!\n";
	return err;
}
