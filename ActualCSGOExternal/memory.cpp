#include "memory.h"

std::uintptr_t client_base_address = NULL;
std::uintptr_t engine_base_address = NULL;

void* process_handle = nullptr;

bool attached = false;

void memory::attach_to_process(const std::string_view process_name)
{
	try
	{
		PROCESSENTRY32 entry = {};
		entry.dwSize = sizeof(PROCESSENTRY32);

		const auto snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (Process32First(snap_shot, &entry))
		{
			while (Process32Next(snap_shot, &entry))
			{
				if (process_name.compare(entry.szExeFile) == 0)
				{
					//https://habr.com/ru/post/448472/
					if (const auto process_duplicate_handle = OpenProcess(
							PROCESS_DUP_HANDLE, FALSE, entry.th32ProcessID);
						DuplicateHandle(process_duplicate_handle, GetCurrentProcess(), GetCurrentProcess(),
						                &process_handle,
						                0,
						                false, DUPLICATE_SAME_ACCESS) != 0)
					//… if process A has a handle to process B with PROCESS_DUP_HANDLE access, it can duplicate the pseudo handle for process B. This creates a handle that has maximum access to process B.
					{
						process_id = entry.th32ProcessID;
						std::cout << "Found csgo.exe (PID: " << process_id << ")" << std::endl;
						CloseHandle(process_duplicate_handle);
						attached = true;
					}
					break;
				}
			}
		}

		// Free handle
		if (snap_shot)
			CloseHandle(snap_shot);
	}
	catch (std::exception& e)
	{
		std::cout << "Failed to attach to a process: " << e.what() << std::endl;
	}
}

std::uintptr_t memory::get_module_address(const std::string_view module_name) const
{
	try
	{
		MODULEENTRY32 entry = {};
		entry.dwSize = sizeof(MODULEENTRY32);

		const auto snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);

		std::uintptr_t result = 0;

		while (Module32Next(snap_shot, &entry))
		{
			if (module_name.compare(entry.szModule) == 0)
			{
				result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}

		if (snap_shot)
			CloseHandle(snap_shot);

		return result;
	}
	catch (std::exception& e)
	{
		std::cout << "Failed to get module address: " << e.what() << std::endl;
	}
	return NULL;
}

void memory::initialize_memory()
{
	attach_to_process("csgo.exe");

	if (!attached)
	{
		std::cout << "Please start csgo first before launching the cheat!" << std::endl;
		return;
	}

	client_base_address = get_module_address("client.dll");
	engine_base_address = get_module_address("engine.dll");

	std::cout << "client.dll address: " << "0x" << std::hex << client_base_address << std::dec << std::endl;
	std::cout << "engine.dll address: " << "0x" << std::hex << engine_base_address << std::dec << std::endl;
}
