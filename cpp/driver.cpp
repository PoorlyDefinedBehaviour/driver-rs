#include "driver.hpp"

namespace NW
{
	bool driver_init()
	{
		g_driver = new driver();

		LoadLibraryA("user32.dll");

		HMODULE mod = LoadLibraryA("win32u.dll");

		if (system("kdu -scv 3 -drvn nw-object -drvr nw -map ./newway-driver.sys") != 0)
		{
			std::cout << "unable map driver" << std::endl;
			return false;
		}

		if (!mod)
			return false;

		g_driver->m_hooked_func = GetProcAddress(mod, "NtUserSetProcessWindowStation");
		// *(PVOID *)&m_hooked_func = GetProcAddress(mod, "NtUserSetProcessWindowStation");
		g_driver->m_current_pid = GetCurrentProcessId();

		if (!g_driver->m_hooked_func || !g_driver->m_current_pid)
			return false;

		return true;
	}

	bool driver::get_module_base(uint32_t proc_id, const wchar_t *mod_name, uint64_t &buffer)
	{
		if (!proc_id || !mod_name)
			return false;

		KERNEL_REQUEST request;
		request.check_code = 0x696969;
		request.instruction_id = KERNEL_INSTRUCTION::INST_GETMODBASE;
		request.source_proc_id = m_current_pid;
		request.target_proc_id = proc_id;
		request.buffer = (UINT_PTR)mod_name;

		call_hook(&request);

		buffer = reinterpret_cast<uint64_t>(request.response);

		return static_cast<bool>(request.succeed);
	}

	bool driver::read_process_memory(uint32_t target_proc_id, uintptr_t read_address, uintptr_t buffer_address, size_t size)
	{
		std::cout << "size=" << size << std::endl;
		if (!read_address || !buffer_address || !size)
			return false;

		size_t read_bytes = 0;

		KERNEL_REQUEST request;
		request.check_code = 0x696969;
		request.instruction_id = KERNEL_INSTRUCTION::INST_READ;
		request.source_proc_id = m_current_pid;
		request.target_proc_id = target_proc_id;
		request.target_address = read_address;
		request.buffer = buffer_address;
		request.size = static_cast<ULONG>(size);

		std::cout << "--- before" << std::endl;
		std::cout << "request.source_proc_id=" << request.source_proc_id << std::endl;
		std::cout << "request.target_proc_id=" << request.target_proc_id << std::endl;
		std::cout << std::hex << "request.target_address=" << request.target_address << std::endl;
		std::cout << std::hex << "request.buffer=" << request.buffer << std::endl;
		std::cout << "request.size=" << request.size << std::endl;

		call_hook(&request);

		read_bytes = reinterpret_cast<size_t>(request.response);

		std::cout << "--- after" << std::endl;
		std::cout << "request.source_proc_id=" << request.source_proc_id << std::endl;
		std::cout << "request.target_proc_id=" << request.target_proc_id << std::endl;
		std::cout << std::hex << "request.target_address=" << request.target_address << std::endl;
		std::cout << std::hex << "request.buffer=" << request.buffer << std::endl;
		std::cout << "request.size=" << request.size << std::endl;
		std::cout << "read_bytes=" << read_bytes << std::endl;

		return static_cast<bool>(request.succeed);
	}

	bool driver::write_process_memory(uint32_t target_proc_id, uintptr_t write_address, uintptr_t buffer_address, size_t size)
	{
		if (!write_address || !buffer_address || !size)
			return false;

		size_t read_bytes = 0;

		KERNEL_REQUEST request;
		request.check_code = 0x696969;
		request.instruction_id = KERNEL_INSTRUCTION::INST_WRITE;
		request.source_proc_id = m_current_pid;
		request.target_proc_id = target_proc_id;
		request.target_address = write_address;
		request.buffer = buffer_address;
		request.size = static_cast<ULONG>(size);

		call_hook(&request);

		read_bytes = reinterpret_cast<size_t>(request.response);

		return static_cast<bool>(request.succeed);
	}

	bool driver::virtual_query(uint32_t proc_id, uintptr_t address, uintptr_t buffer_address, size_t size)
	{
		if (!proc_id || !address || !buffer_address || !size)
			return false;

		size_t read_bytes = 0;

		KERNEL_REQUEST request;
		request.check_code = 0x696969;
		request.instruction_id = KERNEL_INSTRUCTION::INST_VIRTUAL_QUERY;
		request.source_proc_id = m_current_pid;
		request.target_proc_id = proc_id;
		request.target_address = address;
		request.buffer = buffer_address;
		request.size = static_cast<ULONG>(size);

		call_hook(&request);

		read_bytes = reinterpret_cast<size_t>(request.response);

		return static_cast<bool>(request.succeed);
	}

	bool driver::get_mapped_file_name(uint32_t proc_id, uintptr_t address, uintptr_t buffer_address, size_t size)
	{
		if (!proc_id || !address || !buffer_address || !size)
			return false;

		size_t read_bytes = 0;

		KERNEL_REQUEST request;
		request.check_code = 0x696969;
		request.instruction_id = KERNEL_INSTRUCTION::INST_GET_MAPPED_FILENAME;
		request.source_proc_id = m_current_pid;
		request.target_proc_id = proc_id;
		request.target_address = address;
		request.buffer = (UINT_PTR)buffer_address;
		request.size = static_cast<ULONG>(size);

		call_hook(&request);

		read_bytes = reinterpret_cast<size_t>(request.response);

		return static_cast<bool>(request.succeed);
	}

	bool driver::test_communicate()
	{
		KERNEL_REQUEST request;
		request.check_code = 0x696969;
		request.instruction_id = KERNEL_INSTRUCTION::INST_ISRUNNING;
		request.source_proc_id = m_current_pid;

		call_hook(&request);

		return static_cast<bool>(request.succeed);
	}

	bool driver::read_kernel_memory(uintptr_t read_address, uintptr_t buffer_address, size_t size)
	{
		if (!read_address || !buffer_address || !size)
			return false;

		KERNEL_REQUEST request;
		request.check_code = 0x696969;
		request.instruction_id = KERNEL_INSTRUCTION::INST_READ_KERNEL;
		request.source_proc_id = m_current_pid;
		request.target_address = read_address;
		request.buffer = buffer_address;
		request.size = static_cast<ULONG>(size);

		call_hook(&request);

		return static_cast<bool>(request.succeed);
	}

	bool driver::get_kernel_driver(uintptr_t buffer_address, uintptr_t *address, size_t *size)
	{
		if (!buffer_address || !address || !size)
			return false;

		KERNEL_REQUEST request;
		request.check_code = 0x696969;
		request.instruction_id = KERNEL_INSTRUCTION::INST_GET_DRV_INFO;
		request.source_proc_id = m_current_pid;
		request.buffer = (UINT_PTR)buffer_address;

		call_hook(&request);

		*address = reinterpret_cast<uintptr_t>(request.response);
		*size = request.size;

		return static_cast<bool>(request.succeed);
	}
}
