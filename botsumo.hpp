#pragma once

#ifndef BOTSUMO_HPP
#define BOTSUMO_HPP

#include <string>
#include <random>
#include "sumowrestler.hpp"
#include "constants.hpp"

class BotSumo : public SumoWrestler {
public:
	virtual ~BotSumo() = default;

	virtual double generateAttackTime() = 0;
	virtual double getWindupDuration() const = 0;
	virtual char getWindupSymbol() const { return display_constants::DEFAULT_WINDUP_SYMBOL; }

	std::string getName() const override { return "Бот"; }
	char getSymbol() const override { return display_constants::BOT_SYMBOL; }

	virtual int updatePositionBeforeAttack(const int currentBotPos, const int currentPlayerPos)
	{
		(void)currentPlayerPos;
		return currentBotPos;
	}

	virtual int getTimeoutPenalty(const int botPos) const
	{
		(void)botPos;
		return gameplay_constants::PLAYER_TIMEOUT_PENALTY;
	}

	virtual void reset() {}
};

class SlowBot : public BotSumo {
public:
	SlowBot();
	~SlowBot() override = default;

	double generateAttackTime() override;
	double getWindupDuration() const override;
	std::string getName() const override;

private:
	std::mt19937 m_rng;
	std::uniform_real_distribution<double> m_dist;
};

class AggressiveBot : public BotSumo {
public:
	AggressiveBot();
	~AggressiveBot() override = default;

	double generateAttackTime() override;
	double getWindupDuration() const override;
	std::string getName() const override;

private:
	std::mt19937 m_rng;
	std::uniform_real_distribution<double> m_dist;
};

class BerserkerBot : public BotSumo {
public:
	BerserkerBot();
	~BerserkerBot() override = default;

	double generateAttackTime() override;
	double getWindupDuration() const override;
	char getWindupSymbol() const override;
	std::string getName() const override;

	int updatePositionBeforeAttack(const int currentBotPos, const int currentPlayerPos) override;
	int getTimeoutPenalty(const int botPos) const override;
	void reset() override;

private:
	std::mt19937 m_rng;
	int m_lastBotPos;
};

#endif
