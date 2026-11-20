#include "gamestate.hpp"
#include "input.hpp"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

GameState::GameState()
	: m_factory{ nullptr },
	  m_player{ nullptr },
	  m_bot{ nullptr },
	  m_playerPos{ gameplay_constants::START_PLAYER_POS },
	  m_botPos{ gameplay_constants::START_BOT_POS },
	  m_collisionCount{ gameplay_constants::INITIAL_COLLISION_COUNT },
	  m_henkaCharges{ gameplay_constants::HENKA_INITIAL_CHARGES }
{
}

GameState::GameState(SumoFactory* factory)
	: m_factory{ nullptr },
	  m_player{ nullptr },
	  m_bot{ nullptr },
	  m_playerPos{ gameplay_constants::START_PLAYER_POS },
	  m_botPos{ gameplay_constants::START_BOT_POS },
	  m_collisionCount{ gameplay_constants::INITIAL_COLLISION_COUNT },
	  m_henkaCharges{ gameplay_constants::HENKA_INITIAL_CHARGES }
{
	setFactory(factory);
}

GameState::~GameState()
{
	delete m_player;
	m_player = nullptr;
	delete m_bot;
	m_bot = nullptr;
	delete m_factory;
	m_factory = nullptr;
}

void GameState::setFactory(SumoFactory* factory)
{
	delete m_player;
	m_player = nullptr;
	delete m_bot;
	m_bot = nullptr;
	delete m_factory;
	m_factory = factory;

	if (m_factory != nullptr) {
		m_player = m_factory->createPlayer();
		m_bot = m_factory->createBot();
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

void GameState::resetGame()
{
	m_playerPos = gameplay_constants::START_PLAYER_POS;
	m_botPos = gameplay_constants::START_BOT_POS;
	m_collisionCount = gameplay_constants::INITIAL_COLLISION_COUNT;
	m_henkaCharges = gameplay_constants::HENKA_INITIAL_CHARGES;
	if (m_bot != nullptr) {
		m_bot->reset();
	}
	Input::clearBuffer();
}

void GameState::run()
{
	if (m_factory == nullptr) {
		setFactory(new EasyFactory());
	}

	resetGame();

	bool gameOver = false;
	while (!gameOver) {
		m_botPos = m_bot->updatePositionBeforeAttack(m_botPos, m_playerPos);

		double T = m_bot->generateAttackTime();
		double W = m_bot->getWindupDuration();

		auto collisionStart = std::chrono::steady_clock::now();

		bool resolved = false;
		bool prevInWindow = false;
		bool playerSuccess = false;

		render(false);

		while (!resolved) {
			auto now = std::chrono::steady_clock::now();
			double elapsed = std::chrono::duration<double>(now - collisionStart).count();
			bool inWindow = (elapsed >= (T - W)) && (elapsed < T);

			KeyAction action = Input::pollAction();

			if (elapsed >= T) {
				m_playerPos -= m_bot->getTimeoutPenalty(m_botPos);
				resolved = true;
				playerSuccess = false;
				break;
			}

			if (action == KeyAction::SPACE) {
				if (inWindow) {
					m_botPos += gameplay_constants::PLAYER_SUCCESS_PUSH;
					resolved = true;
					playerSuccess = true;
					break;
				}
				else {
					m_playerPos -= gameplay_constants::PLAYER_EARLY_PENALTY;
					resolved = true;
					playerSuccess = false;
					break;
				}
			}
			else if (action == KeyAction::SPECIAL_HENKA) {
				if (m_henkaCharges > 0) {
					m_henkaCharges--;
					bool inHenkaWindow = (elapsed >= (T - (W * gameplay_constants::HENKA_WINDOW_RATIO))) && (elapsed < T);
					if (inHenkaWindow) {
						m_botPos += gameplay_constants::HENKA_SUCCESS_PUSH;
						resolved = true;
						playerSuccess = true;
						break;
					}
					else {
						m_playerPos -= gameplay_constants::HENKA_EARLY_PENALTY;
						resolved = true;
						playerSuccess = false;
						break;
					}
				}
			}
			else if (action == KeyAction::EXIT) {
				std::cout << "\nПоединок прерван пользователем.\n";
				return;
			}

			if (inWindow != prevInWindow) {
				prevInWindow = inWindow;
				render(inWindow);
			}

#ifdef _WIN32
			Sleep(timing_constants::TICK_SLEEP_MS);
#else
			usleep(timing_constants::TICK_SLEEP_MS * 1000);
#endif
		}

		m_bot->onRoundEnd(playerSuccess);

		render(false);

		if (m_botPos > gameplay_constants::ARENA_MAX) {
			gameOver = true;
			std::cout << "\n====================================================\n";
			std::cout << "                 ПОБЕДА ИГРОКА!                     \n";
			std::cout << " Вы вытолкнули соперника за край арены!             \n";
			std::cout << "====================================================\n";
			std::cout << "Финальная позиция игрока: " << m_playerPos << "\n";
			std::cout << "Финальная позиция бота:   " << m_botPos << "\n";
			std::cout << "Количество столкновений: " << m_collisionCount << "\n\n";
			std::cout << "Нажмите любую клавишу, чтобы вернуться в меню...";
#ifdef _WIN32
			_getch();
#endif
			break;
		}
		else if (m_playerPos < gameplay_constants::ARENA_MIN) {
			gameOver = true;
			std::cout << "\n====================================================\n";
			std::cout << "                   ПОРАЖЕНИЕ!                       \n";
			std::cout << " Бот вытолкнул вас за пределы арены!                 \n";
			std::cout << "====================================================\n";
			std::cout << "Финальная позиция игрока: " << m_playerPos << "\n";
			std::cout << "Финальная позиция бота:   " << m_botPos << "\n";
			std::cout << "Количество столкновений: " << m_collisionCount << "\n\n";
			std::cout << "Нажмите любую клавишу, чтобы вернуться в меню...";
#ifdef _WIN32
			_getch();
#endif
			break;
		}
		else if (m_collisionCount >= gameplay_constants::MAX_COLLISIONS) {
			gameOver = true;
			std::cout << "\n====================================================\n";
			std::cout << "                     НИЧЬЯ!                         \n";
			std::cout << " За " << gameplay_constants::MAX_COLLISIONS << " столкновений никто не вылетел за край!\n";
			std::cout << "====================================================\n";
			std::cout << "Финальная позиция игрока: " << m_playerPos << "\n";
			std::cout << "Финальная позиция бота:   " << m_botPos << "\n";
			std::cout << "Итог: оба сумоиста удержались на арене.\n\n";
			std::cout << "Нажмите любую клавишу, чтобы вернуться в меню...";
#ifdef _WIN32
			_getch();
#endif
			break;
		}

		m_collisionCount++;

#ifdef _WIN32
		Sleep(timing_constants::RESULT_PAUSE_MS);
#else
		usleep(timing_constants::RESULT_PAUSE_MS * 1000);
#endif
		Input::clearBuffer();
	}
}

void GameState::render(bool inWindow) const
{
	std::system("cls");
	std::cout << "Столкновение " << m_collisionCount << "\n";
	std::cout << "Противник: " << m_bot->getName() << "\n\n";

	printArena(inWindow);
	printStatus(inWindow);
}

void GameState::printArena(bool inWindow) const
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

	std::cout << "[" << arenaStr << "]\n";
	std::cout << " " << gameplay_constants::ARENA_MIN
		      << "                            "
		      << gameplay_constants::ARENA_MAX << "\n\n";
}

void GameState::printStatus(bool inWindow) const
{
	char botSymbol = inWindow ? m_bot->getWindupSymbol() : m_bot->getSymbol();

	std::cout << "игрок (" << m_player->getSymbol() << "): " << m_playerPos << "\n";
	std::cout << "бот (" << botSymbol << "):   " << m_botPos << "\n";
	std::cout << "цель: вытолкнуть бота за " << gameplay_constants::ARENA_MAX << "\n\n";

	std::cout << "Хэнка [E]: " << m_henkaCharges << "/" << gameplay_constants::HENKA_INITIAL_CHARGES << "\n";
}
