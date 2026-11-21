#pragma once

#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <memory>
#include <string>
#include "constants.hpp"
#include "sumowrestler.hpp"
#include "botsumo.hpp"
#include "playersumo.hpp"
#include "sumofactory.hpp"
#include "input.hpp"

class GameState {
public:
	GameState();
	explicit GameState(std::unique_ptr<SumoFactory> factory);
	~GameState() = default;

	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;

	void setFactory(std::unique_ptr<SumoFactory> factory);
	void runMenu();
	void run();

	int getPlayerPos() const;
	int getBotPos() const;
	int getCollisionCount() const;
	int getHenkaCharges() const;

private:
	void resetGame();
	void clearScreen() const;
	void sleepMs(const int milliseconds) const;
	void printMenu() const;
	void printGameOver(const std::string& title, const std::string& description) const;
	void render(const bool inWindow) const;
	void printArena(const bool inWindow) const;
	void printStatus(const bool inWindow) const;

	Input m_input;
	std::unique_ptr<SumoFactory> m_factory;
	std::unique_ptr<PlayerSumo> m_player;
	std::unique_ptr<BotSumo> m_bot;

	int m_playerPos;
	int m_botPos;
	int m_collisionCount;
	int m_henkaCharges;
};

#endif
