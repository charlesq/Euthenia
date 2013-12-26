#include <cstddef>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <limits>
#include <stdint.h>
#include <string.h>
#include <iomanip>
/* Note:
    1.  must be compiled with g++ solely because of use of namesapce   
    2. with g++, const_cast must be applied to cast from non-const to const 
    3. had to switch from scanf to std::cin due to scanf drawback of handling input buffer clearing
*/
namespace exercise {
    char * strchr(const char *s, int c)
    {
       while(*s != '\0')
           if (*s++ == static_cast<char>(c))
               return const_cast<char *>(--s);
       return NULL; 
    } 
    char *strrchr(const char *s, int c)
    {
        const char *p = NULL;
        while(*s != '\0')
            if (*s++ == static_cast<char>(c))
                p = s -1; 
       return const_cast<char*>(p);
    }
    inline int toupper(int c)
    {
        return c <= 'z' && c >= 'a'? c - 'a' + 'A': c;
    }
    inline int tolower(int c)
    {
        return c >= 'A' && c <= 'Z'? c - 'A' + 'a': c;
    }
    char *strstr(const char *haystack, const char *needle)
    {
        const char *phs = NULL, *pn;        
        while(*haystack != '\0' && needle != NULL)
        {
            phs = haystack;
            pn = needle;
            while(*pn != '\0' && *phs != '\0' && *phs == *pn)
            { 
                phs++;
                pn ++;
            }
            if (*pn == '\0')
                break; 
            haystack++;
        
        }
        return  *needle != '\0' && *pn  =='\0'? const_cast<char *>(haystack): NULL; 
    }
    char *strchrnul(const char *s, int c)
    {
    }
    char *strcasestr(const char *haystack, const char *needle)
    {
        const char *phs = NULL, *pn;
        while(*haystack != '\0' && needle != NULL)
        {
            phs = haystack;
            pn = needle;
            while(*pn != '\0' && *phs != '\0' && toupper(*phs) == toupper(*pn))
            {
                ++phs;
                ++pn;
            }
            if (*pn == '\0')
                break;
            haystack++;

        }
        return  *needle != '\0' && *pn  =='\0'? const_cast<char *>(haystack): NULL;
    }
    size_t strlen(const char *s)
    {
        size_t l = 0;
        while(*s++ != '\0') ++l; 
        return l;
    } 
    char * strcpy(char *dest, const char *src)
    {
        char *d = dest;
        while(*src != '\0')
            *dest ++ = *src++;
        *dest = '\0';
        return d;
    }
    char *strncpy(char *dest, const char *src, size_t n)
    {
       char *d = dest;
       while(n != 0)
       { 
           if ( *src != '\0')
               *dest ++ = *src ++;
           else
               *dest ++ = '\0';
           --n;
       }
       return d;
    }
    void *memmove(void *dest, const void *src, size_t n)
    {
       if (dest == src)
           return dest; 
       char  *to = static_cast<char *>(dest);
       const char *from = static_cast<const char*>(src); 
       if (to + n <= from ||
           from  < to)
       {
           while(n--!= 0)
               *to ++ = *from ++;
       }
       else
       {
          while(n-- != 0)
              *(to + n) = *(from + n);
       }
       return dest;
    }
    void *memccpy(void *dest, void *src, int c, size_t n)
    {
        char *d = static_cast<char *>(dest);
        const char *s = static_cast<const char *>(src);
        while(n-- != 0)
        {
            *d++ = *s++;
            if (c == *(s -1))
                break;
        }
        return n == -1? NULL: d;
    }
    void *memchr(const void *s, int c, size_t n)
    {
        const char *src = static_cast<const char *>(s);
        while(n-- != 0 && *src != c) ++src;
        return n == -1? NULL: const_cast<char *>(src); 
    }
    void *memrchr(const void *s, int c, size_t n)
    {
        while(*(static_cast<const char *>(s) + --n) != c && n != 0 ); 
        return n == -1? NULL: const_cast<char *>(static_cast<const char *>(s) + n); 
    }
    char *strcat(char *dest, const char *src)
    {
        char *d = dest;
        while(*d != '\0') ++d;
        while(*src != '\0')
            *d ++ = *src++;
        *d = '\0';
        return dest;
    }
    char *strncat(char *dest, const char *src, size_t n)
    {
        char *d = dest;
        while(*d != '\0')  ++d;
        while(n-- != 0 && *src != '\0')
            *d++ = *src ++;
        *d = '\0';
        return dest;
    }
    unsigned long strtoul(const char *nptr, char **endptr, int base)
    {
        bool base_specified = base == 0? false: true;
        unsigned long ret = 0;
        bool sign_symbol = false;
        bool consider_digit = false;
        bool non_space = false;
        bool initial_zero = true;
        if (!base_specified)
           base = 10; 
        const char *p = nptr; 
        while(*p != '\0')
        {
            /* skip leading spaces */
            if (!non_space && *p == ' ')
            {
                p++;
                continue;
            }
            if (!non_space) 
                non_space = true;
            if (!consider_digit)
            {
                if (*p == '+' || *p == '-')
                {
                    if (!sign_symbol)
                    {    
                        if (*p == '-')
                            ret = -ret;
                        sign_symbol = true; 
                        p++;
                        consider_digit = true;
                        continue;
                    }
                    else
                       break;
                }
                if (*p == '0' && (!base_specified || base == 16) )
                {
                    if (*(p +1) == 'x' || *(p+1) == 'X')
                    {
                       if(!base_specified)
                           base = 16;
                       consider_digit = true;
                       p += 2;
                       continue;
                    }
                    p++;
                    break;
                    
                }
                consider_digit = true;
            }
            if (*p >= '0' && *p <= '9')
            {
                if (base <= 10 && *p > '0' + base - 1)
                    break;
                ret = ret * base + *p++ - '0'; 
                continue;
            }
            if ((base > 10) &&( toupper(*p) >=  'A' && toupper(*p) <= base - 11 + 'A')) 
            {
                ret = ret * base + toupper(*p ++) - 'A' + 10;
                continue;
            }
            break; 
        } 
        if (endptr != NULL)
            *endptr = const_cast<char *>(p);
        return ret;
    }
    char * strtok_r(char *str, const char *delimeters, char **saveptr)
    {
         if (str == NULL && (str = *saveptr) == NULL)
             return NULL; 
         const char *delim;
         char *tok = str, *p = NULL; 
         while(*str != '\0')
         {
              delim = delimeters;
              while(*delim != '\0')
              {
                  if (*delim == *str)
                      break;
                  delim++;
              } 
             if (*delim != '\0' )
             {
                 if (p == NULL)
                 {
                     p = str;
                     if (tok == p)
                     {
                         tok ++;
                     }
                     else
                     {
                         *str++ = '\0';
                         break;
                     }
                 }
                 else
                 {
                    *str++ = '\0'; 
                    break;
                 }
             }
             ++str;
         }
         if (*str == '\0')
             str = NULL;
         *saveptr = str;
         return tok;
    }
    char *strtok(char *str, const char *delim)
    {
        static char *p = NULL;
        return strtok_r(str, delim, &p);
    }
    #define GET_INTINPUT(n, s)      \
       printf("provide %s:", s);    \
       std::cin.width(10);	            \
       std::cin >> n;               \
       printf("\n");

    #define GET_STRINPUT(str, s)    \
       printf("provide %s:", s);    \
       std::cin.width(200);             \
       std::cin >> str;            \
       printf("\n");

    #define GET_CHARINPUT(c, s)  \
       printf("provide %s:", s); \
       std::cin.width(200);      \
       std::cin >> c;          \
       printf("\n"); 
    #define PRINT_RESULT(p)                   \
        if (p == NULL)                       \
            printf("no occurence found!");   \
        else                                 \
            printf("here it is %s", p);      \

    #define PRINT_RESULT_CHAR(c) printf ("%c", c);
    #define PRINT_RESULT_INT(n)  printf("%d", n);

    #define TEST_FUNC(FUNC) test_##FUNC

    void test_strchr(void)
    {
        char str[100];
        char ch;

        GET_STRINPUT(str, "target string")
        GET_CHARINPUT(ch, "char to search for")
        char *p = strchr(str, ch);
        PRINT_RESULT(p)
    }
    void test_strrchr(void)
    {
        char str[100];
        char ch;
        GET_STRINPUT(str, "target string")
        GET_CHARINPUT(ch, "char to search for")
        char *p = strrchr(str, ch);
        PRINT_RESULT(p)
    }
    void test_strstr(void)
    {
        char str[100];
        char n[100];
        GET_STRINPUT(str, "target string")
        GET_STRINPUT(n, "needle string to search for")
        char *p = strstr(str, n);
        PRINT_RESULT(p)
    }
    
    void test_toupper(void)
    {
        char ch;
        GET_CHARINPUT(ch, "a char")
        PRINT_RESULT_CHAR(static_cast<char>(toupper(ch)))
    }
    void test_tolower(void)
    {
        char ch;
        GET_CHARINPUT(ch, "a char")
        PRINT_RESULT_CHAR(static_cast<char>(tolower(ch)))
    }
    void test_strcasestr(void)
    {
        char str[100];
        char n[100];
        GET_STRINPUT(str, "target string")
        GET_STRINPUT(n, "needle string to search for")
        char *p = strcasestr(str, n);
        PRINT_RESULT(p)

    }
    void test_strlen(void)
    {
       char str[100];
       GET_STRINPUT(str, "test string")
       PRINT_RESULT_INT(strlen(str)); 
    }
    void test_strcpy(void)
    {
       char s1[100];
       char s2[100];
       GET_STRINPUT(s1, "source string")
       GET_STRINPUT(s2, "destination string")  
       char *s = strcpy(s2, s1);
       PRINT_RESULT(s1);
    }
    void test_strncpy(void)
    { 
       char s1[100], s2[100];
       size_t len;
       GET_STRINPUT(s1, "source string")
       GET_STRINPUT(s2, "destination string")
       GET_INTINPUT(len, " length to copy")
       char *s = strncpy(s2, s1, len); 
       PRINT_RESULT(s);
    }
    void print_bytes(const void *s, size_t n)
    {
        const char *src = static_cast<const char *>(s); 
        while(n-- != 0)
        {
            printf("%c", *src++);
        }
    }
    void test_memmove(void)
    {
        static int flag = 0;
        char s[100]; 
        char *src, *dest;
        size_t n;
        GET_STRINPUT(s, " a string")
        GET_INTINPUT(n, " length to copy")   
        switch(flag)
        {
        case 0:
            src = s;
            dest = s + n; 
            printf("\nsource and destination buffer do not overlap"); 
            printf("\nsource buffer:"); 
            print_bytes(src, n);
            printf("\ndestination buffer:");
            printf("%s\n", dest);
            ++flag;
            break;
        case 1:
            src = s;
            dest = s + n/2;
            printf("\nsource and destination buffer overlap with former leading");
            printf("\nsource buffer:"); 
            print_bytes(src, n);
            printf("\ndestination buffer: ");
            printf("%s\n", dest); 
            ++flag;
            break;
        default:
            flag = 0;
            dest = s;
            src = s + n/2;
            printf("\nsource and destination buffer overlap with latter leading");
            printf("\nsource buffer:");
            print_bytes(src, n/2); 
            printf("\ndestination buffer: "); 
            printf("%s\n", dest);
            n = n/2;
        }
        dest = static_cast<char *>(memmove(dest, src, n)); 
        printf("%s\n",dest);
    }
    void test_memccpy(void)
    {
        char dest[200], src[200], c;
        size_t n;
        char *p;
      
        GET_STRINPUT(dest, "target string")
        GET_STRINPUT(src, "source string") 
        GET_INTINPUT(n, " number of bytes to copy")   
        GET_CHARINPUT(c, "the stop char")
        p = static_cast<char *>(memccpy(dest, src, c, n));
        PRINT_RESULT(dest);
        printf("\n");
        PRINT_RESULT(p);
    } 
    void test_memchr(void)
    {
        char s[200], c;
        size_t n;
        char *p;
        GET_STRINPUT(s, "string to search")
        GET_INTINPUT(n, "number of bytes to check")
        GET_CHARINPUT(c, "the stop char ")
        p = static_cast<char *>(memchr(s, c, n));
        PRINT_RESULT(p) 
    }
    void test_memrchr(void)
    {
        char s[200], c;
        size_t n;
        char *p;
        GET_STRINPUT(s, "string to search")
        GET_INTINPUT(n, "number of bytes to check")
        GET_CHARINPUT(c, "the stop char ")
        p = static_cast<char *>(memrchr(s, c, n));
        PRINT_RESULT(p)
    }
    void test_strcat(void)
    {
        char dest[200];
        char src[200];
        GET_STRINPUT(src,"source string")
        GET_STRINPUT(dest, "destination string")
        char *d = strcat(dest, src);  
        PRINT_RESULT(d)
    }
    void test_strncat(void)
    {
        char dest[200];
        char src[200];
        size_t n;
        GET_STRINPUT(src, "source string")
        GET_STRINPUT(dest, "destination string")
        GET_INTINPUT(n, "lenght to copy")
        char *d = strncat (dest, src, n);
        PRINT_RESULT(d);
    }
    void test_strtoul(void)
    {
        int base;
        char str[100];
        char *endptr;
        unsigned long ret;
        GET_STRINPUT(str, "a string")
        GET_INTINPUT(base, "base")
        ret = strtoul(str, &endptr, base); 
        PRINT_RESULT_INT(ret);
    }
    void test_strtok(void)
    {
       char str[200];
       char tok[200]; 
       GET_STRINPUT(str, "Taget string")
       GET_STRINPUT(tok, "token string")
       char *p = str;
       char *ss = str;
       while(p != NULL)
       {
           p = strtok(ss, tok);
           printf("token = %s\n", p);
           ss = NULL;
       }
    }

    static struct _str_match_func
    {
        int index;
        const char *func;
        void (*test)(void);
    }f_table[] =
    {
        {0, "toupper",TEST_FUNC(toupper)},
        {1, "tolower", TEST_FUNC(tolower)},
        {2, "strchr", TEST_FUNC(strchr)},
        {3, "strrchr",TEST_FUNC(strrchr)},
        {4, "strst", TEST_FUNC(tolower)},
        {5, "strcasestr", TEST_FUNC(strcasestr)}, 
        {6, "strlen", TEST_FUNC(strlen)},
        {7, "strcpy", TEST_FUNC(strcpy)},
        {8, "strncpy", TEST_FUNC(strncpy)},
        {9, "memmove", TEST_FUNC(memmove)},
        {10, "memccpy", TEST_FUNC(memccpy)},
        {11, "memchr", TEST_FUNC(memchr)},
        {12, "memrchr", TEST_FUNC(memrchr)},
        {13, "strcat", TEST_FUNC(strcat)},
        {14, "strncat", TEST_FUNC(strncat)},
        {15, "strtoul", TEST_FUNC(strtoul)},
        {16, "strtok", TEST_FUNC(strtok)},
        /*must be the last one */
        {255, "exit", NULL}
    };
};

int get_choice(void)
{
    #define MAX_NAME_LEN 15 
    for (size_t i = 0; i < sizeof(exercise::f_table) / sizeof (exercise:: _str_match_func);i ++ )
    {
#if 0
        char paddings[] = "                              ";        
        paddings[MAX_NAME_LEN - strlen(exercise::f_table[i].func) + 1] = '\0'; 
        printf("(%3d) %10s      ", exercise::f_table[i].index, exercise::f_table[i].func, paddings); 
        if (i%3 == 2 ) 
            printf("\n");
#else
        std::cout << std::setfill(' ');
        std::cout << "(" << std::setw(3) << std::right << exercise::f_table[i].index << ") ";
        std::cout << std::setw(15);
        std::cout << std::setfill(' ');
        std::cout << std::left<< exercise::f_table[i].func;
        if (i %3 == 2)
            std::cout << std::endl;
        
#endif
    }
    int o = -1 ;  
    while (o < 0 ||  o > static_cast <int>(sizeof(exercise::f_table) / sizeof(exercise::_str_match_func) -2))
    {
        printf("select one to proceed:");
        std::cin >> o; 
        if ( o == 255)
            break;
        std::cin.clear();
        printf("\n");
    }
    return o;
}
   #define TEST_BEGIN           \
       while (true)            \
       {

   #define TEST_END                                  \
            char _ch ;                               \
            printf("\nExit(Y/N)?");                  \
            std::cin >> _ch;                         \
            if (_ch == 'y' || _ch == 'Y')            \
                break;                               \
            printf("\n");                            \
                                                     \
        }
int main()
{
    TEST_BEGIN
    int index = get_choice(); 
    if (index == 255)
        break;
    exercise::f_table[index].test();
    TEST_END
    return 0; 
}
