#include "entity.h"
#include "memory.h"
#include "offsets.h"

entity::entity(const std::uintptr_t index)
{
	this->entity_base_ = read_pm<uintptr_t>(client_base_address + signatures::dwEntityList + index * 0x10);
}

int entity::get_entity_team() const
{
	return read_pm<int>(get_entity_base() + netvars::m_iTeamNum);
}

bool entity::is_entity_enemy(const local_player lp_class) const
{
	return get_entity_team() != lp_class.get_local_player_team();
}

bool entity::is_entity_spotted() const
{
	return read_pm<bool>(get_entity_base() + netvars::m_bSpottedByMask);
}

bool entity::is_entity_immune() const
{
	return read_pm<bool>(get_entity_base() + netvars::m_bGunGameImmunity);
}

bool entity::is_entity_alive() const
{
	return get_entity_health() >= 1;
}

int entity::get_glow_index() const
{
	return read_pm<int>(get_entity_base() + netvars::m_iGlowIndex);
}

int entity::get_entity_health() const
{
	return read_pm<int>(get_entity_base() + netvars::m_iHealth);
}

D3DVECTOR entity::get_entity_position() const
{
	return read_pm<D3DVECTOR>(get_entity_base() + netvars::m_vecOrigin);
}

std::uintptr_t entity::get_entity_clr_renderer() const
{
	return read_pm<std::uintptr_t>(get_entity_base() + netvars::m_clrRender);
}
