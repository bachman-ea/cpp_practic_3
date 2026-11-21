#ifdef _WIN32
#include <windows.h>
#endif

#include "gamestate.hpp"

int main()
{
#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif
	GameState game;
	game.runMenu();
	return 0;
}
