#include "features.h"
#include "local_player.h"
#include "memory.h"
#include "offsets.h"

bool legit_mode = false;

void features::bhop()
{
	if (legit_mode) return;

	const auto lp_class = local_player();

	if (lp_class.get_local_player() == NULL) return;

	if (!lp_class.is_local_player_alive()) return;

	// checks if local player's flags equal to fl_on_ground or fl_on_ground_crouched, to determine whether we can bhop
	if (const auto can_bhop = lp_class.get_local_player_flags() == 257 || lp_class.get_local_player_flags() == 263;
		GetAsyncKeyState(VK_SPACE) && can_bhop)
	{
		write_pm<int>(client_base_address + signatures::dwForceJump, 6);
	}
}

void features::no_flash(const local_player lp_class)
{
	if (legit_mode) return;

	if (lp_class.get_local_player() == NULL) return;

	write_pm<float>(lp_class.get_local_player() + netvars::m_flFlashDuration, 0.f);
}

void features::radar_esp(const entity en_class, const local_player lp_class)
{
	if (legit_mode) return;

	if (en_class.get_entity_base() == NULL) return;

	if (en_class.is_entity_enemy(lp_class))
	{
		write_pm<bool>(en_class.get_entity_base() + netvars::m_bSpotted, true);
	}
}

struct default_glow_struct
{
	BYTE base[4];
	float red;
	float green;
	float blue;
	float alpha;
	BYTE buffer[16];
	bool render_when_occluded;
	bool render_when_un_occluded;
	bool full_bloom;
	BYTE buffer1[5];
	int glow_style;
	void set_glow_color(float r, float g, float b, float a = 0.8f);
} glow_struct;

void default_glow_struct::set_glow_color(const float r, const float g, const float b, const float a)
{
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

void features::glow_esp(const entity en_class, const local_player lp_class)
{
	if (en_class.get_entity_base() == NULL) return;

	if (!en_class.is_entity_alive()) return;

	const auto glow_manager_object = read_pm<std::uintptr_t>(client_base_address + signatures::dwGlowObjectManager);

	auto e_glow = read_pm<default_glow_struct>(glow_manager_object + en_class.get_glow_index() * 0x38 + 0x4);

	const auto entity_health = static_cast<float>(en_class.get_entity_health());

	e_glow.render_when_un_occluded = false;
	e_glow.render_when_occluded = true;
	e_glow.full_bloom = false;
	e_glow.glow_style = 0;

	if (is_danger_zone())
	{
		e_glow.set_glow_color(entity_health * -0.01f + 1.f, entity_health * 0.01f, 0);
	}
	else
	{
		if (lp_class.get_local_player() == NULL) return;

		if (en_class.is_entity_enemy(lp_class))
		{
			e_glow.set_glow_color(entity_health * -0.01f + 1.f, entity_health * 0.01f, 0);
		}
	}

	if (legit_mode)
	{
		e_glow.glow_style = 1;
		if (en_class.is_entity_spotted())
		{
			write_pm<default_glow_struct>(
				glow_manager_object + en_class.get_glow_index() * 0x38 + 0x4,
				e_glow);
		}
	}
	else
	{
		write_pm<default_glow_struct>(
			glow_manager_object + en_class.get_glow_index() * 0x38 + 0x4,
			e_glow);
	}
}

#pragma region weapon_ids
constexpr short snipers[4] = {9, 11, 38, 40}; // includes AWP, SSG 08, SCAR-20, G3SG1
constexpr short accuracy_pistols[5] = {1, 32, 36, 61, 4}; // includes Desert Deagle, P250, P2000, USP-S, Glock-18
constexpr short knifes[3] = {42, 59, 507}; // includes T knife, CT knife and karambit
#pragma endregion

void features::trigger_bot()
{
	const auto lp_class = local_player();

	if (lp_class.get_local_player() == NULL) return;

	if (!lp_class.is_local_player_alive()) return;

	const auto client_state = read_pm<std::uintptr_t>(engine_base_address + signatures::dwClientState);

	const auto max_player_count = read_pm<std::int32_t>(client_state + signatures::dwClientState_MaxPlayer);

	if (const auto c_hair = lp_class.get_local_player_cross_id(); c_hair || c_hair <= max_player_count )
	{
		const auto en = entity(c_hair - 1);

		if (en.get_entity_base() == NULL) return;

		if (GetAsyncKeyState(VK_XBUTTON1))
		{
			if (!en.is_entity_alive()) return;

			if (en.is_entity_immune()) return;

			const auto [local_x, local_y, local_z] = lp_class.get_local_player_position();
			const auto [entity_x, entity_y, entity_z] = en.get_entity_position();

			const auto distance = static_cast<DWORD>(sqrt(
				pow(local_x - entity_x, 2) + pow(local_y - entity_y, 2) + pow(local_y - entity_y, 2)) * 0.0254);

			const auto weapon_entity = read_pm<std::uintptr_t>(
				client_base_address + signatures::dwEntityList + ((lp_class.get_local_active_weapon() & 0xFFF) - 1) *
				0x10);

			const auto weapon_id = lp_class.get_local_weapon_id(weapon_entity);

			if (weapon_id == NULL || distance == NULL) return;

			if (std::ranges::find(knifes, weapon_id) != std::end(knifes)) return;
			// if weapon_id equals to knife weapon_id trigger_bot won't continue

			DWORD delay = distance * 2; // distance in meters times 2 (simple trigger_bot delay base logic)

			if (std::ranges::find(snipers, weapon_id) != std::end(snipers) && !
				lp_class.is_local_player_scoped())
				return;
			// checks whether local player is not scoped and if weapon_id is in snipers array to avoid trigger_bot shooting while un-scoped with sniper rifles

			if (std::ranges::find(accuracy_pistols, weapon_id) !=
				std::end(accuracy_pistols))
			// checks whether weapon_id is in accuracy_pistols array to change its delay depending on the range
			{
				delay = distance * 9;
			}

			if (is_danger_zone())
			// we check whether we're in danger zone or not, since everyone is in the same team in danger zone and we wouldn't be able to compare entity's team to local player's team
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
				Sleep(20);
				mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
			}
			else
			{
				if (!en.is_entity_enemy(lp_class)) return;
				// if we're not playing danger zone we check if entity is an enemy

				mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
				Sleep(20);
				mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
			}
			Sleep(delay);
		}
	}
}

struct default_clr_struct
{
	std::uint8_t red{}, green{}, blue{};
	void set_chams_color(std::uint8_t r, std::uint8_t g, std::uint8_t b);
} clr_struct;

void default_clr_struct::set_chams_color(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b)
{
	red = r;
	green = g;
	blue = b;
}

void features::chams(const entity en_class, const local_player lp_class)
{
	if (en_class.get_entity_base() == NULL) return;

	if (!en_class.is_entity_alive()) return;

	if (en_class.is_entity_enemy(lp_class))
	{
		clr_struct.set_chams_color(255, 0, 0);
		write_pm<default_clr_struct>(en_class.get_entity_base() + netvars::m_clrRender, clr_struct);
	}
}
