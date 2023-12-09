#include <iostream>
#include <stdio.h>
#include <string.h>
#include <deque>
#include <algorithm>
#include <vector>
#include <utility>

using namespace std;
// 每次新增页面时将其插入队尾,溢出时弹出队头页面
int FIFO(deque<int> Str, char s[], int f)
{
    int fault = 0;

    for (int i = 0; i < strlen(s); i++)
    {
        int te = s[i] - '0';
        if (find(Str.begin(), Str.end(), te) != Str.end())
        {
            continue;
        }
        else if (Str.size() == f)
        {
            Str.pop_front();
            Str.push_back(te);
            fault++;
        }
        else
        {
            Str.push_back(te);
            fault++;
        }
    }
    printf("Faults using FIFO: %d\n", fault);
    return fault;
}
// 每次访问现有页面时将其移到队尾,新增页面也插入队尾,溢出时弹出队头页面
int LRU(deque<int> Str, char s[], int f)
{
    int fault = 0;

    for (int i = 0; i < strlen(s); i++)
    {
        int te = s[i] - '0';
        if (find(Str.begin(), Str.end(), te) != Str.end())
        {
            deque<int>::iterator ii;
            ii = Str.begin();
            while (ii != Str.end())
            {
                if (*ii == te)
                {
                    Str.erase(ii); 
                }
                ii++;
            }
            Str.push_back(te);
            continue;
        }
        else if (Str.size() == f)
        {
            Str.pop_front();
            Str.push_back(te);
            fault++;
        }
        else
        {
            Str.push_back(te);
            fault++;
        }
    }
    printf("Faults using LRU: %d\n", fault);
    return fault;
}
// 查找最长未来间隔页面,统计并返回页面置换错误数
int futurer(char s[], int i, vector<int> current)
{
    int maxi = 0;
    int tempor = 0;
    vector<int> v;

    for (int j = i; j < strlen(s); j++)
    {
        if (s[j] >= '0' && s[j] <= '9')
            v.push_back(s[j] - '0');
    }
    for (size_t ii = 0; ii < current.size(); ii++)
    {
        vector<int> tempvec;
        tempvec.push_back(current[ii]);
        vector<int>::iterator ip;

        ip = find_first_of(v.begin(), v.end(), tempvec.begin(), tempvec.end());
        if (maxi < ip - v.begin())
        {
            tempor = current[ii];
            maxi = ip - v.begin();
        }
    }
    return tempor;
}
// 选择最远未来要访问的页面置换
int OPT(vector<int> Str, char s[], int f)
{
    int fault = 0;
    vector<int> st;
    for (int i = 0; i < strlen(s); i++)
    {
        int te = (s[i] - '0');
        if (find(st.begin(), st.end(), te) != st.end())
        {
            continue;
        }

        if (st.size() != f)
        {
            st.push_back(te);
            fault++;
        }
        else if (st.size() == f)
        {
            // find position to be removed in ii
            int removable = futurer(s, i, st);
            vector<int>::iterator ii;
            ii = Str.begin();
            while (ii != Str.end())
            {
                if (*ii == removable)
                {
                    Str.erase(ii);
                }
                ii++;
            }
            st.push_back(te);
            fault++;
        }
    }
    printf("Faults using OPT: %d\n", fault);
    return fault;
}

int main()
{
    int n;
    printf("Enter number of iterations: ");
    scanf("%d", &n);
    vector<pair<int, int>> belady;

    for (int v = 0; v < n; v++)
    {
        int f, fifo, lru, opt;
        deque<int> Str;
        char s[100];
        printf("Enter the number of frames allocated: ");
        scanf("%d", &f);
        printf("Enter the reference string: ");
        scanf("%s", s);
        vector<int> cat;

        fifo = FIFO(Str, s, f);
        lru = LRU(Str, s, f);
        opt = OPT(cat, s, f);
        belady.push_back(make_pair(f, fifo));
    }
    for (int w = 0; w < n; w++)
    {
        for (int ww = 0; ww < n; ww++)
        {
            if (belady[w].first > belady[ww].first)
                if (belady[w].second > belady[ww].second)

                    printf("\nBelady Anomaly detected\n");
        }
    }
}