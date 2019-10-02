fn main() {
    cc::Build::new()
        .cpp(true)
        .file("src/t.cc")
        .compile("libt.a");
}
