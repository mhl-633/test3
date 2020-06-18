#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

map<string,int> C;
map<string,int> L;
vector <int> mularray;
vector <int> addarray;
vector<vector<string> > D;

int min_supt,events;
/*读取数据，min_supt为最小支持度； events为事件数，然后读取各个事件；*/
void init_data()
{
    int n;
    string tmp;
    cin >> min_supt >>events;
    for(int i = 0 ; i < events; i++)
    {
        cin >> n;
        vector <string> d;
        for(int j = 0 ; j < n; j++)
        {
            cin >> tmp;
            d.push_back(tmp);
        }
        sort(d.begin(),d.end());
        D.push_back(d);
    }
}
/*初始化C和L两个集合，C为候选集合，L为频繁项集；*/
void init_CL()
{
    for(int i = 0 ; i < events; i++)
    {
        for(int j = 0 ; j < D[i].size(); j++)
        {
            C[D[i][j]]++;
        }
    }
    for(map<string,int>::iterator it= C.begin(); it != C.end(); it++)
    {
        if(it->second >= min_supt)
        {
            L[(*it).first]=(*it).second;
        }
    }
}
/*这里采用一个特殊的方法来剪枝，如果一个频繁项集的各个项的和还有乘积都
相等的话。那么这两个项相等，用于如果一个项集的子集不是频繁项集的话，那
么该项集一定不是频繁项集，这里可以剪掉一枝*/
void calc_mul_add_value()
{
    addarray.clear();
    mularray.clear();
    for(map<string,int>::iterator it= L.begin(); it != L.end(); it++)
    {
        string  s = (*it).first;
        int mulval=1,addval=0;
        for(int i = 0 ; i < s.length(); i++)
        {
            int t = (int)s[i];
            addval += t;
            mulval *= t;
        }
        addarray.push_back(addval);
        mularray.push_back(mulval);
    }
}
/*打印L项集中的结果，参数n表示的是频繁n项集*/
void print_result(int n)
{
    cout<<"L"<<n<<"频繁"<<n<<"项集:"<<endl;
    for(map<string,int>::iterator it= L.begin(); it != L.end(); it++)
    {
        cout<<"{"<<(*it).first<<"}:"<<(*it).second<<"  "<<endl;
    }
    cout<<endl;
}
/*判断S是不是频繁项集，两个剪枝。第一，利用反单调性剪枝。统计频繁项集的个数*/
int judge_item(string s)
{
    int mulval=1,addval=0;
    for(int i = 0 ; i < s.length(); i++)
    {
        int t = (int)s[i];
        addval += t;
        mulval *= t;
    }
    for(int i = 0 ; i < s.length();i++)
    {
        bool flag = true;
        int tmpaddvalue = addval-(int)s[i];
        int tmpmulvalue = mulval/((int)s[i]);
        for(int j = 0;j < addarray.size();j++)
        {
            if(tmpaddvalue==addarray[j] && tmpmulvalue==mularray[j])
            {
                flag = false;
                break;
            }
        }
        if(flag) return 0;
    }
    int i,j,k,count = 0;
    for(i = 0 ; i < events; i++)
    {
        bool flag = true ;
        for(j = 0; j < s.length(); j++)
        {
            string tmps = s.substr(j,1);
            for(k = 0 ; k < D[i].size(); k++)
            {
                if(tmps == D[i][k]) break;
            }
            if(k == D[i].size())
            {
                flag = false;
                break;
            }
        }
        if(flag) count++;
    }
    return count;
}
/*第二：利用支持度剪掉另外一枝*/
void get_frequent_L()
{
    L.clear();
    for(map<string,int>::iterator it= C.begin(); it != C.end(); it++)
    {
        if(it->second >= min_supt)
        {
            L[(*it).first]=(*it).second;
        }
    }
}
/*获取候选集合C，用的是比较两个候选项的前n-1项，若相等，则可以生成
  一个候选项，  另外要处理一个特例，就是候选项长度为一的情况*/
void get_candidate_C(int n)
{
    C.clear();
    for(map<string,int>::iterator it = L.begin(); it != L.end(); it++)
    {
        string str1 = (*it).first,s;
        map<string,int>::iterator t = it ;
        t++;
        for(; t != L.end(); t++)
        {
            s = str1;
            string str2 = (*t).first;
            if(n==1 || str1.substr(0,str1.length()-1)==str2.substr(0,str2.length()-1))
            {
                s.append(str2.substr(str2.length()-1,1));
                int num = judge_item(s);
                if( num > 0) C[s] = num;
            }
        }
    }
    get_frequent_L();
}
/*控制整个算法的运转*/
void Apriori()
{
    int n=1;
    while(!L.empty())
    {
        print_result(n);
        calc_mul_add_value();
        get_candidate_C(n);
        get_frequent_L();
        n++;
    }
}
int main()
{
    freopen("in.txt","r",stdin);
    init_data();
    init_CL();
    Apriori();
    return 0;
}
