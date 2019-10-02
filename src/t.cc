#include <memory>
#include <cstddef>

struct Foreign;

extern "C" {
  void AddRef(Foreign*);
  void Release(Foreign*);
}

template <typename T>
class already_AddRefed {
  T* mRawPtr;

 public:
  already_AddRefed(const already_AddRefed&) = delete;

  already_AddRefed(already_AddRefed&& aOther) {
    T* tmp = aOther.mRawPtr;
    aOther.mRawPtr = mRawPtr;
    mRawPtr = tmp;
  }

  already_AddRefed() : mRawPtr(nullptr) {}
  already_AddRefed(decltype(nullptr)) : mRawPtr(nullptr) {}
  already_AddRefed(T* aPtr) : mRawPtr(aPtr) {}

  T* take() {
    T* ptr = mRawPtr;
    mRawPtr = nullptr;
    return ptr;
  }
};

template<typename F>
struct GenericFoo {
  const F* bar;

  already_AddRefed<F> Consume() {
    return already_AddRefed<F>(const_cast<F*>(bar));
  }
};

template <typename T>
class RefPtr {
  T* mRawPtr;

  void assign_assuming_AddRef(T* aNewPtr) {
    T* oldPtr = mRawPtr;
    mRawPtr = aNewPtr;
    if (oldPtr) {
      Release(oldPtr);
    }
  }

  void unref() {
    if (mRawPtr) {
      Release(mRawPtr);
    }
  }

  void ref() {
    if (mRawPtr) {
      AddRef(mRawPtr);
    }
  }

 public:
  RefPtr() : mRawPtr(nullptr) {}

  RefPtr(already_AddRefed<T> aPtr) : mRawPtr(aPtr.take()) {}

  RefPtr(T* aPtr) : mRawPtr(aPtr) {
    ref();
  }

  template <typename I>
  RefPtr& operator=(already_AddRefed<I>&& aRhs) {
    assign_assuming_AddRef(aRhs.take());
    return *this;
  }

  T* get() {
    return mRawPtr;
  }

  already_AddRefed<T> forget() {
    T* ptr = mRawPtr;
    mRawPtr = nullptr;
    return already_AddRefed<T>(ptr);
  }

  ~RefPtr() {
    unref();
  }
};

struct Foreign;

struct Foo {
  const Foreign* bar;
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

void do_something_internal(RefPtr<Foreign>& aResult) {
  aResult = Servo_GetGenericFoo((void*)0x2, nullptr, nullptr, IterationCompositeOperation::Bar, 32.0, 5).Consume();
}

extern "C" int do_something() {
  auto foo = Servo_GetFoo((void*)0x1);
  printf("%p\n", foo.bar);
  RefPtr<Foreign> result((Foreign*)0x3);
  do_something_internal(result);
  printf("%p\n", result.get());
  return 0;
}

int main() {
  return Servo_DoSomething();
}
