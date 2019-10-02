#include <memory>
#include <cstddef>

struct Foreign;

struct Foo {
  const Foreign* bar;
};

template<typename F>
struct GenericFoo {
  const F* bar;

  std::unique_ptr<F> Consume();
};

enum class IterationCompositeOperation : unsigned char {
    Foo,
    Bar
};

extern "C" {
  int Servo_DoSomething();
  Foo Servo_GetFoo(void*);
  GenericFoo<Foreign> Servo_GetGenericFoo(void*, const Foreign*, const Foreign*, IterationCompositeOperation, double, uint64_t);
}

extern "C" int do_something() {
  auto foo = Servo_GetFoo((void*)0x1);
  printf("%p\n", foo.bar);
  auto genericFoo = Servo_GetGenericFoo((void*)0x2, nullptr, nullptr, IterationCompositeOperation::Bar, 32.0, 5);
  printf("%p\n", genericFoo.bar);
  return 0;
}

int main() {
  return Servo_DoSomething();
}
