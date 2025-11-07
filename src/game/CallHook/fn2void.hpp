#ifndef FN2VOID_H
#define FN2VOID_H

template<typename R, typename... Args>
void* fn2void(R(*fn)(Args...)) {
    union {
        R(*fn)(Args...);
        void* ptr;
    } converter;
    converter.fn = fn;
    return converter.ptr;
}

#ifdef WIN32
template<typename R, typename... Args>
void* fn2void(R(__stdcall* fn)(Args...)) {
#else
template<typename R, typename... Args>
void* fn2void(R(__attribute__((stdcall)) * fn)(Args...)) {
#endif
    union {
        R(__stdcall* fn)(Args...);
        void* ptr;
    } converter;
    converter.fn = fn;
    return converter.ptr;
}

template<typename R, class C, typename... Args>
void* fn2void(R(C:: * fn)(Args...)) {
    union {
        R(C::* fn)(Args...);
        void* ptr;
    } converter;
    converter.fn = fn;
    return converter.ptr;
}

#endif // FN2VOID_H