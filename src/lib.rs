/// Pointers are mut even when they are not meant to be mutated
/// so the generated c code uses a *T instead of a const *T.
#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("driver-rs/cpp/rust_bindings.hpp");

        type Driver;

        fn driver_init() -> bool;

        fn get_module_base(proc_id: u32, mod_name: &CxxString, buffer: &mut u64) -> bool;

        unsafe fn read_process_memory(
            target_proc_id: u32,
            read_address: u64,
            buffer_address: u64,
            size: u64,
        ) -> bool;

        unsafe fn write_process_memory(
            target_proc_id: u32,
            write_address: u64,
            buffer_address: u64,
            size: u64,
        ) -> bool;

        unsafe fn virtual_query(proc_id: u32, address: u64, buffer_address: u64, size: u64)
            -> bool;

        unsafe fn get_mapped_file_name(
            proc_id: u32,
            address: u64,
            buffer_address: u64,
            size: u64,
        ) -> bool;

        fn test_communicate() -> bool;

        unsafe fn read_kernel_memory(read_address: usize, buffer_address: usize, size: u64)
            -> bool;
    }
}

pub use ffi::*;

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn debug() {
        dbg!('a');
        dbg!(driver_init());
        dbg!(test_communicate());
    }
}
