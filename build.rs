fn main() {
    // Every cpp file must be included by calling file().
    cxx_build::bridge("src/lib.rs")
        .file("cpp/driver.cpp")
        .flag("/std:c++17")
        .compile("driver-rs");

    // Every header and cpp file must be included here.
    println!("cargo:rerun-if-changed=src/lib.rs");
}
