#pragma once

#include "common.hpp"

typedef enum _KERNEL_INSTRUCTION
{
	INST_GETMODBASE = 1,
	INST_READ,
	INST_WRITE,
	INST_VIRTUAL_QUERY,
	INST_GET_MAPPED_FILENAME,
	INST_GET_DRV_INFO,
	INST_READ_KERNEL,
	INST_ISRUNNING
} KERNEL_INSTRUCTION,
	*PKERNEL_INSTRUCTION;

typedef struct _KERNEL_REQUEST
{
	ULONG check_code;
	ULONG instruction_id;
	ULONG source_proc_id;
	ULONG target_proc_id;
	UINT_PTR target_address;
	UINT_PTR buffer;
	ULONG size;
	PVOID response;
	BOOLEAN succeed;
} KERNEL_REQUEST, *PKERNEL_REQUEST;

namespace NW
{
	bool driver_init();

	class driver
	{
	public:
		driver() = default;
		~driver() = default;

		bool driver::get_module_base(uint32_t proc_id, const wchar_t *mod_name, uintptr_t *address);
		bool driver::read_process_memory(uint32_t target_proc_id, uintptr_t read_address, uintptr_t buffer_address, size_t size);
		bool driver::write_process_memory(uint32_t target_proc_id, uintptr_t write_address, uintptr_t buffer_address, size_t size);
		bool driver::virtual_query(uint32_t proc_id, uintptr_t address, uintptr_t buffer_address, size_t size);
		bool driver::get_mapped_file_name(uint32_t proc_id, uintptr_t address, const wchar_t *buffer, size_t size);
		bool driver::test_communicate();
		bool driver::read_kernel_memory(uintptr_t read_address, uintptr_t buffer_address, size_t size);
		bool driver::get_kernel_driver(const wchar_t *driver_name, uintptr_t *address, size_t *size);

		// Reads a value from the process.
		template <typename T>
		bool read(uint64_t address, T &value);

		// Writes a value to the process.
		template <typename T>
		bool write(uint64_t address, const T &value);

		// Reads an array of values from the process.
		template <typename T>
		bool read_array(uint64_t address, T *array, size_t len);

		// Writes an array of values to the process.
		template <typename T>
		bool write_array(uint64_t address, const T *array, size_t len);

		template <class type>
		type rpm(uint64_t address)
		{
			type tmp;
			if (read_process_memory(address, (uint64_t)&tmp, sizeof(type)))
				return tmp;
			else
				return {0};
		}

		template <typename... arg>
		uint64_t call_hook(const arg... args);

		void *m_hooked_func;
		uint32_t m_current_pid;
	};

	template <typename... arg>
	inline uint64_t driver::call_hook(const arg... args)
	{
		// setting up a function template
		auto func = static_cast<uint64_t(_stdcall *)(arg...)>(m_hooked_func);
		return func(args...); // call the function
	}

	inline driver *g_driver;

	template <typename T>
	inline bool driver::read(uint64_t address, T &value)
	{
		return read_process_memory(address, (uintptr_t)&value, sizeof(T));
	}

	template <typename T>
	inline bool driver::write(uint64_t address, const T &value)
	{
		return write_process_memory(address, (uint64_t)&value, sizeof(T));
	}

	template <typename T>
	inline bool driver::read_array(uint64_t address, T *array, size_t len)
	{
		return read_process_memory(address, (uintptr_t)array, sizeof(T) * len);
	}

	template <typename T>
	inline bool driver::write_array(uint64_t address, const T *array, size_t len)
	{
		return write_process_memory(address, (uintptr_t)array, sizeof(T) * len);
	}
}
