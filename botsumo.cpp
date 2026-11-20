#include "botsumo.hpp"
#include "constants.hpp"
#include <algorithm>

SlowBot::SlowBot()
	: m_rng{ std::random_device{}() },
	  m_dist{ gameplay_constants::SLOW_BOT_ATTACK_MIN, gameplay_constants::SLOW_BOT_ATTACK_MAX }
{
}

double SlowBot::generateAttackTime()
{
	return m_dist(m_rng);
}

double SlowBot::getWindupDuration()
{
	return gameplay_constants::SLOW_BOT_WINDUP_DURATION;
}

std::string SlowBot::getName() const
{
	return "Медленный сумоист";
}

AggressiveBot::AggressiveBot()
	: m_rng{ std::random_device{}() },
	  m_dist{ gameplay_constants::AGGRESSIVE_BOT_ATTACK_MIN, gameplay_constants::AGGRESSIVE_BOT_ATTACK_MAX }
{
}

double AggressiveBot::generateAttackTime()
{
	return m_dist(m_rng);
}

double AggressiveBot::getWindupDuration()
{
	return gameplay_constants::AGGRESSIVE_BOT_WINDUP_DURATION;
}

std::string AggressiveBot::getName() const
{
	return "Агрессивный сумоист";
}

BerserkerBot::BerserkerBot()
	: m_rng{ std::random_device{}() },
	  m_lastBotPos{ gameplay_constants::START_BOT_POS },
	  m_lastPlayerPos{ gameplay_constants::START_PLAYER_POS }
{
}

int BerserkerBot::updatePositionBeforeAttack(int currentBotPos, int currentPlayerPos)
{
	m_lastPlayerPos = currentPlayerPos;
	if (currentBotPos > gameplay_constants::BERSERKER_MIN_ADVANCE_POS &&
		currentBotPos > currentPlayerPos + gameplay_constants::BERSERKER_MIN_SAFETY_DISTANCE) {
		std::uniform_int_distribution<int> dist(
			gameplay_constants::PERCENT_DIST_MIN,
			gameplay_constants::PERCENT_DIST_MAX
		);
		if (dist(m_rng) <= gameplay_constants::BERSERKER_ADVANCE_CHANCE_PERCENT) {
			int newPos = currentBotPos - gameplay_constants::BERSERKER_ADVANCE_STEP;
			if (newPos < gameplay_constants::BERSERKER_MIN_ADVANCE_POS) {
				newPos = gameplay_constants::BERSERKER_MIN_ADVANCE_POS;
			}
			m_lastBotPos = newPos;
			return newPos;
		}
	}
	m_lastBotPos = currentBotPos;
	return currentBotPos;
}

double BerserkerBot::generateAttackTime()
{
	if (m_lastBotPos <= gameplay_constants::BERSERKER_CLOSE_RANGE_THRESHOLD) {
		std::uniform_real_distribution<double> dist(
			gameplay_constants::BERSERKER_CLOSE_ATTACK_MIN,
			gameplay_constants::BERSERKER_CLOSE_ATTACK_MAX
		);
		return dist(m_rng);
	}
	std::uniform_real_distribution<double> dist(
		gameplay_constants::BERSERKER_FAR_ATTACK_MIN,
		gameplay_constants::BERSERKER_FAR_ATTACK_MAX
	);
	return dist(m_rng);
}

double BerserkerBot::getWindupDuration()
{
	if (m_lastBotPos <= gameplay_constants::BERSERKER_CLOSE_RANGE_THRESHOLD) {
		return gameplay_constants::BERSERKER_CLOSE_WINDUP;
	}
	return gameplay_constants::BERSERKER_FAR_WINDUP;
}

char BerserkerBot::getWindupSymbol() const
{
	if (m_lastBotPos <= gameplay_constants::BERSERKER_CLOSE_RANGE_THRESHOLD) {
		return display_constants::BERSERKER_ENRAGED_SYMBOL;
	}
	return display_constants::DEFAULT_WINDUP_SYMBOL;
}

std::string BerserkerBot::getName() const
{
	return "Яростный берсерк";
}

int BerserkerBot::getTimeoutPenalty(int botPos) const
{
	if (botPos <= gameplay_constants::BERSERKER_CLOSE_RANGE_THRESHOLD) {
		return gameplay_constants::BERSERKER_CLOSE_TIMEOUT_PENALTY;
	}
	return gameplay_constants::PLAYER_TIMEOUT_PENALTY;
}

void BerserkerBot::reset()
{
	m_lastBotPos = gameplay_constants::START_BOT_POS;
	m_lastPlayerPos = gameplay_constants::START_PLAYER_POS;
}
