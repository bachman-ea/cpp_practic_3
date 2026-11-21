#pragma once

#ifndef SUMOFACTORY_HPP
#define SUMOFACTORY_HPP

#include <memory>
#include "sumowrestler.hpp"
#include "botsumo.hpp"
#include "playersumo.hpp"

class SumoFactory {
public:
	virtual ~SumoFactory() = default;

	virtual std::unique_ptr<BotSumo> createBot() const = 0;
	virtual std::unique_ptr<PlayerSumo> createPlayer() const
	{
		return std::make_unique<PlayerSumo>();
	}
};

class EasyFactory : public SumoFactory {
public:
	std::unique_ptr<BotSumo> createBot() const override
	{
		return std::make_unique<SlowBot>();
	}
};

class HardFactory : public SumoFactory {
public:
	std::unique_ptr<BotSumo> createBot() const override
	{
		return std::make_unique<AggressiveBot>();
	}
};

class BerserkerFactory : public SumoFactory {
public:
	std::unique_ptr<BotSumo> createBot() const override
	{
		return std::make_unique<BerserkerBot>();
	}
};

#endif
