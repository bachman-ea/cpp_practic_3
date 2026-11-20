#pragma once

#ifndef PLAYERSUMO_HPP
#define PLAYERSUMO_HPP

#include <string>
#include "sumowrestler.hpp"

class PlayerSumo : public SumoWrestler {
public:
	PlayerSumo() = default;
	~PlayerSumo() override = default;

	std::string getName() const override;
	char getSymbol() const override;
};

#endif
