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
/*��ȡ���ݣ�min_suptΪ��С֧�ֶȣ� eventsΪ�¼�����Ȼ���ȡ�����¼���*/
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
/*��ʼ��C��L�������ϣ�CΪ��ѡ���ϣ�LΪƵ�����*/
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
/*�������һ������ķ�������֦�����һ��Ƶ����ĸ�����ĺͻ��г˻���
��ȵĻ�����ô����������ȣ��������һ������Ӽ�����Ƶ����Ļ�����
ô���һ������Ƶ�����������Լ���һ֦*/
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
/*��ӡL��еĽ��������n��ʾ����Ƶ��n�*/
void print_result(int n)
{
    cout<<"L"<<n<<"Ƶ��"<<n<<"�:"<<endl;
    for(map<string,int>::iterator it= L.begin(); it != L.end(); it++)
    {
        cout<<"{"<<(*it).first<<"}:"<<(*it).second<<"  "<<endl;
    }
    cout<<endl;
}
/*�ж�S�ǲ���Ƶ�����������֦����һ�����÷������Լ�֦��ͳ��Ƶ����ĸ���*/
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
/*�ڶ�������֧�ֶȼ�������һ֦*/
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
/*��ȡ��ѡ����C���õ��ǱȽ�������ѡ���ǰn-1�����ȣ����������
  һ����ѡ�  ����Ҫ����һ�����������Ǻ�ѡ���Ϊһ�����*/
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
/*���������㷨����ת*/
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
