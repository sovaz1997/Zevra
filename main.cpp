#include <iostream>
#include "game.hpp"

int main() {
	std::string str;

	while(str != "uci") {
		std::getline(std::cin, str);
	}

	std::cout << "id name Tiger\n";
	std::cout << "id author sovaz1997\n";

	Game game;
}
