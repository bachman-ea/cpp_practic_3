#include "playersumo.hpp"
#include "constants.hpp"

std::string PlayerSumo::getName() const
{
	return "Игрок";
}

char PlayerSumo::getSymbol() const
{
	return display_constants::PLAYER_SYMBOL;
}
