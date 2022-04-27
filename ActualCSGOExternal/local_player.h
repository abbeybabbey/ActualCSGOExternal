#pragma once
#include "includes.h"
#include "d3d9.h"


class local_player
{
	std::uintptr_t local_player_ = 0;
public:
	explicit local_player();

	[[nodiscard]] std::uintptr_t get_local_player() const
	{
		return local_player_;
	}

	[[nodiscard]] std::int32_t get_local_player_health() const;
	[[nodiscard]] bool is_local_player_alive() const;
	[[nodiscard]] bool is_local_player_scoped() const;
	[[nodiscard]] std::int32_t get_local_player_flags() const;
	[[nodiscard]] std::int32_t get_local_shots_fired() const;
	[[nodiscard]] std::int32_t get_local_player_team() const;
	[[nodiscard]] std::int32_t get_local_player_cross_id() const;
	[[nodiscard]] D3DVECTOR get_local_player_position() const;
	[[nodiscard]] std::uintptr_t get_local_active_weapon() const;
	[[nodiscard]] std::int32_t get_local_weapon_id(std::uintptr_t weapon_entity) const;
	[[nodiscard]] D3DVECTOR get_local_aim_punch() const;
};
