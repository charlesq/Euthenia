#ifndef __BIG_NUMBER_HPP__
#define __BIG_NUMBER_HPP__
#include <vector>
#include <iomanip>
#include <iostream>
class big_number
{
    std::vector<unsigned long> _s;
public:
    big_number(void) {};
    big_number(unsigned long n)
    {
        _s.push_back(n);
    };
    big_number (const big_number &bn)
    {
        _s = bn._s;
    };
    big_number & operator = (unsigned long n)
    {
        this->_s.clear();
        this->_s.push_back(n);
        return *this;
    };
    big_number & operator = (const big_number &bn)
    {
        if (this == &bn)
            return *this;
        _s.clear();
        for (auto it = bn._s.cbegin(); it != bn._s.cend(); it ++)
           _s.push_back(*it);
        return *this;
    };
    big_number operator ++ (int)
    {
        big_number bn = *this; 
        *this += 1;
        return bn; 
    };
    big_number & operator ++ (void)
    {
        *this += 1;
        return *this;
    };
    big_number operator -- (int n)
    {
        big_number bn = *this;
        *this -= 1;
        return bn;
    };
    big_number &operator -- (void)
    {
       *this -= 1;
       return *this;
    };
    friend big_number operator + (const big_number &bn,  unsigned long n)
    {
        big_number sum;
        sum._s.resize(bn._s.size() + 1, 0);
        if (bn._s.size() == 0)
        {
            sum._s.push_back(n);
            return bn;
        }
        unsigned long long s = n;
        unsigned int i = 0;
        unsigned long sl = -1;
        while (i < bn._s.size())
        {
           s += bn._s[i];
           sum._s[i++] = s & sl;
           s >>= sizeof(unsigned long) * 8;
        }
        if ( s!= 0)
            sum._s[i] = s;
        else sum._s.pop_back();
        return sum;
    };
    friend big_number & operator += (big_number &bn, unsigned long n)
    {
        unsigned long long s = n;
        unsigned int i = 0;
        unsigned long m = -1;
        while (i < bn._s.size())
        {
             s += bn._s[i];
             bn._s[i++] = s & m;
             s >>= sizeof(unsigned long) * 8;
        }
        if (s != 0)
            bn._s.push_back(s);
        return bn;
    };
    friend big_number & operator += (big_number &bn1, const big_number &bn2)
    {
        unsigned long long s = 0;
        unsigned long m = -1;
        unsigned int i = 0;
        while (i < bn1._s.size() && i < bn2._s.size())
        {
            s += bn1._s[i] + bn2._s[i];
            bn1._s[i++] = s & m;
            s >>= sizeof(unsigned long) * 8;
        }
        while (i < bn2._s.size())
        {
            s+= bn2._s[i++];
            bn1._s.push_back(s & m);
            s >>= sizeof (unsigned long) * 8;
        }
        while (i < bn1._s.size())
        {
            s+= bn1._s[i];
            bn1._s[i++] = s  & m;
            s >> sizeof(unsigned long) * 8;
        }
        if (s > 0)
            bn1._s.push_back(s & m);
        return bn1;
    };
    friend big_number operator + (unsigned long n, const big_number &bn)
    {
       return bn + n;
    };
    friend big_number operator + (const big_number &bn1, const big_number &bn2)
    {
        unsigned long long s;
        unsigned long m = -1;
        big_number sum;
        sum._s.resize((bn1._s.size() > bn2._s.size())? bn1._s.size(): bn2._s.size());
        s = 0;
        unsigned int i = 0;
        while(i < bn1._s.size() && i < bn2._s.size())
        {
            s = bn1._s[i] + bn2._s[i];
            sum._s[i++] = s & m;
            s >>= sizeof(unsigned long) * 8;
        }
        const big_number &r = (bn1._s.size() > bn2._s.size())? bn1: bn2;
        while(i < r._s.size())
        {
            s += r._s[i];
            sum._s[i++] = s & m;
            s >>= sizeof(unsigned long) * 8;
        }
        if (s > 0)
            sum._s.push_back(s);
        return sum;
    };
    friend big_number operator - (const big_number &bn1 , const big_number &bn2)
    {
        if (bn1 < bn2)
            return bn2 - bn1;
        big_number dif;
        dif._s.resize(bn1._s.size(), 0);
        long b = 0;
        unsigned long long o1, o2;
        unsigned int i = 0;
        unsigned long m = -1;
        while(i < bn2._s.size())
        {
            o1 = bn1._s[i];
            o2 = bn2._s[i];
            if (o1 > o2 || (o1 == o2 && b == 0))
            {
                dif._s[i++] = o1 - o2 -b;
                b = 0;
                continue; 
            }
            o1 += m ;
            dif._s[i++] = (o1 -b) - o2;  
            b = 1;
        }
        while (i < bn1._s.size())
        {
           if (bn1._s[i] != 0 || b == 0)
           { 
               dif._s[i++] =  bn1._s[i] - b;
               b = 0;
               continue;
           }
           dif._s[i++] = m; 
           b = 1; 
        }
        dif._consolidate();
        return dif;
        
    };
    friend big_number operator - (const big_number bn, unsigned long n)
    {
        big_number bn1 = n;
        return bn - bn1;
    }; 
    friend big_number operator - (unsigned long n, const big_number &bn)
    {
        big_number bn1 = n;
        return bn1 - bn;
    };
    friend big_number operator % (big_number bn1, const big_number &bn2)
    {
        if (bn2 == 0)
            return big_number(0);
        while (bn1 >= bn2)
        {
            bn1 -= bn2;
        }
        return bn1;

    };
    friend big_number operator % (big_number bn, unsigned long n)
    {
        if (n == 0)
            return big_number(0);
        while (bn >= n)
        {
            bn -= n;
        }
        return bn;
    };
    friend big_number &operator %= (big_number &bn, unsigned long n)
    {
        if (n == 0)
        {
            bn._s.clear();
            return bn;
        }
        while (bn >= n)
        {
            bn -= n;
        }
        return bn;   
    };
    friend big_number &operator %= (big_number &bn1, const big_number &bn2)
    {
        if (bn2 == 0)
        {
            bn1._s.clear();
            return bn1;
        }
        while (bn1 >= bn2)
        {
            bn1 -= bn2;
        }
        return bn1;
    };
    friend big_number operator * (const big_number &bn1, const big_number &bn2)
    {
        if (bn1 == 0 || bn2 == 0)
            return bn1 == 0? bn1: bn2;
        if (bn1 < bn2)
            return bn2 * bn1;
        unsigned long long p = 0;
        unsigned long carry = 0;
        unsigned int i = 0, j = 0, k = 0;
        big_number bp;
        unsigned long m = -1;
        bp._s.resize(bn1._s.size() * bn2._s.size(),0);
        while (i < bn2._s.size())
        {
            while (j < bn1._s.size())
            {
                p = bn1._s[j] * bn2._s[i];
                p += carry;
                bp._s[k + j]+= p & m;
                carry = p >> sizeof(unsigned long ) * 8;
                j ++;
            }
            if (carry != 0)
                bp._s[j + k] += carry;
            k ++;
            i ++;
        }
        bp._consolidate();
       return bp;
    };
    friend big_number operator * (const big_number bn, unsigned long n)
    {
        big_number bn1 = n;
        return bn * bn1;
    };
    friend big_number operator *= (big_number &bn, unsigned long n)
    {
        bn = bn * n;
        return bn;
    };
    friend big_number operator *= (big_number &bn1, const big_number &bn2)
    {
       bn1 = bn1 * bn2;
       return bn1; 
    };
    friend big_number operator / (big_number bn, unsigned long n)
    {
        if (n == 0)
        {
            if (bn._s.size() == 0)
                return 1;
            return 1/n;
        }
        
        big_number  q = 0;
        unsigned int i = 0;
        while(bn >= n)
        {
            q ++;
            bn -= n;
      
        }
        return q;
    };
    friend big_number exponent(const big_number &base, unsigned long n)
    {
        big_number power;
        if (base == 0)
            return n==0? 1: power;  
        power = 1;
        while (n-- > 0)
           power *= base; 
        return power;
    };
    friend big_number exponent (const big_number &base, big_number bn)
    {
        big_number power;
        if (base == 0)
            return bn == 0? 1: power; 
        power = 1;
        while (bn-- > 0)
        {
            power *= base;
        }
        return power; 
    };
    friend big_number operator >> (big_number bn1, big_number bn2)
    {
        if (bn2._s.size() == 0)
            return bn1;
        unsigned long remainder = 0, interim = 0;; 
        while (bn2 >= 32)
        {
            bn1._s.erase(bn1._s.begin());
            bn2 -= 32;
        }
        unsigned long i = 0;
        while(bn2-- > 0)
        { 
            if (bn1 == 0)
                return bn1;
             
            for (i = bn1._s.size() -1; i < bn1._s.size(); i --)
            {
                interim =  bn1._s[i];
                bn1._s[i] >>= 1;
                if (remainder)
                {
                    bn1._s[i] |= 1 << 31; 
                } 
                if (interim & 1 > 0)
                    remainder =1;
                else
                    remainder = 0; 
            }
       }
       bn1._consolidate();
       return bn1;
    };
    friend big_number operator >> (big_number bn, unsigned long n)
    {
        big_number bn1 = n;
        return bn >> bn1;
    };
    friend big_number operator >>= (big_number &bn1, big_number bn2)
    {
        if (bn2._s.size() == 0)
            return bn1;
        unsigned long remainder = 0, interim = 0;;
        while (bn2 >= 32)
        {
            bn1._s.erase(bn1._s.begin());
            bn2 -= 32;
        }
        unsigned long i = 0;
        while(bn2-- != 0)
        {
            if (bn1 == 0)
                return bn1;

            for (i = bn1._s.size() -1; i < bn1._s.size(); i --)
            {
                interim =  bn1._s[i];
                bn1._s[i] >>= 1;
                if (remainder)
                {
                    bn1._s[i] |= 1 << 31;
                }
                if (interim & 1 > 0)
                    remainder =1;
                else
                    remainder = 0;
            }
       }
       bn1._consolidate();
       return bn1; 
    };
    friend big_number operator >>= (big_number &bn, unsigned long n)
    {
        big_number bn1 = n;
        bn >>= bn1;
        return bn;
    };
    friend big_number operator << (big_number bn1, big_number bn2)
    {
        unsigned int i = 0, carry = 0, interim = 0; 
        while(bn2 >= 32)
        {
            bn1._s.insert(bn1._s.begin(), 0);
            bn2 +- 32;
        }
        while (bn2 -- != 0)
        {
            for (i = 0; i < bn1._s.size(); i ++)
            {
                interim = bn1._s[i]; 
                bn1._s[i] <<= 1;
                if (carry > 0)
                    bn1._s[i] |= carry;
                if ((interim & 0x80000000) > 0)
                    carry = 1;
                else
                    carry = 0;  
            }
        }
        if (carry > 0)
            bn1._s.push_back(1);
    };
    friend big_number operator << (big_number bn, unsigned long n)
    {
        big_number bn1 = n;
        return bn << bn1;
    };
    friend big_number operator <<= (big_number &bn1, big_number &bn2)
    {
        bn1 = bn1 << bn2;
    };
    friend big_number operator <<= (big_number &bn, unsigned long n)
    {
        big_number bn1 = n;
        bn <<= bn1;
        return bn;
    }; 
    friend big_number  operator ~ (big_number bn)
    {
        for (auto it = bn._s.begin(); it != bn._s.end(); it ++)
            *it = ~(*it);
        return bn;
    }; 
    friend big_number operator ^ (const big_number &bn1, const big_number &bn2)
    {
        big_number ret;
        if (bn1._s.size() == 0 && bn2._s.size() ==0)
            return ret;
        if (bn1._s.size() == 0 || bn2._s.size() ==0)
            return bn1._s.size() == 0? ~bn2: ~bn1;
        ret._s.resize(bn1._s.size() > bn2._s.size()? bn1._s.size(): bn2._s.size());
        unsigned int i = 0;
        while (i < bn1._s.size() && i < bn2._s.size())
        {
            ret._s[i] = bn1._s[i] ^ bn2._s[i++];
        }
        const big_number &bn = bn1._s.size() > bn2._s.size()? bn1: bn2;
        while(i < bn._s.size())
        {
            ret._s[i] = bn._s[i++];
        }
        return ret;
    };
    friend big_number & operator ^= (big_number &bn1, const big_number &bn2)
    {
        if (bn1._s.size() < bn2._s.size())
            bn1._s.resize(bn2._s.size(), 0);
        unsigned int i = 0;
        while(i < bn1._s.size())
        {
            bn1._s[i] ^= bn2._s[i];
        }
        return bn1;
    };
    friend big_number operator ^ (const big_number &bn, unsigned long n)
    {
        big_number bn1 = n;
        return bn1 ^ bn;

    };
    friend big_number operator | (big_number bn, unsigned long n)
    {
        if (bn._s.size() == 0)
            return big_number(n);
        bn._s[0] |= n;
        return bn;
    };
    friend big_number operator | (big_number bn1, const big_number &bn2)
    {
        unsigned long i = 0;
        while(i < bn1._s.size() && i < bn2._s.size())
        {
            bn1._s[i] |= bn2._s[i++];
        }
        while (i < bn2._s.size())
            bn1._s.push_back(bn2._s[i++]);
        return bn1;
    };
    friend big_number & operator |= (big_number &bn, unsigned long n)
    {
        if (bn._s.size() == 0)
        {
            bn = n;
            return bn;
        }
        bn._s[0] |= n;
        return bn;
    };
    friend big_number  &operator |= (big_number &bn1, const big_number &bn2)
    {
        unsigned long i = 0;
        while(i < bn1._s.size() && i < bn2._s.size())
        {
            bn1._s[i] |= bn2._s[i++];
        }
        while (i < bn2._s.size())
            bn1._s.push_back(bn2._s[i++]);
        return bn1;
    }; 
    friend big_number operator & (big_number bn, unsigned long n)
    {
        if (bn._s.size() == 0)
            return big_number(0);
        bn._s[0] &= n;
        return bn;
    };
    friend big_number operator & (big_number bn1, const big_number &bn2)
    {
        unsigned long i = 0;
        while(i < bn1._s.size() && i < bn2._s.size())
        {
            bn1._s[i] &= bn2._s[i++];
        }
        return bn1;
    };
    friend big_number  &operator &= (big_number &bn1, const big_number &bn2)
    {
        unsigned long i = 0;
        while(i < bn1._s.size() && i < bn2._s.size())
        {
            bn1._s[i] |= bn2._s[i++];
        }
        return bn1;
    };
    friend big_number &operator &= (big_number &bn, unsigned long n)
    {
        if (bn._s.size() == 0)
        {
            bn._s.clear();
            return bn;
        }
        bn._s[0] &= n;
        return bn;
    };


    friend big_number operator ^= (big_number &bn, unsigned long n)
    {
        big_number bn1 = n;
        return bn ^= bn1;
    };
    friend big_number operator -= (big_number &bn1, const big_number &bn2)
    {
        bn1 = bn1 - bn2;
        return bn1;
    };
    friend inline bool operator == (const big_number &bn, unsigned long n)
    {
        return (bn._s.size() == 1 && n == bn._s.front());
    };
    friend inline bool operator != (const big_number &bn, unsigned long n)
    {
        return ! (bn == n);
    };
    friend inline bool operator > (const big_number &bn,  unsigned long n)
    {
        if(bn._s.size() > 1)
            return true;
        if (bn._s.size() == 0)
            return false;
        return bn._s.front() > n ;

    };
    friend inline bool operator >= (const big_number &bn, unsigned long n)
    {
        return bn > n || bn == n;
    };
    friend inline bool operator < (const big_number bn, unsigned long n)
    {
        return !(bn >= n);
    };
    friend inline bool operator <= (const big_number &bn, unsigned long n)
    {
        return !(bn > n);
    };
    friend inline bool operator == (const unsigned long n, const big_number &bn)
    {
        return bn == n;
    };
    friend inline bool operator != (unsigned long n, const big_number &bn)
    {
        return bn != n;
    };
    friend inline bool operator > (unsigned long n, const big_number &bn)
    {
        return bn < n;
    };
    friend inline bool operator <= (unsigned long n, const big_number &bn)
    {
        return bn >= n;
    };
    friend inline bool operator >= (unsigned long n, const big_number &bn)
    {
        return bn <= n;
    };
    friend inline bool operator < (unsigned long n, const big_number &bn)
    {
        return bn > n;
    };
    friend inline bool operator == (const big_number &bn1, const big_number &bn2)
    {
        if (bn1._s.size() != bn2._s.size()) return false;
        for (auto i1 = bn1._s.cbegin(), i2 = bn2._s.cbegin(); i1 != bn1._s.cend(); i1 ++, i2 ++)
        {
            if (*i1 == *i2)
                continue;
            return false;
        }
            return true;
    };
    friend inline bool operator != (const big_number &bn1, const big_number &bn2)
    {
        return !(bn1 == bn2);
    };
    friend bool operator > (const big_number &bn1, const big_number &bn2)
    {
        if (bn1._s.size() == 0 && bn2._s.size() == 0)
            return false;
        if (bn1._s.size() != bn2._s.size())
            return bn1._s.size() > bn2._s.size();
        for (auto i1 = bn1._s.cend() -1, i2 = bn2._s.cend() -1; i1 >= bn1._s.cbegin(); i1--, i2--)
        {
            if (*i1 == *i2)
                continue;
            return *i1 > *i2;
        }
        return false;
    };
    friend bool operator >= (const big_number &bn1, const big_number &bn2)
    {
        return bn1 == bn2 || bn1 > bn2;
    };
    friend bool operator < (const big_number &bn1, const big_number &bn2)
    {
        return !(bn1 >= bn2);
    };
    friend bool operator <= (const big_number &bn1 , const big_number &bn2)
    {
        return !(bn1 > bn2);
    };
    friend bool operator ! (const big_number &bn)
    {
        return bn._s.size() == 0? false: true;
    };
    friend void print_bn(const big_number &bn, int base = 16)
    {
        if (base == 16)
            std::cout << "0x";
        if (bn._s.size() == 0)
            std::cout << 0;
        for (auto it = bn._s.cbegin(); it != bn._s.cend(); it ++)
        {
            if (base == 16)
                std::cout << std::setfill('0') << std::setw(8)<<std::setbase(base) <<  *it << " ";
            if (base == 10)
                 std::cout <<  *it << " ";
        }
    };
public:
    void _consolidate(void)
    {
        while(_s.back() == 0) {_s.pop_back();};
    };

    ~big_number(void)
    {
        _s.clear();
    };
};


#endif
