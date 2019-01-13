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

    template <typename D>
    static operator cli_ptr<T>^(cli_ptr<D>^ p)
	{
        cli_ptr<T>^ result = gcnew cli_ptr<T>(*&p);
        *&p = nullptr;
        return result;
    }
};

template <typename T, typename... Args>
cli_ptr<T>^ make_cli(Args&&... args)
{
    return gcnew cli_ptr<T>(new T(std::forward<Args>(args)...));
}
