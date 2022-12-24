#pragma once
#include "driver.hpp"
#include <memory>
#include <cstdint>

using c_uint32_t = uint32_t;
using c_wchar_t = wchar_t;
using c_uintptr_t = uintptr_t;
using c_size_t = size_t;
using Driver = NW::driver;

bool driver_init()
{
    return NW::driver_init();
}

uintptr_t get_module_base(uint32_t *proc_id, const wchar_t *mod_name)
{
    uintptr_t address = 0;
    NW::g_driver->get_module_base(*proc_id, mod_name, &address);
    return address;
}

bool read_process_memory(uint32_t *target_proc_id, uintptr_t *read_address, uintptr_t *buffer_address, size_t *size)
{
    return NW::g_driver->read_process_memory(*target_proc_id, *read_address, *buffer_address, *size);
}

bool write_process_memory(uint32_t *target_proc_id, uintptr_t *write_address, uintptr_t *buffer_address, size_t *size)
{
    return NW::g_driver->write_process_memory(*target_proc_id, *write_address, *buffer_address, *size);
}

bool virtual_query(uint32_t *proc_id, uintptr_t *address, uintptr_t *buffer_address, size_t *size)
{
    return NW::g_driver->virtual_query(*proc_id, *address, *buffer_address, *size);
}

bool get_mapped_file_name(uint32_t *proc_id, uintptr_t *address, const wchar_t *buffer, size_t *size)
{
    return NW::g_driver->get_mapped_file_name(*proc_id, *address, buffer, *size);
}

bool test_communicate()
{
    return NW::g_driver->test_communicate();
}

bool read_kernel_memory(uintptr_t *read_address, uintptr_t *buffer_address, size_t *size)
{
    return NW::g_driver->read_kernel_memory(*read_address, *buffer_address, *size);
}

bool get_kernel_driver(const wchar_t *driver_name, uintptr_t *address, size_t *size)
{
    return NW::g_driver->get_kernel_driver(driver_name, address, size);
}