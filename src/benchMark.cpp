#include "benchMark.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::pair;
using std::stod;
using std::string;
using std::vector;

map<string, map<string, vector<double>>> initalBenchMark()
{
    map<string, map<string, vector<double>>> benchMarkData;
    ifstream infile1, infile2;
    infile1.open("benchMark.txt");
    infile2.open("benchMarkInterval.txt");
    string buf1, buf2;
    while (getline(infile1, buf1) && getline(infile2, buf2))
    {
        vector<double> intervals;
        int pos = buf1.find(' ');
        string name = buf1.substr(0, pos);
        string expr = buf1.substr(pos + 1, buf1.length() - pos - 1);
        char s[1024];
        for (size_t i = 0; i < buf2.size(); i++)
        {
            s[i] = buf2[i];
        }
        vector<string> res;
        string str = buf2;
        std::stringstream input(str);
        string temp;
        const char pattern = ' ';
        while (getline(input, temp, pattern))
        {
            res.push_back(temp);
        }
        for (size_t k = 0; k < res.size(); k++)
        {
            intervals.push_back(atof(res.at(k).c_str()));
        }
        map<string, vector<double>> m1;
        m1.insert(pair<string, vector<double>>(expr, intervals));
        benchMarkData.insert(pair<string, map<string, vector<double>>>(name, m1));
    }

    return benchMarkData;
}