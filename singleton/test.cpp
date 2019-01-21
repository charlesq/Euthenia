#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <typeinfo>
#include <memory>
#include <vector>
#if 0
template<typename T>
class Singleton1
{
private:
static T * instance;

public:
static T * getInstance(void)
    {
        if (instance == nullptr)
           instance = new T();
        return instance;
    }
protected:

    Singleton1(void) 
    {
        std::cout << typeid(Singleton1<T>).name() << std::endl;
    }
    virtual ~Singleton1() = default;
    Singleton1(const Singleton1 &) = delete;
    Singleton1 & operator = (const Singleton1 &) = delete;

};

template<typename T>
T* Singleton1<T>::instance = nullptr;

class A: public Singleton1<A>
{
public:
  void print(void)
  {
    std::cout << typeid(A).name() << std::endl;
  } 
private:
    virtual~A() {};
protected:
    A() = default; 
//friend class Singleton1<A>;
};


template <typename T>
class Singleton2
{
private:
    static std::mutex mtx;
    static T * instance;
public:
    static T *getInstance(void)
    {
        std::lock_guard<std::mutex> _l(mtx);
        if (instance == nullptr)
            instance = new T();
        return instance;
    }
protected:
    Singleton2() = default;
    Singleton2(Singleton2 const &) = delete;
    Singleton2 & operator = (const Singleton2 &) = delete;
    virtual ~Singleton2() = default;
    
};

template<typename T>
std::mutex Singleton2<T>::mtx;
template<typename T>
T * Singleton2<T>::instance = nullptr;

class B:public Singleton2<B>
{
public:
   void print (void)
   {
      static std::mutex mtx; 
      std::lock_guard<std::mutex> _l(mtx);
      std::cout <<std::hex << std::this_thread::get_id() << ": " <<  std::hex << this<< std::endl;
   }
private:
    B() = default;
    ~B() = default;       
    
friend class Singleton2<B>;
friend class std::unique_ptr<B>;
};

template <typename T>
class Singleton3
{
private:
    static std::atomic<T*> instance;
    static std::mutex mtx; 
public:
    static T * getInstance(void)
    {
        T* temp = instance.load();
        if (temp == nullptr)
        {
            std::lock_guard<std::mutex> _l(mtx);
            if (instance.load() == nullptr)
            {
               temp = new T();
               instance.store(temp);
            }
        }
        return temp;

    }
protected:
    Singleton3() = default;
    Singleton3(const Singleton3 &) = delete;
    Singleton3& operator = (const Singleton3 &) = delete;
    virtual ~Singleton3() = default;


};


template<typename T>
std::atomic<T*> Singleton3<T>::instance; 
template<typename T>
std::mutex Singleton3<T>::mtx;

class C:public Singleton3<C>
{
public:
    void print(void)
    {
        static std::mutex m;
        std::lock_guard<std::mutex> _l(m);
        std::cout << std::hex << std::this_thread::get_id() << ": "<< this << std::endl;
    
    }
private:
    C() {};
    ~C(){};
friend class Singleton3<C>; 
};


template <typename T>
class Singleton4
{
    static std::mutex mtx;
    static std::atomic<T*> instance;
public:
    static T* getInstance(void)
    {
        T * tmp = instance.load(std::memory_order_acquire);
        if (!tmp)
        {
            std::lock_guard<std::mutex> _l(mtx);
            tmp = instance.load(std::memory_order_relaxed);
            if (tmp == nullptr)
            {
                tmp = new T();
                instance.store(tmp, std::memory_order_release);
            }
        }
        return tmp;
    }
protected:
    Singleton4() = default;
    virtual ~Singleton4() = default;
    Singleton4(const Singleton4 &) = delete;
    Singleton4 & operator = (const Singleton4 &) = delete;
};


template<typename T>
std::mutex Singleton4<T>::mtx;
template <typename T>
std::atomic<T*> Singleton4<T>::instance;

class D:public Singleton4<D>
{
public:
    void print(void) 
    {
        static std::mutex mtx;
        std::lock_guard<std::mutex> _l(mtx);
        std::cout << std::hex << std::this_thread::get_id() << ": " << this << std::endl;
    }
private:
    D() = default;
    ~D() = default;
friend class Singleton4<D>;    
};
#else
template <typename T>
class Singleton5
{

private:
    static std::once_flag flag;
    static T *instance;
public:
    static T *getInstance()
    {
        std::call_once(flag, [] (void) { instance =  new T(); });
        return instance;
    }


protected:
    Singleton5() = default;
    virtual ~Singleton5() = default;
    Singleton5(const Singleton5 &) = delete;
    Singleton5& operator = (const Singleton5 &) = delete;
};

template<typename T>
std::once_flag Singleton5<T>::flag;

template<typename T>
T* Singleton5<T>::instance = nullptr;

class E:public Singleton5<E>
{
public:
    void print(void)
    {
        static std::mutex mtx;
        std::lock_guard<std::mutex> _l(mtx);
        std::cout << std::hex << std::this_thread::get_id() << ": " << this << std::endl; 

    }
protected:
private:
    E() = default;
    ~E() = default;
friend class Singleton5<E>;
     
};
#endif
int main()
{
    E* b = E::getInstance();
    std::vector<std::thread> v;   
    for (int i = 0; i < 20; ++i)
    {
        v.push_back(std::thread(&E::print, E::getInstance()));
    }
    for (auto &e: v)
       e.join();
    return 0;

}



