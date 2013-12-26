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


string lp_manatra(string s)
{
}

string lp_dp(string s)
{
    vector<vector<bool>> v(s.length());
    unsigned int i = 0, j = 0;
    for (vector<bool> &a: v)
        a.resize(s.length() +1, false); 
    for (j = 0; j < v.size(); j ++)
    {
            v[j][1] = true;
            v[j][0] = true;
    }
    unsigned int mi = 0, mj = 1;
    for (j = 2; j <= v.size() ; j ++)
    {
        for (i = 0; i + j -1 < v.size(); i ++)
        {
            if( s[i] != s[i + j -1] || !v[i+1][j-2])
                continue;
            v[i][j] = true;
            if (mj < j)
            {
                mj = j;
                mi = i;
            }
        }
    } 
    for (vector<bool> &vv: v)
        vv.clear();
    v.clear();
    return s.substr(mi, mj);

}
int main()
{
    
    unsigned int k = 0; 
    while(true)
    {
        string a;
        cout << "Input string : "; 
        cin >> a;
        cout << endl <<  lp_dp(a) << endl;
        
        char t;
        cout << "Halt(Y/N)?"; 
        cin >> t;
        if ( t == 'y' || t == 'Y')
            break;
    }
    return 0;
}
