#pragma once

#ifndef SUMOWRESTLER_HPP
#define SUMOWRESTLER_HPP

#include <string>

class SumoWrestler {
public:
	virtual ~SumoWrestler() = default;
	virtual std::string getName() const = 0;
	virtual char getSymbol() const = 0;
};

#endif
