#pragma once
#include "includes.h"
#include "local_player.h"
#include "d3d9.h"

class entity
{
	std::uintptr_t entity_base_ = 0;
public:
	explicit entity(std::uintptr_t index);

	[[nodiscard]] uintptr_t get_entity_base() const
	{
		return entity_base_;
	}

	[[nodiscard]] int get_entity_team() const;
	[[nodiscard]] bool is_entity_enemy(local_player lp_class) const;
	[[nodiscard]] bool is_entity_spotted() const;
	[[nodiscard]] bool is_entity_immune() const;
	[[nodiscard]] bool is_entity_alive() const;
	[[nodiscard]] int get_glow_index() const;
	[[nodiscard]] int get_entity_health() const;
	[[nodiscard]] D3DVECTOR get_entity_position() const;
	[[nodiscard]] std::uintptr_t get_entity_clr_renderer() const;
};
