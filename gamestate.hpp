#pragma once

#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>
#include "constants.hpp"
#include "sumowrestler.hpp"
#include "botsumo.hpp"
#include "playersumo.hpp"
#include "sumofactory.hpp"

class GameState {
public:
	GameState();
	explicit GameState(SumoFactory* factory);
	~GameState();

	void setFactory(SumoFactory* factory);
	void run();

	int getPlayerPos() const;
	int getBotPos() const;
	int getCollisionCount() const;
	int getHenkaCharges() const;

private:
	void resetGame();
	void render(bool inWindow) const;
	void printArena(bool inWindow) const;
	void printStatus(bool inWindow) const;

	SumoFactory* m_factory;
	PlayerSumo* m_player;
	BotSumo* m_bot;

	int m_playerPos;
	int m_botPos;
	int m_collisionCount;
	int m_henkaCharges;
};

#endif
