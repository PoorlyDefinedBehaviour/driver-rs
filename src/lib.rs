/// Pointers are mut even when they are not meant to be mutated
/// so the generated c code uses a *T instead of a const *T.
#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("driver-rs/cpp/rust_bindings.hpp");

        type Driver;

        type c_uint32_t;
        type c_wchar_t;
        type c_uintptr_t;
        type c_size_t;

        fn driver_init() -> bool;

        unsafe fn get_module_base(proc_id: *mut c_uint32_t, mod_name: *const c_wchar_t) -> usize;

        unsafe fn read_process_memory(
            target_proc_id: *mut c_uint32_t,
            read_address: *mut c_uintptr_t,
            buffer_address: *mut c_uintptr_t,
            size: *mut c_size_t,
        ) -> bool;

        unsafe fn write_process_memory(
            target_proc_id: *mut c_uint32_t,
            write_address: *mut c_uintptr_t,
            buffer_address: *mut c_uintptr_t,
            size: *mut c_size_t,
        ) -> bool;

        unsafe fn virtual_query(
            proc_id: *mut c_uint32_t,
            address: *mut c_uintptr_t,
            buffer_address: *mut c_uintptr_t,
            size: *mut c_size_t,
        ) -> bool;

        unsafe fn get_mapped_file_name(
            proc_id: *mut c_uint32_t,
            address: *mut c_uintptr_t,
            buffer: *const c_wchar_t,
            size: *mut c_size_t,
        ) -> bool;

        fn test_communicate() -> bool;

        unsafe fn read_kernel_memory(
            read_address: *mut c_uintptr_t,
            buffer_address: *mut c_uintptr_t,
            size: *mut c_size_t,
        ) -> bool;
    }
}

pub use ffi::*;

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn debug() {
        // dbg!('a');
        dbg!(driver_init());
        dbg!(test_communicate());
    }
}
