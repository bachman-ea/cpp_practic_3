#pragma once

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace gameplay_constants {
	const int ARENA_MIN = 0;
	const int ARENA_MAX = 30;
	const int ARENA_SIZE = 31;

	const int START_PLAYER_POS = 10;
	const int START_BOT_POS = 20;

	const int INITIAL_COLLISION_COUNT = 1;
	const int MAX_COLLISIONS = 20;

	const int PLAYER_SUCCESS_PUSH = 3;
	const int PLAYER_EARLY_PENALTY = 1;
	const int PLAYER_TIMEOUT_PENALTY = 2;

	const double SLOW_BOT_ATTACK_MIN = 1.5;
	const double SLOW_BOT_ATTACK_MAX = 2.5;
	const double SLOW_BOT_WINDUP_DURATION = 0.7;

	const double AGGRESSIVE_BOT_ATTACK_MIN = 0.8;
	const double AGGRESSIVE_BOT_ATTACK_MAX = 1.5;
	const double AGGRESSIVE_BOT_WINDUP_DURATION = 0.3;

	const int BERSERKER_MIN_ADVANCE_POS = 14;
	const int BERSERKER_MIN_SAFETY_DISTANCE = 3;
	const int BERSERKER_ADVANCE_CHANCE_PERCENT = 75;
	const int BERSERKER_ADVANCE_STEP = 2;
	const int BERSERKER_CLOSE_RANGE_THRESHOLD = 16;
	const double BERSERKER_CLOSE_ATTACK_MIN = 0.7;
	const double BERSERKER_CLOSE_ATTACK_MAX = 1.1;
	const double BERSERKER_FAR_ATTACK_MIN = 1.2;
	const double BERSERKER_FAR_ATTACK_MAX = 1.8;
	const double BERSERKER_CLOSE_WINDUP = 0.22;
	const double BERSERKER_FAR_WINDUP = 0.40;
	const int BERSERKER_CLOSE_TIMEOUT_PENALTY = 3;

	const int HENKA_INITIAL_CHARGES = 2;
	const int HENKA_SUCCESS_PUSH = 4;
	const int HENKA_EARLY_PENALTY = 2;
	const double HENKA_WINDOW_RATIO = 0.5;

	const int PERCENT_DIST_MIN = 1;
	const int PERCENT_DIST_MAX = 100;
}

namespace display_constants {
	const char PLAYER_SYMBOL = '*';
	const char BOT_SYMBOL = 'O';
	const char DEFAULT_WINDUP_SYMBOL = '@';
	const char BERSERKER_ENRAGED_SYMBOL = '!';
	const char ARENA_EMPTY_SYMBOL = '.';
}

namespace timing_constants {
	const int TICK_SLEEP_MS = 5;
	const int RESULT_PAUSE_MS = 600;
}

namespace input_constants {
	const char KEY_SPACE = ' ';
	const char KEY_SPECIAL_LOWER = 'e';
	const char KEY_SPECIAL_UPPER = 'E';
	const char KEY_ESCAPE = 27;
	const char KEY_RETURN = '0';
}

#endif
