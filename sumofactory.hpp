#pragma once

#ifndef SUMOFACTORY_HPP
#define SUMOFACTORY_HPP

#include "sumowrestler.hpp"
#include "botsumo.hpp"
#include "playersumo.hpp"

class SumoFactory {
public:
	virtual ~SumoFactory() = default;

	virtual BotSumo* createBot() = 0;
	virtual PlayerSumo* createPlayer()
	{
		return new PlayerSumo();
	}
};

class EasyFactory : public SumoFactory {
public:
	BotSumo* createBot() override
	{
		return new SlowBot();
	}
};

class HardFactory : public SumoFactory {
public:
	BotSumo* createBot() override
	{
		return new AggressiveBot();
	}
};

class BerserkerFactory : public SumoFactory {
public:
	BotSumo* createBot() override
	{
		return new BerserkerBot();
	}
};

#endif
