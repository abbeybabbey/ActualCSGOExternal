// ReSharper disable CppClangTidyPerformanceNoIntToPtr
#pragma once
#include "includes.h"

extern std::uintptr_t client_base_address;
extern std::uintptr_t engine_base_address;

extern void* process_handle;

extern bool attached;

class memory
{
public:
	DWORD process_id{};
	void attach_to_process(std::string_view process_name);
	[[nodiscard]] std::uintptr_t get_module_address(std::string_view module_name) const;
	void initialize_memory();
};

// Read process memory
template <typename T>
T read_pm(const std::uintptr_t& address)
{
	T value = {};
	::ReadProcessMemory(process_handle, reinterpret_cast<const void*>(address), &value, sizeof(T), nullptr);
	return value;
}

// Write process memory
template <typename T>
void write_pm(const std::uintptr_t& address, const T& value)
{
	::WriteProcessMemory(process_handle, reinterpret_cast<void*>(address), &value, sizeof(T), nullptr);
}
