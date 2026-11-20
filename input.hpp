#pragma once

#ifndef INPUT_HPP
#define INPUT_HPP

enum class KeyAction {
	NONE,
	SPACE,
	SPECIAL_HENKA,
	EXIT
};

class Input {
public:
	static KeyAction pollAction();
	static bool isSpacePressed();
	static bool isSpecialPressed();
	static void clearBuffer();
};

bool isSpacePressed();

#endif
