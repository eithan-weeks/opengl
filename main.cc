#include <iostream>

int hello_window();

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	auto const err = hello_window();

	std::cout << "Hello, world!\n";
	return err;
}
