use std::marker::PhantomData;
use std::ffi::c_void;
use std::os::raw::c_int;

pub struct Foreign {
    address: u8,
}

#[repr(C)]
pub struct GenericFoo<F> {
    ptr: *const F,
    _marker: PhantomData<F>,
}

#[repr(C)]
pub struct Foo {
    ptr: *const Foreign,

}

#[no_mangle]
pub extern "C" fn Servo_GetFoo(ptr: *mut c_void) -> Foo {
    println!("{:?}", ptr);
    Foo {
        ptr: ptr as *const _,
    }
}

#[no_mangle]
pub extern "C" fn Servo_GetGenericFoo(ptr: *mut c_void) -> GenericFoo<Foreign> {
    println!("{:?}", ptr);
    GenericFoo {
        ptr: ptr as *const _,
        _marker: PhantomData,
    }
}

extern "C" {
    fn do_something() -> c_int;
}

fn main() {
    let out = unsafe { do_something() };
    assert_eq!(out, 0);
}
