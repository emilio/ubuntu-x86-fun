.PHONY: all
all: cpp-rust-dep

cpp-rust-dep: rust-no-features src/t.cc
	$(CC) -o target/t-cpp -m32 src/t.cc -L target/i686-unknown-linux-gnu/release -lt -O3 -flto -fno-exceptions -lpthread -ldl
	./target/t-cpp


.PHONY: rust-no-features
rust-no-features:
	cargo rustc --release -vvv --target=i686-unknown-linux-gnu -- -Clto
