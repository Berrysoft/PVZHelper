#pragma once

template <class T>
ref class cli_ptr
{
private:
    T* ptr;

    void finalize()
    {
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }

public:
    cli_ptr() : ptr(nullptr) {}
    cli_ptr(T* ptr) : ptr(ptr) {}

    ~cli_ptr() { finalize(); }
    !cli_ptr() { finalize(); }

    T* operator->() { return ptr; }
    auto operator&() { return &ptr; }
};

template <typename T, typename D = T, typename... Args>
cli_ptr<D>^ make_cli(Args&&... args)
{
    return gcnew cli_ptr<D>(new T(std::forward<Args>(args)...));
}
