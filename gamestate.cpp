#include "gamestate.hpp"
#include <iostream>
#include <chrono>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

GameState::GameState()
	: m_input{},
	  m_factory{ nullptr },
	  m_player{ nullptr },
	  m_bot{ nullptr },
	  m_playerPos{ gameplay_constants::START_PLAYER_POS },
	  m_botPos{ gameplay_constants::START_BOT_POS },
	  m_collisionCount{ gameplay_constants::INITIAL_COLLISION_COUNT },
	  m_henkaCharges{ gameplay_constants::HENKA_INITIAL_CHARGES }
{
}

GameState::GameState(std::unique_ptr<SumoFactory> factory)
	: GameState()
{
	setFactory(std::move(factory));
}

void GameState::setFactory(std::unique_ptr<SumoFactory> factory)
{
	m_factory = std::move(factory);
	if (m_factory != nullptr) {
		m_player = m_factory->createPlayer();
		m_bot = m_factory->createBot();
	}
	else {
		m_player.reset();
		m_bot.reset();
	}
}

int GameState::getPlayerPos() const
{
	return m_playerPos;
}

int GameState::getBotPos() const
{
	return m_botPos;
}

int GameState::getCollisionCount() const
{
	return m_collisionCount;
}

int GameState::getHenkaCharges() const
{
	return m_henkaCharges;
}

void GameState::clearScreen() const
{
#ifdef _WIN32
	std::system("cls");
#else
	std::system("clear");
#endif
}

void GameState::sleepMs(const int milliseconds) const
{
#ifdef _WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * timing_constants::US_PER_MS);
#endif
}

void GameState::printMenu() const
{
	clearScreen();
	std::cout << "===================================================" << std::endl;
	std::cout << "                 СУМО: ТОЛКАЙ УЖЕ!                 " << std::endl;
	std::cout << "===================================================" << std::endl;
	std::cout << " Выберите противника для поединка:" << std::endl << std::endl;
	std::cout << " [1] Медленный сумоист (SlowBot)" << std::endl;
	std::cout << " [2] Агрессивный сумоист (AggressiveBot)" << std::endl;
	std::cout << " [3] Яростный берсерк (BerserkerBot)" << std::endl << std::endl;
	std::cout << " --------------------------------------------------" << std::endl;
	std::cout << " [0/ESC] Выход из игры" << std::endl;
	std::cout << "===================================================" << std::endl;
	std::cout << " Управление:" << std::endl;
	std::cout << "   [Пробел] - Обычный толчок" << std::endl;
	std::cout << "   [E]      - Хэнка (обманный уворот, 2 на матч)" << std::endl;
	std::cout << "===================================================" << std::endl;
	std::cout << "Ваш выбор: ";
}

void GameState::runMenu()
{
	bool running = true;
	while (running) {
		printMenu();
		char ch = m_input.waitForKey();

		switch (ch) {
		case input_constants::KEY_EASY_BOT:
			setFactory(std::make_unique<EasyFactory>());
			run();
			break;
		case input_constants::KEY_HARD_BOT:
			setFactory(std::make_unique<HardFactory>());
			run();
			break;
		case input_constants::KEY_BERSERKER_BOT:
			setFactory(std::make_unique<BerserkerFactory>());
			run();
			break;
		case input_constants::KEY_RETURN:
		case input_constants::KEY_ESCAPE:
			running = false;
			break;
		default:
			break;
		}
	}
	clearScreen();
}

void GameState::resetGame()
{
	m_playerPos = gameplay_constants::START_PLAYER_POS;
	m_botPos = gameplay_constants::START_BOT_POS;
	m_collisionCount = gameplay_constants::INITIAL_COLLISION_COUNT;
	m_henkaCharges = gameplay_constants::HENKA_INITIAL_CHARGES;
	if (m_bot != nullptr) {
		m_bot->reset();
	}
	m_input.clearBuffer();
}

void GameState::printGameOver(const std::string& title, const std::string& description) const
{
	std::cout << std::endl << "====================================================" << std::endl;
	std::cout << title << std::endl;
	std::cout << description << std::endl;
	std::cout << "====================================================" << std::endl;
	std::cout << "Финальная позиция игрока: " << m_playerPos << std::endl;
	std::cout << "Финальная позиция бота:   " << m_botPos << std::endl;
	std::cout << "Количество столкновений: " << m_collisionCount << std::endl << std::endl;
	std::cout << "Нажмите любую клавишу, чтобы вернуться в меню...";
	m_input.waitForKey();
}

void GameState::run()
{
	if (m_factory == nullptr || m_bot == nullptr || m_player == nullptr) {
		setFactory(std::make_unique<EasyFactory>());
	}

	resetGame();

	bool gameOver = false;
	while (!gameOver) {
		m_botPos = m_bot->updatePositionBeforeAttack(m_botPos, m_playerPos);

		double attackTime = m_bot->generateAttackTime();
		double windupDuration = m_bot->getWindupDuration();

		auto collisionStart = std::chrono::steady_clock::now();

		bool resolved = false;
		bool prevInWindow = false;

		render(false);

		while (!resolved) {
			auto now = std::chrono::steady_clock::now();
			double elapsed = std::chrono::duration<double>(now - collisionStart).count();
			bool inWindow = (elapsed >= (attackTime - windupDuration)) && (elapsed < attackTime);

			KeyAction action = m_input.pollAction();

			if (elapsed >= attackTime) {
				m_playerPos -= m_bot->getTimeoutPenalty(m_botPos);
				resolved = true;
				break;
			}

			if (action == KeyAction::SPACE) {
				if (inWindow) {
					m_botPos += gameplay_constants::PLAYER_SUCCESS_PUSH;
				}
				else {
					m_playerPos -= gameplay_constants::PLAYER_EARLY_PENALTY;
				}
				resolved = true;
				break;
			}
			else if (action == KeyAction::SPECIAL_HENKA) {
				if (m_henkaCharges > 0) {
					m_henkaCharges--;
					bool inHenkaWindow = (elapsed >= (attackTime - (windupDuration * gameplay_constants::HENKA_WINDOW_RATIO))) && (elapsed < attackTime);
					if (inHenkaWindow) {
						m_botPos += gameplay_constants::HENKA_SUCCESS_PUSH;
					}
					else {
						m_playerPos -= gameplay_constants::HENKA_EARLY_PENALTY;
					}
					resolved = true;
					break;
				}
			}
			else if (action == KeyAction::EXIT) {
				std::cout << std::endl << "Поединок прерван пользователем." << std::endl;
				return;
			}

			if (inWindow != prevInWindow) {
				prevInWindow = inWindow;
				render(inWindow);
			}

			sleepMs(timing_constants::TICK_SLEEP_MS);
		}

		render(false);

		if (m_botPos > gameplay_constants::ARENA_MAX) {
			gameOver = true;
			printGameOver(
				"                 ПОБЕДА ИГРОКА!                     ",
				" Вы вытолкнули соперника за край арены!             "
			);
			break;
		}
		else if (m_playerPos < gameplay_constants::ARENA_MIN) {
			gameOver = true;
			printGameOver(
				"                   ПОРАЖЕНИЕ!                       ",
				" Бот вытолкнул вас за пределы арены!                 "
			);
			break;
		}
		else if (m_collisionCount >= gameplay_constants::MAX_COLLISIONS) {
			gameOver = true;
			printGameOver(
				"                     НИЧЬЯ!                         ",
				" Никто не вылетел за край арены за лимит раундов!   "
			);
			break;
		}

		m_collisionCount++;
		sleepMs(timing_constants::RESULT_PAUSE_MS);
		m_input.clearBuffer();
	}
}

void GameState::render(const bool inWindow) const
{
	clearScreen();
	std::cout << "Столкновение " << m_collisionCount << std::endl;
	std::cout << "Противник: " << m_bot->getName() << std::endl << std::endl;

	printArena(inWindow);
	printStatus(inWindow);
}

void GameState::printArena(const bool inWindow) const
{
	char arenaStr[gameplay_constants::ARENA_SIZE + 1];
	for (int i = 0; i < gameplay_constants::ARENA_SIZE; ++i) {
		arenaStr[i] = display_constants::ARENA_EMPTY_SYMBOL;
	}
	arenaStr[gameplay_constants::ARENA_SIZE] = '\0';

	char botSymbol = inWindow ? m_bot->getWindupSymbol() : m_bot->getSymbol();

	if (m_playerPos >= gameplay_constants::ARENA_MIN && m_playerPos <= gameplay_constants::ARENA_MAX) {
		arenaStr[m_playerPos] = m_player->getSymbol();
	}
	if (m_botPos >= gameplay_constants::ARENA_MIN && m_botPos <= gameplay_constants::ARENA_MAX) {
		arenaStr[m_botPos] = botSymbol;
	}

	std::cout << "[" << arenaStr << "]" << std::endl;
	std::cout << " " << gameplay_constants::ARENA_MIN
		      << std::string(display_constants::ARENA_LABEL_GAP, ' ')
		      << gameplay_constants::ARENA_MAX << std::endl << std::endl;
}

void GameState::printStatus(const bool inWindow) const
{
	char botSymbol = inWindow ? m_bot->getWindupSymbol() : m_bot->getSymbol();

	std::cout << "игрок (" << m_player->getSymbol() << "): " << m_playerPos << std::endl;
	std::cout << "бот (" << botSymbol << "):   " << m_botPos << std::endl;
	std::cout << "цель: вытолкнуть бота за " << gameplay_constants::ARENA_MAX << std::endl << std::endl;

	std::cout << "Хэнка [E]: " << m_henkaCharges << "/" << gameplay_constants::HENKA_INITIAL_CHARGES << std::endl;
}
