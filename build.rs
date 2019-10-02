use std::process::Command;
use std::path::{Path, PathBuf};

fn main() {
    /*
    cc::Build::new()
        .cpp(true)
        .file("src/t.cc")
        .compile("libt.a");
    */

    let out_dir = std::env::var_os("OUT_DIR").unwrap();
    let obj = PathBuf::from(&out_dir).join("t.o");
    Command::new("g++")
        .arg("-m32")
        .arg("-fPIE")
        .arg("-O3")
        .arg("-c")
        .arg("-o")
        .arg(&obj)
        .arg("src/t.cc")
        .spawn()
        .unwrap()
        .wait()
        .unwrap();

    let lib = PathBuf::from(&out_dir).join("libt.a");
    Command::new("ar")
        .arg("rcs")
        .arg(&lib)
        .arg(&obj)
        .spawn()
        .unwrap()
        .wait()
        .unwrap();

    println!("cargo:rustc-link-search={}", out_dir.to_string_lossy());
    println!("cargo:rustc-link-lib=t");
}
