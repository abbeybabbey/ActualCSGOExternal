#include "local_player.h"
#include "memory.h"
#include "offsets.h"

local_player::local_player()
{
	this->local_player_ = read_pm<std::uintptr_t>(client_base_address + signatures::dwLocalPlayer);
}

std::int32_t local_player::get_local_player_health() const
{
	return read_pm<std::int32_t>(get_local_player() + netvars::m_iHealth);
}

bool local_player::is_local_player_alive() const
{
	return get_local_player_health() >= 1;
}

bool local_player::is_local_player_scoped() const
{
	return read_pm<bool>(get_local_player() + netvars::m_bIsScoped);
}

std::int32_t local_player::get_local_player_flags() const
{
	return read_pm<std::int32_t>(get_local_player() + netvars::m_fFlags);
}

std::int32_t local_player::get_local_shots_fired() const
{
	return read_pm<std::int32_t>(get_local_player() + netvars::m_iShotsFired);
}

std::int32_t local_player::get_local_player_team() const
{
	return read_pm<std::int32_t>(get_local_player() + netvars::m_iTeamNum);
}

std::int32_t local_player::get_local_player_cross_id() const
{
	return read_pm<std::int32_t>(get_local_player() + netvars::m_iCrosshairId);
}

D3DVECTOR local_player::get_local_player_position() const
{
	return read_pm<D3DVECTOR>(get_local_player() + netvars::m_vecOrigin);
}

std::uintptr_t local_player::get_local_active_weapon() const
{
	return read_pm<std::uintptr_t>(get_local_player() + netvars::m_hActiveWeapon);
}

std::int32_t local_player::get_local_weapon_id(const std::uintptr_t weapon_entity) const
{
	return read_pm<std::int32_t>(weapon_entity + netvars::m_iItemDefinitionIndex);
}

D3DVECTOR local_player::get_local_aim_punch() const
{
	return read_pm<D3DVECTOR>(get_local_player() + netvars::m_aimPunchAngle);
}
