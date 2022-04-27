#include "features.h"
#include "includes.h"
#include "memory.h"
#include "offsets.h"

bool is_process_running(const DWORD process_id)
{
	const auto process = OpenProcess(SYNCHRONIZE, FALSE, process_id);
	const auto ret = WaitForSingleObject(process, 0);
	CloseHandle(process);
	return ret == WAIT_TIMEOUT && attached;
}

[[ noreturn ]] void threaded_features()
{
	for (;;)
	{
		features::bhop();
		features::trigger_bot();
	}
}

int main()
{
	SetConsoleTitle("Trash thread my beloved");

	auto mem = memory();

	mem.initialize_memory();

	std::thread threaded_features_thread(threaded_features);

	while (is_process_running(mem.process_id))
	{
		const auto client_state = read_pm<std::uintptr_t>(engine_base_address + signatures::dwClientState);

		if (const auto max_player_count = read_pm<std::int32_t>(client_state + signatures::dwClientState_MaxPlayer);
			max_player_count > 0)
		{
			const local_player lp_class;

			for (auto index = 0; index < max_player_count; index++)
			{
				const auto en_class = entity(index);
				features::radar_esp(en_class, lp_class);
				features::glow_esp(en_class, lp_class);
				features::chams(en_class, lp_class);
			}

			features::no_flash(lp_class);

			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}
		else std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}

	TerminateProcess(GetCurrentProcess(), 42);
}
