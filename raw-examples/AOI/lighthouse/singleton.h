#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
public:
    static T* getInstance() {
        if (instance == nullptr) {
            instance = new T();
        }
        return instance;
    }
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator = (const Singleton&) = delete;
    Singleton& operator = (Singleton&&) = delete;
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
private:
    static T* instance;
};

template <typename T>
T* Singleton<T>::instance = nullptr;

#endif // SINGLETON_H
