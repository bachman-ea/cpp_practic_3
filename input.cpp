#include "input.hpp"
#include "constants.hpp"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstdlib>

static bool s_initTermios = false;
static struct termios s_origTermios;

static void disableRawMode()
{
	if (s_initTermios) {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &s_origTermios);
		s_initTermios = false;
	}
}

static void enableRawMode()
{
	if (!s_initTermios) {
		tcgetattr(STDIN_FILENO, &s_origTermios);
		std::atexit(disableRawMode);
		struct termios raw = s_origTermios;
		raw.c_lflag &= ~(ECHO | ICANON);
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
		s_initTermios = true;
	}
}
#endif

KeyAction Input::pollAction()
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
	enableRawMode();
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	if (select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0) {
		char c = 0;
		if (read(STDIN_FILENO, &c, 1) > 0) {
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

bool Input::isSpacePressed()
{
#ifdef _WIN32
	if (_kbhit()) {
		int c = _getch();
		return c == input_constants::KEY_SPACE;
	}
	return false;
#else
	return pollAction() == KeyAction::SPACE;
#endif
}

bool Input::isSpecialPressed()
{
	return pollAction() == KeyAction::SPECIAL_HENKA;
}

void Input::clearBuffer()
{
#ifdef _WIN32
	while (_kbhit()) {
		_getch();
	}
#else
	enableRawMode();
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	while (select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0) {
		char c = 0;
		if (read(STDIN_FILENO, &c, 1) <= 0) {
			break;
		}
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
	}
#endif
}

bool isSpacePressed()
{
	return Input::isSpacePressed();
}
