#include <ctype.h>
#include <vector>
#include <cstddef>
#include <memory>
#include <cstring>
#include <cstdarg>
#include <unordered_set>
class trie
{
    class node
    {
        unsigned int prefixes;
        unsigned int words;
        std::vector<std::shared_ptr<node>> subnodes;
        node(void):prefixes(0), words(0){};
        void insert_word(const char * word)
        {
            prefixes ++;
            if (*++word == '\0')
            {
               words++;
               return;
            }
            if(subnodes.empty())
            {
                subnodes.resize(trie::num_letters, std::shared_ptr<node>());
            }
            int ch = static_cast<int>(*word);
            ch = tolower(ch);
            if (subnodes[ch].use_count() == 0)
            {
                subnodes[ch] = std::shared_ptr<node>(new node);
            }
            subnodes[ch]->insert_word(word);
        };
        const std::vector<std::shared_ptr<node>> & get_suffix_nodes(const char *w) const
        {
           static std::vector<std::shared_ptr<node>> null_vector;
           if (*++w == '\0')
               return subnodes;
           int ch = static_cast<int>(*w);
           if (subnodes[ch].get() == NULL)
               return null_vector;
           return subnodes[ch]->get_suffix_nodes(w); 
        }
        unsigned int count_word(const char *w) const
        {
            if (*++w == '\0')
                return words;
            int ch = static_cast<int>(*w);
            ch = tolower(ch);
            if (subnodes.empty() || subnodes[ch].use_count() == 0)
                return 0;
            return subnodes[ch]->count_word(w);
        };
        unsigned int count_prefix(const char *w) const
        {
            if (*++w == '\0')
                return prefixes;
            int ch = static_cast<int>(*w);
            ch = tolower(ch);
            if (subnodes.empty() || subnodes[ch].use_count() == 0)
                return 0;
            return subnodes[ch]->count_prefix(w);
        };
        friend class trie;
    public:
        ~node()
        {
            subnodes.clear();
        };
      
    };
    node anode;
public:
    static const unsigned int num_letters = 127;
    trie(void){};
    void insert_word(const char * w)
    {
        if (w == NULL || *w == '\0')
            return;
        anode.insert_word(w);
    };
    bool find_word(const char *w) const
    {
        if (w == NULL || *w == '\0')
           return false;
        return anode.count_word(w) > 0;
    };
    unsigned int get_prefix_count(const char *w) const
    {
        if (w == NULL || *w == '\0')
           return 0;
        return anode.count_prefix(w);
    };
    unsigned int get_word_count(const char *w) const
    {
        if (w == NULL || *w == '\0')
            return 0;
        return anode.count_word(w);
    };

    std::unique_ptr<const char > find_longest_common_prefix(unsigned int count, ...)
    {
         // anode must be reset for this operation
         anode.subnodes.clear(); 
         std::unique_ptr<const char > up;
         if (count == 0)
             return up;
         const char *shortest = NULL, *p1;
         unsigned int _count = count;
         va_list ap;
         va_start(ap, count);
         while(count-- != 0)
         {
             p1 = va_arg(ap, const char *);
             insert_word(p1);
             if (shortest == NULL || strlen(p1) < strlen(shortest))
                 shortest = p1;
         }
         va_end(ap);
         unsigned int len = strlen(shortest);
         if (len == 0)
             return up;
         unsigned int c = 0;
         while(c < len)
         {
             if (anode.count_prefix(shortest+c) == _count)
                 break;
             c++;
         }
         if (c != len)
             up = std::unique_ptr<const char >(strdup(shortest + c));
         return up;
    };
    std::unordered_set<std::string> get_auto_complete_words(const char *w)
    {
        if (w == NULL || *w == '\0' || anode.subnodes.empty())
            return std::unordered_set<std::string>();
        int ch = tolower(static_cast<int>(*w));
        const std::vector<std::shared_ptr<node>> vsnodes = anode.get_suffix_nodes(w);
        if (vsnodes.empty() || vsnodes[ch].get() == NULL)
            return std::unordered_set<std::string>();
         
         

        
    };
    ~trie()
    {
        anode.subnodes.clear();
    };
private:
#if 0
    std::unordered_set<std::string> reproduce_words(const char *w, std::vector<std::shared_ptr<node>> vn, bool include)
    {
        string s(w);
        std::unordered_set<std::string> us;
        us.insert(s); 
        while(!vn.empty()) 
        {
           std::unordered_set<std::string> w;
           for (auto it = vn.cbegin(); it != vn.cend(); it++)
           {
               if ((*it).get() == NULL)
                   continue;
               std::unordered_set<std::string> w;
               for (auto s: us)
               {
                   s += string(it - vn.cbegin(); 
                   w.insert(s);

               } 
               
           }
           
        }
    };
#endif
    
};
#include <iostream>
int main()
{
    trie pt;
    std::unique_ptr<const char > lcp = pt.find_longest_common_prefix(4, "Wellsfargo", "well", "wellington", "wellS");
    std::cout << lcp.get() << std::endl;
    return 0;
}
