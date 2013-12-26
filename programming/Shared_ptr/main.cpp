#include <mutex>
#include <cstddef>
#include <memory>
#include <algorithm>
#include <iostream>
using namespace std;
template <typename T>
class  ref_count
{
    T *p; 
    unsigned int count;
    mutex mtx; 
public:
    ref_count(void)
    {
        p = static_cast<T*>(NULL);
        count = 0;
    };
   explicit ref_count(T *pp):p(pp), count (1) 
    {
    };
    void inc_count(void)
    {
        if (p != NULL)
        { 
            mtx.lock();
            count ++;
            mtx.unlock();
        } 
    };
    void dec_count(void)
    {
        if (p != NULL)
        {
           mtx.lock();
           if (count-- == 1)
           {
               delete p;
               p = static_cast<T*>(NULL);
           } 
           mtx.unlock();
        }
    };
    unsigned int use_count(void) const
    {
       return count;
    };
    virtual ~ref_count()
    {
        std::cout << "ref_count destructor invoekd " << std::endl;
        if (p != NULL)
            delete p;
    }; 
};

template <typename T>
class Shared_ptr
{
    mutable ref_count<T> *ref;   
    T *p;
public:
    Shared_ptr(void)
    {
        ref = new ref_count<T>();
        p = static_cast<T*>(NULL);
    }; 
    Shared_ptr(T *pp)
    {
        ref = new ref_count<T> (pp);
        p = pp;
        
    };
    unsigned int use_count(void) const
    {
        return ref->use_count();
    }; 
    Shared_ptr(const Shared_ptr<T> &sp)
    {
        if (sp.use_count() == 0)
        {
            Shared_ptr();
            return;
        }
        ref = sp.ref;
        ref->inc_count();
        p = sp.p;
        
    };
    operator bool (void) const  noexcept
    {
        return use_count () != 0; 
    };
    T & operator * (void) const
    {
        return *p;
    }; 
    T * operator -> (void) const
    {
        return p;
    };
    void reset(void)
    {
       p = static_cast<T*>(NULL); 
       ref->dec_count(); 
       if (use_count() != 0)
       {
           ref = new ref_count <T>();
       } 
    };
    Shared_ptr & operator = (const Shared_ptr & sp) noexcept
    {
        reset();
        if (sp.use_count() == 0)
        {
           return *this;  
        }
        delete ref; 
        p = sp.p;
        ref = sp.ref; 
        ref->inc_count();
        return *this;
    }; 
    template<typename U> Shared_ptr & operator = (const Shared_ptr<U> & sp) noexcept
    {
        reset();  
        if (sp.use_count () == 0)
            return *this;
        delete ref;
        p = dynamic_cast<T*>(sp.p);
        ref = reinterpret_cast<Shared_ptr<T> *> (sp.ref);
        return *this;  
    };
    bool expired (void)const
    {
        return use_count () == 0; 
    };
    bool unique (void) const
    {
        return use_count() == 1;
    };
    void swap(Shared_ptr & sp)
    {
        std::swap(sp.ref, ref); 
        std::swap(sp.p, this->p);
    };  
    T * get(void) const
    {
        return p;
    }; 
    virtual ~Shared_ptr()
    {
        if (ref->use_count() == 1)
        {
            delete ref;
            return;
        }
        ref->dec_count();
    };
private:
  
};
template <class charT, class traits, class U >
inline std::basic_ostream<charT,traits>& operator<<( std::basic_ostream<charT,traits>& os, const Shared_ptr<U>& x)
{
    return os << x.get();
}
 #include <iostream>
#include <type_traits>

typedef std::add_pointer<int>::type A;        // int*
typedef std::add_pointer<const int>::type B;  // const int*
typedef std::add_pointer<int&>::type C;       // int*
typedef std::add_pointer<int*>::type D;       // int**
typedef std::add_pointer<int(int)>::type E;   // int(*)(int)

class base
{
public:
 int i; 
 
};
class derived: public base
{
};

int main() {
  Shared_ptr<int> sp(new int(3));
  Shared_ptr<int> sp2(sp);
  *sp2 = 4;
  std::cout << *sp << std::endl;
  std::cout << "use_count " << sp.use_count();
  sp2.reset();
  std::cout << "use count " << sp.use_count() << std::endl;
  std::cout << "**********" << std::endl;
  Shared_ptr<base> spb(new base);
  spb->i = 0;
  Shared_ptr<base> spd(new derived);
  spd->i = 4;
  spb = spd;
  
  return 0;
}
