#include "input.hpp"
#include "constants.hpp"

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <sys/select.h>
#endif

Input::Input()
#ifndef _WIN32
	: m_rawModeEnabled{ false }, m_origTermios{}
#endif
{
#ifndef _WIN32
	if (tcgetattr(STDIN_FILENO, &m_origTermios) == 0) {
		struct termios raw = m_origTermios;
		raw.c_lflag &= ~(ECHO | ICANON);
		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == 0) {
			m_rawModeEnabled = true;
		}
	}
#endif
}

Input::~Input()
{
#ifndef _WIN32
	if (m_rawModeEnabled) {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_origTermios);
	}
#endif
}

KeyAction Input::pollAction() const
{
#ifdef _WIN32
	if (_kbhit()) {
		int c = _getch();
		if (c == input_constants::KEY_SPACE) {
			return KeyAction::SPACE;
		}
		if (c == input_constants::KEY_SPECIAL_LOWER || c == input_constants::KEY_SPECIAL_UPPER) {
			return KeyAction::SPECIAL_HENKA;
		}
		if (c == input_constants::KEY_ESCAPE || c == input_constants::KEY_RETURN) {
			return KeyAction::EXIT;
		}
	}
	return KeyAction::NONE;
#else
	struct timeval tv = { input_constants::SELECT_TIMEOUT_SEC, input_constants::SELECT_TIMEOUT_USEC };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	if (select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0) {
		char c = 0;
		if (read(STDIN_FILENO, &c, input_constants::SINGLE_BYTE_READ) > 0) {
			if (c == input_constants::KEY_SPACE) {
				return KeyAction::SPACE;
			}
			if (c == input_constants::KEY_SPECIAL_LOWER || c == input_constants::KEY_SPECIAL_UPPER) {
				return KeyAction::SPECIAL_HENKA;
			}
			if (c == input_constants::KEY_ESCAPE || c == input_constants::KEY_RETURN) {
				return KeyAction::EXIT;
			}
		}
	}
	return KeyAction::NONE;
#endif
}

char Input::waitForKey() const
{
#ifdef _WIN32
	return static_cast<char>(_getch());
#else
	char c = 0;
	if (read(STDIN_FILENO, &c, input_constants::SINGLE_BYTE_READ) > 0) {
		return c;
	}
	return 0;
#endif
}

void Input::clearBuffer() const
{
#ifdef _WIN32
	while (_kbhit()) {
		_getch();
	}
#else
	struct timeval tv = { input_constants::SELECT_TIMEOUT_SEC, input_constants::SELECT_TIMEOUT_USEC };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	while (select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0) {
		char c = 0;
		if (read(STDIN_FILENO, &c, input_constants::SINGLE_BYTE_READ) <= 0) {
			break;
		}
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
	}
#endif
}
