#include <iostream>
#include <utility>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <forward_list>
#include <stack>
#include <queue>
#include <string.h>
#include <vector>
using namespace std;

string  lcs(const string &a, const string & b) 
{
    unsigned int ai = 0, bi = 0;
    unsigned int l = 0;
    vector<vector<unsigned int>> v(a.length() + 1);
    for (vector<unsigned int > &aa: v)
        aa.resize(b.length() + 1, 0);
    for (int i = 0; i < a.length(); i++)
    {
        for ( int j = 0; j < b.length(); j ++)
        {
           if ( a[i] == b[j])
               v[i + 1][j+1] = v[i][j] + 1;
           if (v[i +1] [j+1] > l)
           {
              ai = i ; 
              bi = j ;
              l = v[i+1][j+1];
            }
        }
    } 
    cout << " len is " << l << endl;
    if (l == 0)
        return string("");
    return a.substr(ai - l +1, l); 
}
int main()
{
    
    unsigned int k = 0; 
    while(true)
    {
        string a, b;
        cout << "Input string a: "; 
        cin >> a;
        cout << endl<<"Input string b: ";
        cin >> b;
        cout << endl;
        cout << lcs(a, b) << endl;
        
        char t;
        cout << "Halt(Y/N)?"; 
        cin >> t;
        if ( t == 'y' || t == 'Y')
            break;
    }
    return 0;
}
