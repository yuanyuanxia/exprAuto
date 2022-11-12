#include "benchMark.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <fstream>

using std::ifstream;
using std::map;
using std::pair;
using std::string;
using std::vector;

map<string, map<string, vector<int>>> initalBenchMark()
{
    map<string, map<string, vector<int>>> benchMarkData;
    ifstream infile;
    infile.open("benchMark.txt");
    string buf;
    while (getline(infile, buf))
    {
        int pos = buf.find(' ');
        string name = buf.substr(0, pos);
        string expr = buf.substr(pos + 1, buf.length() - pos - 1);
        //区间信息暂不确定
        vector<int> interval;
        interval.push_back(1024);
        map<string, vector<int>> m1;
        m1.insert(pair<string, vector<int>>(expr, interval));
        benchMarkData.insert(pair<string, map<string, vector<int>>>(name, m1));
    }

    return benchMarkData;
}