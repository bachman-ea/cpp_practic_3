#include <iostream>
#include <clocale>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "gamestate.hpp"
#include "sumofactory.hpp"
#include "constants.hpp"

int main()
{
#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif

	enum MenuChoice {
		EASY_BOT = '1',
		HARD_BOT = '2',
		BERSERKER_BOT = '3',
		EXIT_NUM = input_constants::KEY_RETURN,
		EXIT_ESC = input_constants::KEY_ESCAPE
	};

	bool running = true;
	while (running) {
		std::system("cls");
		std::cout << "===================================================\n";
		std::cout << "                 СУМО: ТОЛКАЙ УЖЕ!                 \n";
		std::cout << "===================================================\n";
		std::cout << " Выберите противника для поединка:\n\n";
		std::cout << " [1] Медленный сумоист (SlowBot)\n";
		std::cout << " [2] Агрессивный сумоист (AggressiveBot)\n";
		std::cout << " [3] Яростный берсерк (BerserkerBot)\n\n";
		std::cout << " --------------------------------------------------\n";
		std::cout << " [0/ESC] Выход из игры\n";
		std::cout << "===================================================\n";
		std::cout << " Управление:\n";
		std::cout << "   [Пробел] - Обычный толчок\n";
		std::cout << "   [E]      - Хэнка (обманный уворот, 2 на матч)\n";
		std::cout << "===================================================\n";
		std::cout << "Ваш выбор: ";

		char ch = _getch();
		SumoFactory* factory = nullptr;

		switch (ch) {
		case EASY_BOT:
			factory = new EasyFactory();
			break;
		case HARD_BOT:
			factory = new HardFactory();
			break;
		case BERSERKER_BOT:
			factory = new BerserkerFactory();
			break;
		case EXIT_NUM:
		case EXIT_ESC:
			running = false;
			continue;
		default:
			continue;
		}

		if (factory != nullptr) {
			GameState gameState{ factory };
			gameState.run();
		}
	}
	std::system("cls");
	return 0;
}
