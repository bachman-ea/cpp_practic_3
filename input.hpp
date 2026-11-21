#pragma once

#ifndef INPUT_HPP
#define INPUT_HPP

#ifndef _WIN32
#include <termios.h>
#endif

enum class KeyAction {
	NONE,
	SPACE,
	SPECIAL_HENKA,
	EXIT
};

class Input {
public:
	Input();
	~Input();

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	KeyAction pollAction() const;
	char waitForKey() const;
	void clearBuffer() const;

private:
#ifndef _WIN32
	bool m_rawModeEnabled;
	struct termios m_origTermios;
#endif
};

#endif
