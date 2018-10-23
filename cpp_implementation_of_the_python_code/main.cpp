#include <iostream>
#include <bits/stdc++.h>
#include "CRO.h"

using namespace std;

int main()
{
    //cout << "Hello world!" << endl;
    srand (time(NULL));

    vector<pair<int,int>> vii;
    float t1, t2;
    for(int i = 0; i < 10 ; i++)
    {
        //https://stackoverflow.com/questions/686353/c-random-float-number-generation
        t1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        t2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        vii.push_back(make_pair(t1, t2));

        //cout << t1 << " " << t2 << endl;
    }
    CRO c(vii);
    c.run();
    cout << "hello CRO" <<c.optimal.pe << endl;

    return 0;
}
