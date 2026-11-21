#pragma once

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace gameplay_constants {
	constexpr int ARENA_MIN = 0;
	constexpr int ARENA_MAX = 30;
	constexpr int ARENA_SIZE = 31;

	constexpr int START_PLAYER_POS = 10;
	constexpr int START_BOT_POS = 20;

	constexpr int INITIAL_COLLISION_COUNT = 1;
	constexpr int MAX_COLLISIONS = 20;

	constexpr int PLAYER_SUCCESS_PUSH = 3;
	constexpr int PLAYER_EARLY_PENALTY = 1;
	constexpr int PLAYER_TIMEOUT_PENALTY = 2;

	constexpr double SLOW_BOT_ATTACK_MIN = 1.5;
	constexpr double SLOW_BOT_ATTACK_MAX = 2.5;
	constexpr double SLOW_BOT_WINDUP_DURATION = 0.7;

	constexpr double AGGRESSIVE_BOT_ATTACK_MIN = 0.8;
	constexpr double AGGRESSIVE_BOT_ATTACK_MAX = 1.5;
	constexpr double AGGRESSIVE_BOT_WINDUP_DURATION = 0.3;

	constexpr int BERSERKER_MIN_ADVANCE_POS = 14;
	constexpr int BERSERKER_MIN_SAFETY_DISTANCE = 3;
	constexpr int BERSERKER_ADVANCE_CHANCE_PERCENT = 75;
	constexpr int BERSERKER_ADVANCE_STEP = 2;
	constexpr int BERSERKER_CLOSE_RANGE_THRESHOLD = 16;
	constexpr double BERSERKER_CLOSE_ATTACK_MIN = 0.7;
	constexpr double BERSERKER_CLOSE_ATTACK_MAX = 1.1;
	constexpr double BERSERKER_FAR_ATTACK_MIN = 1.2;
	constexpr double BERSERKER_FAR_ATTACK_MAX = 1.8;
	constexpr double BERSERKER_CLOSE_WINDUP = 0.22;
	constexpr double BERSERKER_FAR_WINDUP = 0.40;
	constexpr int BERSERKER_CLOSE_TIMEOUT_PENALTY = 3;

	constexpr int HENKA_INITIAL_CHARGES = 2;
	constexpr int HENKA_SUCCESS_PUSH = 4;
	constexpr int HENKA_EARLY_PENALTY = 2;
	constexpr double HENKA_WINDOW_RATIO = 0.5;

	constexpr int PERCENT_DIST_MIN = 1;
	constexpr int PERCENT_DIST_MAX = 100;
}

namespace display_constants {
	constexpr char PLAYER_SYMBOL = '*';
	constexpr char BOT_SYMBOL = 'O';
	constexpr char DEFAULT_WINDUP_SYMBOL = '@';
	constexpr char BERSERKER_ENRAGED_SYMBOL = '!';
	constexpr char ARENA_EMPTY_SYMBOL = '.';
	constexpr int ARENA_LABEL_GAP = 28;
}

namespace timing_constants {
	constexpr int TICK_SLEEP_MS = 5;
	constexpr int RESULT_PAUSE_MS = 600;
	constexpr int US_PER_MS = 1000;
}

namespace input_constants {
	constexpr char KEY_SPACE = ' ';
	constexpr char KEY_SPECIAL_LOWER = 'e';
	constexpr char KEY_SPECIAL_UPPER = 'E';
	constexpr char KEY_ESCAPE = 27;
	constexpr char KEY_RETURN = '0';
	constexpr char KEY_EASY_BOT = '1';
	constexpr char KEY_HARD_BOT = '2';
	constexpr char KEY_BERSERKER_BOT = '3';
	constexpr long SELECT_TIMEOUT_SEC = 0L;
	constexpr long SELECT_TIMEOUT_USEC = 0L;
	constexpr int SINGLE_BYTE_READ = 1;
}

#endif
