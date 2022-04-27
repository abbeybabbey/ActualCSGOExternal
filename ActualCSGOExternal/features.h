#pragma once
#include "entity.h"
#include "local_player.h"
#include "memory.h"
#include "offsets.h"

extern bool legit_mode;

class features
{
	static bool is_danger_zone()
	{
		const auto game_rules = read_pm<uintptr_t>(client_base_address + signatures::dwGameRulesProxy);

		if (game_rules == NULL) return false;

		const auto survival_decision_types = read_pm<uintptr_t>(game_rules + netvars::m_SurvivalGameRuleDecisionTypes);

		if (survival_decision_types == NULL) return false;

		return survival_decision_types != 0;
	}

public:
	static void bhop();
	static void trigger_bot();
	static void no_flash(local_player lp_class);
	static void radar_esp(entity en_class, local_player lp_class);
	static void glow_esp(entity en_class, local_player lp_class);
	static void chams(entity en_class, local_player lp_class);
};
