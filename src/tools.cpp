#include "tools.hpp"
#include "exprAuto.hpp"
#include "parserASTLY.hpp"
#include "basic.hpp"
#include "geneCode.hpp"
#include "color.hpp"
#include "devideUpEdgeData.hpp"
#include <ctime>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::to_string;

//get unique label(timestamp)
string getUniqueLabel()
{
    time_t currentTime = time(NULL);
    char chCurrentTime[256];
    strftime(chCurrentTime, sizeof(chCurrentTime), "%Y%m%d%H%M%S", localtime(&currentTime));
    string uniqueLabel = chCurrentTime;
    return uniqueLabel;
}

//get init interval info
vector<double> getIntervals(string interval, const char *split)
{
    char *p;
    char *a = new char[interval.length() + 1]();
    size_t i;
    for (i = 0; i < interval.length(); i++)
    {
        a[i] = interval[i];
    }
    a[i] = '\0';
    vector<double> intervalVector;
    p = strtok(a, split);
    while (p != NULL)
    {
        intervalVector.push_back(atof(p));
        p = strtok(NULL, split);
    }

    delete [] a;
    return intervalVector;
}

//get init scale info
vector<int> getScales(string scale, const char *split)
{
    char *q;
    char *b = new char[scale.length() + 1]();
    size_t i;
    for (i = 0; i < scale.length(); i++)
    {
        b[i] = scale[i];
    }
    b[i] = '\0';
    vector<int> scaleVector;
    q = strtok(b, split);
    while (q != NULL)
    {
        scaleVector.push_back(stoi(q));
        q = strtok(NULL, split);
    }
    delete [] b;
    return scaleVector;
}

// test error
// old method, useless
exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, int x0Size)
{
    string param1 = fmt::format("{}", x0Start);
    string param2 = fmt::format("{}", x0End);
    string param3 = fmt::format("{}", x0Size);

    string prefix = "expr_" + uniqueLabel;
    string middle = param1 + "_" + param2 + "_" + param3;
    string fileNameKernel = prefix + "__" + middle + "_" + suffix;
    string testName = "./outputs/" + fileNameKernel + "_error.txt";
    string scriptName = "./detectErrorOneFPEDParallel.sh";
    string commandStr = scriptName + " " + uniqueLabel + " " + param1 + " " + param2 + " " + param3 + " " + prefix + " " + middle + " " + suffix;
    cout << "fileNameKernel: " << fileNameKernel << "\ncommand: " << commandStr << "\ntestName: " << testName << endl;
    char command[200] = {0};
    strcat(command, commandStr.c_str());
    system(command);
    std::ifstream ifs(testName, std::ios::in);

    double aveError = 0;
    double maxError = 0;
    exprInfo tempError;

    char ch;
    ifs >> ch;
    if (ifs.eof())
    {
        std::cout << "is null" << std::endl;
        ifs.close();
        std::vector<double> intervals;
        intervals.push_back(x0Start);
        intervals.push_back(x0End);

        tempError.intervals = intervals;
        tempError.aveError = aveError;
        tempError.maxError = maxError;
    }
    else
    {
        std::ifstream ifs(testName, std::ios::in);

        std::string lineStr;
        std::getline(ifs, lineStr); // discard first line
        std::getline(ifs, lineStr); // get the second line
        char *line = (char *)calloc(lineStr.length(), sizeof(char));
        strcpy(line, lineStr.c_str());
        const char *delim = " ,\t"; // Sets the delimiter for the string to be decomposed
        string aveErrorTemp = strtok(line, delim);
        string maxErrorTemp = strtok(NULL, delim);
        // cout << "aveError: " << aveErrorTemp << "\tmaxError: " << maxErrorTemp << endl;

        std::vector<double> intervals;
        intervals.push_back(x0Start);
        intervals.push_back(x0End);

        tempError.intervals = intervals;
        tempError.aveError = atof(aveErrorTemp.c_str());
        tempError.maxError = atof(maxErrorTemp.c_str());
    }

    return tempError;
}
// old method, useless
exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, int x0Size, int x1Size)
{
    string param1 = fmt::format("{}", x0Start);
    string param2 = fmt::format("{}", x0End);
    string param3 = fmt::format("{}", x1Start);
    string param4 = fmt::format("{}", x1End);
    string param5 = fmt::format("{}", x0Size);
    string param6 = fmt::format("{}", x1Size);


    string prefix = "expr_" + uniqueLabel;
    string middle = param1 + "_" + param2 + "_" + param3 + "_" + param4 + "_" + param5 + "_" + param6;
    string fileNameKernel = prefix + "__" + middle + "_" + suffix;
    string testName = "./outputs/" + fileNameKernel + "_error.txt";
    string scriptName = "./detectErrorTwoFPEDParallel.sh";
    string commandStr = scriptName + " " + uniqueLabel + " " + param1 + " " + param2 + " " + param3 + " " + param4 + " " + param5 + " " + param6 + " " + prefix + " " + middle + " " + suffix;
    cout << "fileNameKernel: " << fileNameKernel << "\ncommand: " << commandStr << "\ntestName: " << testName << endl;
    char command[200] = {0};
    strcat(command, commandStr.c_str());
    system(command);
    std::ifstream ifs(testName, std::ios::in);

    double aveError = 0;
    double maxError = 0;
    exprInfo tempError;

    char ch;
    ifs >> ch;
    if (ifs.eof())
    {
        std::cout << "is null" << std::endl;
        ifs.close();
        std::vector<double> intervals;
        intervals.push_back(x0Start);
        intervals.push_back(x0End);
        intervals.push_back(x1Start);
        intervals.push_back(x1End);

        tempError.intervals = intervals;
        tempError.aveError = aveError;
        tempError.maxError = maxError;
    }
    else
    {
        std::ifstream ifs(testName, std::ios::in);

        std::string lineStr;
        std::getline(ifs, lineStr); // discard first line
        std::getline(ifs, lineStr); // get the second line
        char *line = (char *)calloc(lineStr.length(), sizeof(char));
        strcpy(line, lineStr.c_str());
        const char *delim = " ,\t"; // Sets the delimiter for the string to be decomposed
        string aveErrorTemp = strtok(line, delim);
        string maxErrorTemp = strtok(NULL, delim);
        // cout << "aveError: " << aveErrorTemp << "\tmaxError: " << maxErrorTemp << endl;

        std::vector<double> intervals;
        intervals.push_back(x0Start);
        intervals.push_back(x0End);
        intervals.push_back(x1Start);
        intervals.push_back(x1End);

        tempError.intervals = intervals;
        tempError.aveError = atof(aveErrorTemp.c_str());
        tempError.maxError = atof(maxErrorTemp.c_str());
    }

    return tempError;
}
// old method, useless
exprInfo testError(string uniqueLabel, string suffix, double x0Start, double x0End, double x1Start, double x1End, double x2Start, double x2End, int x0Size, int x1Size, int x2Size)
{
    string param1 = fmt::format("{}", x0Start);
    string param2 = fmt::format("{}", x0End);
    string param3 = fmt::format("{}", x1Start);
    string param4 = fmt::format("{}", x1End);
    string param5 = fmt::format("{}", x2Start);
    string param6 = fmt::format("{}", x2End);
    string param7 = fmt::format("{}", x0Size);
    string param8 = fmt::format("{}", x1Size);
    string param9 = fmt::format("{}", x2Size);

    string prefix = "expr_" + uniqueLabel;
    string middle = param1 + "_" + param2 + "_" + param3 + "_" + param4 + "_" + param5 + "_" + param6 + "_" + param7 + "_" + param8 + "_" + param9;
    string fileNameKernel = prefix + "__" + middle + "_" + suffix;
    string testName = "./outputs/" + fileNameKernel + "_error.txt";
    string scriptName = "./detectErrorThreeFPEDParallel.sh";
    string commandStr = scriptName + " " + uniqueLabel + " " + param1 + " " + param2 + " " + param3 + " " + param4 + " " + param5 + " " + param6 + " " + param7 + " " + param8 + " " + param9 + " " + prefix + " " + middle + " " + suffix;
    cout << "fileNameKernel: " << fileNameKernel << "\ncommand: " << commandStr << "\ntestName: " << testName << endl;
    char command[200] = {0};
    strcat(command, commandStr.c_str());
    system(command);
    std::ifstream ifs(testName, std::ios::in);

    double aveError = 0;
    double maxError = 0;
    exprInfo tempError;

    char ch;
    ifs >> ch;
    if (ifs.eof())
    {
        std::cout << "is null" << std::endl;
        ifs.close();
        std::vector<double> intervals;
        intervals.push_back(x0Start);
        intervals.push_back(x0End);
        intervals.push_back(x1Start);
        intervals.push_back(x1End);
        intervals.push_back(x2Start);
        intervals.push_back(x2End);

        tempError.intervals = intervals;
        tempError.aveError = aveError;
        tempError.maxError = maxError;
    }
    else
    {
        std::ifstream ifs(testName, std::ios::in);

        std::string lineStr;
        std::getline(ifs, lineStr); // discard first line
        std::getline(ifs, lineStr); // get the second line
        char *line = (char *)calloc(lineStr.length(), sizeof(char));
        strcpy(line, lineStr.c_str());
        const char *delim = " ,\t"; // Sets the delimiter for the string to be decomposed
        string aveErrorTemp = strtok(line, delim);
        string maxErrorTemp = strtok(NULL, delim);
        // cout << "aveError: " << aveErrorTemp << "\tmaxError: " << maxErrorTemp << endl;

        std::vector<double> intervals;
        intervals.push_back(x0Start);
        intervals.push_back(x0End);
        intervals.push_back(x1Start);
        intervals.push_back(x1End);
        intervals.push_back(x2Start);
        intervals.push_back(x2End);

        tempError.intervals = intervals;
        tempError.aveError = atof(aveErrorTemp.c_str());
        tempError.maxError = atof(maxErrorTemp.c_str());
    }

    return tempError;
}
// for multi parameters, will be useless
exprInfo testError(string uniqueLabel, string suffix, const vector<double> &intervals)
{
    vector<string> parameters;
    string prefix = "expr_" + uniqueLabel;
    string middle;
    string commandStr = " " + uniqueLabel;
    for(size_t i = 0; i < intervals.size(); ++i)
    {
        auto temp = fmt::format("{}", intervals.at(i));
        parameters.push_back(temp);
        if(i == 0)
        {
            middle = temp;
        }
        else
        {
            middle = middle + "_" + temp;
        }
        commandStr = commandStr + " " + temp;
    }
    string fileNameKernel = prefix + "__" + middle + "_" + suffix;
    string testName = "./outputs/" + fileNameKernel + "_error.txt";
    string scriptName = "./detectErrorMultiPure.sh";
    commandStr = scriptName + commandStr + " " + prefix + " " + middle + " " + suffix;
    cout << "fileNameKernel: " << fileNameKernel << "\ncommand: " << commandStr << "\ntestName: " << testName << endl;
    char command[200] = {0};
    strcat(command, commandStr.c_str());
    system(command);
    std::ifstream ifs(testName, std::ios::in);

    double aveError = 0;
    double maxError = 0;
    exprInfo tempError;
    tempError.intervals = intervals;

    char ch;
    ifs >> ch;
    if(ifs.eof())
    {
        std::cout << "is null" << std::endl;
        ifs.close();

        tempError.aveError = aveError;
        tempError.maxError = maxError;
    }
    else
    {
        std::ifstream ifs(testName, std::ios::in);

        std::string lineStr;
        std::getline(ifs, lineStr);  // discard first line
        std::getline(ifs, lineStr);  // get the second line
        char *line = (char *)calloc(lineStr.length(), sizeof(char));
        strcpy(line, lineStr.c_str());
        const char *delim = " ,\t";  // Sets the delimiter for the string to be decomposed
        string aveErrorTemp = strtok(line, delim);
        string maxErrorTemp = strtok(NULL, delim);
        // cout << "aveError: " << aveErrorTemp << "\tmaxError: " << maxErrorTemp << endl;

        tempError.aveError = atof(aveErrorTemp.c_str());
        tempError.maxError = atof(maxErrorTemp.c_str());
    }

    return tempError;
}

exprInfo testError(string uniqueLabel, string suffix, const vector<double> &intervals, const vector<int> &scales, bool errfile)
{
    exprInfo tempError;
    size_t size = scales.size();

    if (size < 4)
    {
        string prefix = "expr_" + uniqueLabel;
        vector<string> params;
        for(const auto &interval : intervals)
        {
            auto paraTmp = fmt::format("{}", interval);
            params.push_back(paraTmp);
        }
        for(const auto &scale : scales)
        {
            auto scaleTmp = fmt::format("{}", scale);
            params.push_back(scaleTmp);
        }
        string middle;
        for(size_t i = 0; i < params.size(); ++i)
        {
            if(i == 0)
            {
                middle = params.at(i);
            }
            else
            {
                middle = middle + "_" + params.at(i);
            }
        }
        string fileNameKernel = prefix + "__" + middle + "_" + suffix;
        namespace fs = std::filesystem;
        string currentPath = fs::current_path();
        string testName = currentPath + "/outputs/" + uniqueLabel + "/" + fileNameKernel + "_error.txt"; // get the output of error detecting
        string number[3] = {"One", "Two", "Three"};
        string scriptName = "./detectError" + number[size - 1] + "FPEDParallel.sh";
        stringstream ss;
        ss << scriptName << " " << uniqueLabel;
        for(const auto & param : params)
        {
            ss << " " << param;
        }
        ss << " " << prefix << " " << middle << " " << suffix << " " << errfile;
        string commandStr = ss.str();

        // cout << "fileNameKernel: " << fileNameKernel << "\n";
        cout << "command: " << commandStr << "\n";
        // cout << "testName: " << testName << "\n";
        char command[512] = {0};
        strcat(command, commandStr.c_str());
        system(command);

        std::ifstream ifs(testName, std::ios::in);
        if(ifs.eof())
        {
            cout << "is null" << "\n";
            ifs.close();

            tempError.intervals = intervals;
            tempError.aveError = 0;
            tempError.maxError = 0;
        }
        else
        {
            std::string lineStr;
            std::getline(ifs, lineStr);  // discard first line
            std::getline(ifs, lineStr);  // get the second line
            char *line = (char *)calloc(lineStr.length(), sizeof(char));
            strcpy(line, lineStr.c_str());
            const char *delim = " ,\t";  // Sets the delimiter for the string to be decomposed
            string aveErrorTemp = strtok(line, delim);
            string maxErrorTemp = strtok(NULL, delim);
            // cout << "aveError: " << aveErrorTemp << "\tmaxError: " << maxErrorTemp << endl;

            tempError.intervals = intervals;
            tempError.aveError = atof(aveErrorTemp.c_str());
            tempError.maxError = atof(maxErrorTemp.c_str());
        }
        tempError.suffix = suffix;
        return tempError;
    }
    else if (size == 4)
    {
        tempError = testError(uniqueLabel, suffix, intervals);
    }
    else
    {
        fprintf(stderr, "WRONG: testError: the intervalTmp's dimension is %ld, which we don't support now.\n", size);
        exit(EXIT_FAILURE);
    }

    return tempError;
}

void sampleError(string uniqueLabel, string suffix, const vector<double> &intervals, const vector<int> &scales) {
    exprInfo tempError;
    size_t size = intervals.size() / 2;

    if (size < 4)
    {
        string prefix = "expr_" + uniqueLabel;
        vector<string> params;
        for(const auto &interval : intervals)
        {
            auto paraTmp = fmt::format("{}", interval);
            params.push_back(paraTmp);
        }
        for(const auto &scale : scales)
        {
            auto scaleTmp = fmt::format("{}", scale);
            params.push_back(scaleTmp);
        }
        string middle;
        for(size_t i = 0; i < params.size(); ++i)
        {
            if(i == 0)
            {
                middle = params.at(i);
            }
            else
            {
                middle = middle + "_" + params.at(i);
            }
        }
        string fileNameKernel = prefix + "__" + middle + "_" + suffix;
        namespace fs = std::filesystem;
        string currentPath = fs::current_path();
        string testName = currentPath + "/outputs/" + uniqueLabel + "/" + fileNameKernel + "_error.txt"; // used to get the output of error detecting, not input parameters
        string number[3] = {"One", "Two", "Three"};
        string scriptName = "./sampleError" + number[size - 1] + ".sh";
        stringstream ss;
        ss << scriptName << " " << uniqueLabel;
        for(const auto & param : params)
        {
            ss << " " << param;
        }
        ss << " " << prefix << " " << middle << " " << suffix;
        string commandStr = ss.str();

        // cout << "fileNameKernel: " << fileNameKernel << "\n";
        cout << "command: " << commandStr << "\n";
        // cout << "testName: " << testName << "\n";
        char command[200] = {0};
        strcat(command, commandStr.c_str());
        system(command);
    }
    else
    {
        fprintf(stderr, "ERROR: testError: the intervalTmp's dimension is %ld, which we don't support now.\n", size);
        exit(EXIT_FAILURE);
    }
}

double testPerformance(string uniqueLabel, string suffix, const vector<double> &intervals)
{
    size_t size = intervals.size() / 2;
    string prefix = "expr_" + uniqueLabel;
    string fileNameKernel = prefix + "_" + suffix;
    if (size < 4)
    {
        vector<string> params;
        for(const auto &interval : intervals)
        {
            auto paraTmp = fmt::format("{}", interval);
            params.push_back(paraTmp);
        }

        string number[3] = {"One", "Two", "Three"};
        string scriptName = "./testPerformance" + number[size - 1] + ".sh";
        string commandStr = "taskset -c 0 " +scriptName + " " + uniqueLabel + " " + suffix;
        for(const auto & param : params)
        {
            commandStr = commandStr + " " + param;
        }
        commandStr = "cd srcTest; " + commandStr + "; cd - > /dev/null;";
        // cout << "fileNameKernel: " << fileNameKernel << "\n";
        cout << "command: " << commandStr << "\n";
        char command[200] = {0};
        strcat(command, commandStr.c_str());
        system(command);
    }
    else if (size == 4)
    {
        fprintf(stderr, "WRONG: testPerformance: the intervalTmp's dimension is %ld, which we don't support now.\n", size);
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stderr, "WRONG: testPerformance: the intervalTmp's dimension is %ld, which we don't support now.\n", size);
        exit(EXIT_FAILURE);
    }

    namespace fs = std::filesystem;
    string currentPath = fs::current_path();
    string testName = currentPath + "/outputs/" + uniqueLabel + "/" + fileNameKernel + "_performance.txt";
    std::ifstream ifs(testName, std::ios::in);
    double tempCycles;
    if(ifs.eof())
    {
        std::cout << "is null" << std::endl;
        ifs.close();
        tempCycles = 0;
    }
    else
    {
        std::string lineStr;
        std::getline(ifs, lineStr);
        std::istringstream ss(lineStr);
        ss >> tempCycles;
    }

    return tempCycles;
}

// support setting matlab
string runCommand(string command)
{
    const int maxlen = 128;
    FILE *fp = NULL;
    char *buff = NULL;

    buff = (char *)malloc(maxlen);
    if (buff == NULL)
    {
        perror("malloc:");
        exit(EXIT_FAILURE);
    }
    memset(buff, 0, maxlen);

    fp = popen(command.c_str(), "r");
    if (fp == NULL)
    {
        perror("popen error:");
        free(buff);
        exit(EXIT_FAILURE);
    }
    fgets(buff, maxlen, fp);
    if (buff != nullptr && buff[0] == '\0')
    {
        fprintf(stderr, "there is no result returned by %s\n", command.c_str());
    }

    pclose(fp);
    string result = buff;
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    free(buff);
    return result;
}

// call matlab to generate the upEdgeData to file
string geneBoundaryData(string uniqueLabel, string suffix, double &costTime)
{
    char *currentPath;
    if((currentPath = getcwd(NULL, 0)) == NULL)
    {
        perror("getcwd error");
    }
    string currentPathStr(currentPath);
    string outputFilesPath = currentPathStr + "/outputs/" + uniqueLabel + "/";
    string sampleFileNameKernel = "sample_" + uniqueLabel + "_" + suffix + ".txt";
    string matlabExecutable = runCommand("which matlab"); // find matlab
    string matlabCommands = " -batch \"outputFilesPath=\'" + outputFilesPath + "\'; sampleFileNameKernel = \'" + sampleFileNameKernel + "\'; run " + currentPathStr + "/src/getUpEdge\""; // DO NOT need to add the suffix ".m" for matlab file!
    string commandStr = matlabExecutable + matlabCommands;
    cout << "Matlab command: " << commandStr << "\n";
    // cout << "length of commandStr: " << commandStr.size() << "\n";
    char command[512] = {0};
    strcat(command, commandStr.c_str());
    auto matlabKernelTime = runCommand(command);
    costTime = stod(matlabKernelTime);
    free(currentPath);
    // std::cout << "generate upEdge file:" << upEdgeFileName << std::endl;

    string upEdgeFileName = outputFilesPath + "upEdge_" + uniqueLabel + "_" + suffix + ".txt";
    return upEdgeFileName;
}

vector<string> geneBoundaryData(string uniqueLabel, string suffix, vector<string> suffixes, double &costTime)
{
    char *currentPath;
    if((currentPath = getcwd(NULL, 0)) == NULL)
    {
        perror("getcwd error");
    }
    string currentPathStr(currentPath);
    string outputFilesPath = currentPathStr + "/outputs/" + uniqueLabel + "/";
    string sampleFileNameKernel = "sample_" + uniqueLabel + "_" + suffix + "_" + "*.txt";
    string matlabExecutable = runCommand("which matlab"); // find matlab
    string matlabCommands = " -batch \"outputFilesPath=\'" + outputFilesPath + "\'; sampleFileNameKernel = \'" + sampleFileNameKernel + "\'; run " + currentPathStr + "/src/getUpEdge\""; // DO NOT need to add the suffix ".m" for matlab file!
    string commandStr = matlabExecutable + matlabCommands;
    cout << "Matlab command: " << commandStr << "\n";
    char command[512] = {0};
    strcat(command, commandStr.c_str());
    auto matlabKernelTime = runCommand(command);
    costTime = stod(matlabKernelTime);
    free(currentPath);
    vector<string> upEdgeFileNames;
    for(auto &suffix : suffixes)
    {
        string upEdgeFileName = outputFilesPath + "upEdge_" + uniqueLabel + "_" + suffix + ".txt";
        upEdgeFileNames.push_back(upEdgeFileName);
    }

    return upEdgeFileNames;
}

// string geneIntervalData1D(string upEdgeFileName, string uniqueLabel, double threshold) // generate the interval info to file at the target dimension
// {
//     // init
//     double thresholdCombine = 101;
//     string prefix = "./outputs/" + uniqueLabel + "/expr_" + uniqueLabel;
//     string suffix = "_interval.txt";
//     // call devideUpEdgeData for each dimension
//     string middle = "_" + to_string(threshold);
//     string intervalFileName = prefix + middle + suffix;
//     cout << "generate interval file:" << intervalFileName << endl;
//     devideUpEdgeData(upEdgeFileName, intervalFileName, threshold, thresholdCombine);
//     return intervalFileName;
// }

// vector<double> getIntervalData1D(string filename) // read filename to get vector<double>
// {
//     vector<double> intervalData;
//     std::ifstream ifs;
//     ifs.open(filename, std::ios::in);
//     if (!ifs.is_open())
//     {
//         std::cout << "ERROR: getIntervalData: fail to open " << filename << std::endl;
//         exit(EXIT_FAILURE);
//     }
//     string currentLine;
//     while (getline(ifs, currentLine, '\n'))
//     {
//         // cout << currentLine << endl;
//         std::istringstream iss(currentLine);
//         string tmp;
//         while(getline(iss, tmp, ' '))
//         {
//             intervalData.push_back(atof(tmp.c_str()));
//         }
//     }
//     ifs.close();
//     return intervalData;
// }

// vector<string> geneIntervalData(vector<string> upEdgeFileNames, string uniqueLabel, vector<double> &thresholds) // according the upEdgeFile data, generate the interval info into file
// {
//     // init
//     double thresholdCombine = 101;
//     vector<string> intervalFilenames;
//     string prefix = "./outputs/" + uniqueLabel + "/expr_" + uniqueLabel;
//     string suffix = "_interval.txt";
//     // call devideUpEdgeData for each dimension
//     for (size_t i = 0; i < thresholds.size(); i++)
//     {
//         auto &upEdgeFileName = upEdgeFileNames.at(i);
//         auto &threshold = thresholds.at(i);
//         string middle = "_" + to_string(threshold);
//         string intervalFileName = prefix + middle + suffix;
//         cout << "generate interval file:" << intervalFileName << endl;
//         intervalFilenames.push_back(intervalFileName);
//         devideUpEdgeData(upEdgeFileName, intervalFileName, threshold, thresholdCombine);
//     }
//     return intervalFilenames;
// }

// may useless, ready to comment it
// read filename to get vector<vector<double>>.
// A line is a vector<double>. lines are vector<~> (~ is vector<double>)
vector<vector<double>> getIntervalData(string filename)
{
    vector<vector<double>> intervalData;
    std::ifstream ifs;
    ifs.open(filename, std::ios::in);
    if (!ifs.is_open())
    {
        std::cout << "ERROR: getIntervalData: fail to open " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    string currentLine;
    while (getline(ifs, currentLine, '\n'))
    {
        // cout << currentLine << endl;
        vector<double> lineData;
        std::istringstream iss(currentLine);
        string tmp;
        while(getline(iss, tmp, ' '))
        {
            lineData.push_back(atof(tmp.c_str()));
        }
        intervalData.push_back(lineData);
    }
    ifs.close();
    return intervalData;
}

// according to the corresponding upEdgeFile, generate intervalData for each dimension, and then combine all permutation of all dimensions.
vector<vector<double>> getIntervalData(vector<string> upEdgeFileNames, vector<double> &thresholds, vector<double> &intervals, int &numIntervalsBefore)
{
    vector<vector<double>> intervalDataMultiDim;
    int dimension = thresholds.size();
    for (int i = 0; i < dimension; i++) // iterate all the dimensions
    {
        // call devideUpEdgeData for each dimension: matlab upEdge ==> interval vector
        auto &upEdgeFileName = upEdgeFileNames.at(i);
        auto &threshold = thresholds.at(i);
        auto thresholdCombine = (intervals.at(2 * i + 1) - intervals.at(2 * i)) / 100; // thresholdCombine = 1% of the interval width at the target demision
        auto intervalData1D = devideUpEdgeData(upEdgeFileName, threshold, numIntervalsBefore, thresholdCombine);
        // fmt::print("thresholdCombine: {}, intervalData1D {}\n", thresholdCombine, intervalData1D);
        intervalDataMultiDim.push_back(intervalData1D);
    }
    // call permuteMultiVec to get all permutation
    auto results = permuteMultiVec(intervalDataMultiDim);
    return results;
}

// call exprAuto to rewrite the input expression
vector<exprInfo> rewrite(string exprStr, string uniqueLabel, vector<vector<double>> &intervalData, int &numOfExprs, const vector<double> &intervals)
{
    // string filename = "expr_" + uniqueLabel + "_interval.txt"; // useless
    // string filename = "./intervalData.txt"; // TODO-done: get the filename from uniqueLabel. Now we get intervalData from input parameters directly.

    auto tempExpr = ParseExpressionFromString(exprStr);

    vector<exprInfo> exprInfoVector;

    // for (int i = 0; i < intervalData.size(); i++)
    size_t count = 0;
    for (auto &intervalTmp : intervalData)
    {
        size_t scale;
        auto dimension = intervalTmp.size() / 2;
        if (dimension == 1)
            scale = 500000;
        else if (dimension == 2)
            scale = 1024;
        else if (dimension == 3)
            scale = 256;
        else
            scale = 10;
        vector<int> scales(dimension, scale);
        for (int i = 0; i < dimension; i++)
        {
            double width = intervals.at(i * 2 + 1) - intervals.at(i * 2);
            double step = width / (double)scales.at(i);
            int stepNum = (intervalTmp.at(i * 2) - intervals.at(i * 2)) / step;
            int stepNum1 = (intervalTmp.at(i * 2 + 1) - intervals.at(i * 2)) / step;
            if(stepNum == stepNum1)
            {
                stepNum1 += 1;
            }
            scales.at(i) = stepNum1 - stepNum;

            intervalTmp.at(i * 2) = intervals.at(i * 2) + stepNum * step;
            intervalTmp.at(i * 2 + 1) = intervals.at(i * 2) + stepNum1 * step;
            // scales.at(i) = scales.at(i) * (intervalTmp.at(i * 2 + 1) - intervalTmp.at(i * 2)) / width;
        }
        auto newTempExprs = exprAutoWrapper(tempExpr, intervalTmp, scales);
        numOfExprs = newTempExprs.size();
        string suffix = "temp_" + std::to_string(count) + "_";

        // special conditions
        if(numOfExprs <= 1)
        {
            // auto tempError = testError(uniqueLabel, "mpfr", intervalTmp, scales); // use mpfr
            exprInfo tempInfo;
            tempInfo.intervals = intervalTmp;
            tempInfo.exprStr = "mpfr";
            tempInfo.aveError = 0;
            tempInfo.maxError = 0;
            tempInfo.rewriteID = -1;
            exprInfoVector.push_back(tempInfo);
            continue;
            // if(isEqual(newTempExprs.at(0), tempExpr))
            // {
            //     fprintf(stderr, "rewrite: only %d Equivalent expressions and is equal to inputexpr, so we do not rewrite.\n", newTempExprs.size());
            //     continue;
            // }
        }

        // pick the best rewrite expression
        string bestRewriteExpr;
        double maxError = -1;
        double aveError = 0;
        size_t maxIdx = -1;
        size_t jEnd = min(numOfExprs, 100000);
        // std::ofstream fTimeout;
        // fTimeout.open("./tmpTimeResult.txt");
        // if (!fTimeout.is_open())
        // {
        //     cout << "open tmpResult.txt failed";
        //     exit(EXIT_FAILURE);
        // }

        for (size_t j = 0; j < jEnd; j++)
        {
            cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-" << endl;
            string newExpr = PrintExpression(newTempExprs.at(j));
            cout << "No." << j << ": " << newExpr << endl;

            // generate function code and test error
            string suffixTmp = suffix + std::to_string(j);
            geneExprCode(newExpr, uniqueLabel, suffixTmp);
            // auto timeTmp1 = std::chrono::high_resolution_clock::now();
            auto tempError = testError(uniqueLabel, suffixTmp, intervalTmp, scales);
            // auto timeTmp2 = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> testError_seconds = timeTmp2 - timeTmp1;
            // cout << BLUE << "rewrite: For NO." << j << ": testError time: " << testError_seconds.count() << " s" << RESET << endl;
            // fTimeout << testError_seconds.count() << endl;
            if (j == 0)
            {
                maxError = tempError.maxError;
                bestRewriteExpr = newExpr;
                aveError = tempError.aveError;
                maxIdx = j;
            }
            else
            {
                if ((tempError.maxError < maxError) || ((tempError.maxError == maxError) && (tempError.aveError < aveError)))
                {
                    maxError = tempError.maxError;
                    bestRewriteExpr = newExpr;
                    aveError = tempError.aveError;
                    maxIdx = j;
                }
            }

            cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n"
                 << endl;
        }

        exprInfo tempInfo;
        // tempInfo.start = 0;
        // tempInfo.end = 1;
        tempInfo.intervals = intervalTmp;
        tempInfo.exprStr = bestRewriteExpr;
        // tempInfo.error = 0.1;
        tempInfo.aveError = aveError;
        tempInfo.maxError = maxError;
        // tempInfo.performance = 0.2;
        tempInfo.rewriteID = maxIdx;

        exprInfoVector.push_back(tempInfo);

        count++;
    }
    return exprInfoVector;
}

// permute multiple vector
vector<vector<double>> permuteMultiVec (vector<vector<double>> vec)
{
    vector<vector<double>> suzu;
    size_t row = vec.size();
    if (row == 3)
    {
        int rowSize = vec.size();      // 确定有几行
        int columnSize = 2 * rowSize;  // 新生成二维数组的列数
        int *b = new int[rowSize]();   // b[]存储每一行的元素个数
        int *f = new int[rowSize]();   // f[]中的每个元素是b[]中的0.5倍。即每一行有几对。
        int *d = new int[rowSize]();   // 建立数组，求e值
        for(size_t i = 0; i < vec.size(); i++)
        {
            b[i] = vec[i].size();
            f[i] = 0.5 * b[i];
            d[i] = 0.5 * b[i];
        }
        for (size_t i = 0; i < vec.size(); i++)
        {
            d[i + 1] = d[i] * d[i + 1];
        }
        int e = d[vec.size() - 1]; //新生成二维数组的行数为e
        double **sz;
        sz = new double *[e];
        for(int i = 0; i < e; i++)
        {
            sz[i] = new double[columnSize]();
        }

        for (int i = 0; i < e; i++) //确定前两列
        {
            int j = i % f[0];
            sz[i][0] = vec[0][2 * j];
            sz[i][1] = vec[0][2 * j + 1];
        }
        for (int i = 0; i < f[2]; i++) //确定中间两列
        {
            for (int j = 0; j < f[1]; j++)
            {
                for (int k = 0; k < f[0]; k++)
                {
                    sz[k + j * f[0] + i * f[0] * f[1]][2] = vec[1][2 * j];
                    sz[k + j * f[0] + i * f[0] * f[1]][3] = vec[1][2 * j + 1];
                }
            }
        }
        for (int i = 0; i < f[2]; i++) //确定最后两列
        {
            for (int j = 0; j < f[0] * f[1]; j++)
            {
                sz[j + i * f[0] * f[1]][4] = vec[2][2 * i];
                sz[j + i * f[0] * f[1]][5] = vec[2][2 * i + 1];
            }
        }
        for (int i = 0; i < e; i++)
        {
            vector<double> tmps;
            for (int j = 0; j < columnSize; j++)
            {
                tmps.push_back(sz[i][j]);
            }
            suzu.push_back(tmps);
        }

        delete [] b;
        delete [] f;
        delete [] d;
        for(int i = 0; i < e; i++)
        {
            delete[] sz[i];
        }
        delete[] sz;
    }
    else if (row == 2)
    {
        int rowSize = vec.size();     //确定有几行
        int columnSize = 2 * rowSize; //新生成二维数组的列数
        int *b = new int[rowSize]();   // b[]存储每一行的元素个数
        int *f = new int[rowSize]();   // f[]中的每个元素是b[]中的0.5倍。即每一行有几对。
        int *d = new int[rowSize]();   // 建立数组，求e值
        for (size_t i = 0; i < vec.size(); i++)
        {
            b[i] = vec[i].size();
            f[i] = 0.5 * b[i];
            d[i] = 0.5 * b[i];
        }
        for (size_t i = 0; i < vec.size(); i++)
        {
            d[i + 1] = d[i] * d[i + 1];
        }
        int e = d[vec.size() - 1]; //新生成二维数组的行数为e
        double **sz;
        sz = new double *[e];
        for(int i = 0; i < e; i++)
        {
            sz[i] = new double[2 * rowSize]();
        }
        for (int i = 0; i < e; i++) //确定前两列
        {
            int j = i % f[0];
            sz[i][0] = vec[0][2 * j];
            sz[i][1] = vec[0][2 * j + 1];
        }
        for (int i = 0; i < f[1]; i++) //确定最后两列
        {
            for (int j = 0; j < f[0]; j++)
            {
                sz[j + i * f[0]][2] = vec[1][2 * i];
                sz[j + i * f[0]][3] = vec[1][2 * i + 1];
            }
        }
        for (int i = 0; i < e; i++)
        {
            vector<double> tmpss;
            for (int j = 0; j < columnSize; j++)
            {
                tmpss.push_back(sz[i][j]);
            }
            suzu.push_back(tmpss);
        }

        delete [] b;
        delete [] f;
        delete [] d;
        for(int i = 0; i < e; i++)
        {
            delete[] sz[i];
        }
        delete[] sz;
    }
    else if (row == 1)
    {
        size_t rowSize = vec[0].size();
        int b = 0.5 * rowSize;
        double **sz;
        sz = new double *[b];
        for(int i = 0; i < b; i++)
        {
            sz[i] = new double[2]();
        }
        for (int i = 0; i < b; i++)
        {
            sz[i][0] = vec[0][2 * i];
            sz[i][1] = vec[0][2 * i + 1];
        }
        for (int i = 0; i < b; i++)
        {
            vector<double> tmps;
            for (int j = 0; j < 2; j++)
            {
                tmps.push_back(sz[i][j]);
            }
            suzu.push_back(tmps);
        }
        for(int i = 0; i < b; i++)
        {
            delete[] sz[i];
        }
        delete[] sz;
    }
    else
    {
        cout << "The prime group you entered does not conform to the specification!" << endl;
    }
    return suzu;
}