#pragma once
#include "driver.hpp"
#include <memory>
#include <cstdint>

using Driver = NW::driver;

bool driver_init()
{
    return NW::driver_init();
}

bool get_module_base(uint32_t proc_id, const std::string &mod_name, uint64_t &buffer)
{
    auto wstring = std::wstring(mod_name.begin(), mod_name.end());
    const wchar_t *mod_name_wchar_t = wstring.c_str();
    return NW::g_driver->get_module_base(proc_id, mod_name_wchar_t, buffer);
}

bool read_process_memory(uint32_t target_proc_id, uint64_t read_address, uint64_t buffer_address, uint64_t size)
{
    return NW::g_driver->read_process_memory(target_proc_id, read_address, buffer_address, size);
}

bool write_process_memory(uint32_t target_proc_id, uint64_t write_address, uint64_t buffer_address, uint64_t size)
{
    return NW::g_driver->write_process_memory(target_proc_id, write_address, buffer_address, size);
}

bool virtual_query(uint32_t proc_id, uint64_t address, uint64_t buffer_address, uint64_t size)
{
    return NW::g_driver->virtual_query(proc_id, address, buffer_address, size);
}

bool get_mapped_file_name(uint32_t proc_id, uint64_t address, uint64_t buffer_address, uint64_t size)
{
    return NW::g_driver->get_mapped_file_name(proc_id, address, buffer_address, size);
}

bool test_communicate()
{
    return NW::g_driver->test_communicate();
}

bool read_kernel_memory(uint64_t read_address, uint64_t buffer_address, uint64_t size)
{
    return NW::g_driver->read_kernel_memory(read_address, buffer_address, size);
}

bool get_kernel_driver(uint64_t buffer_address, uint64_t *address, uint64_t *size)
{
    return NW::g_driver->get_kernel_driver(buffer_address, address, size);
}