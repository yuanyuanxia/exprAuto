#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
clock_t start;
clock_t end;

using namespace std;
#define DEBUG
#undef DEBUG

int read_scanf(const string &filename, const int &cols, vector<double *> &_vector, const double &ulp) {
    FILE *fp = fopen(filename.c_str(), "r");
    bool flag = true;
    double tmp;
    if (!fp) {
        cout << "File open error!\n";
        return 0;
    }
    while (flag) {
        //将txt文本文件中的一行数据存入rowArray中，并将rowArray存入vector中
        double *rowArray = new double[cols]; // new一个double类型的动态数组
        if (EOF == fscanf(fp, "%le", &rowArray[0])) { //读取数据，存在_vector[cols]中
            break;
        }
        if (EOF == fscanf(fp, "%le", &rowArray[1])) {
            break;
        }
        if (EOF == fscanf(fp, "%le", &tmp)) {
            break;
        }
        // if (tmp < ulp) {
        //     tmp = ulp;
        // }
        rowArray[2] = tmp;
        _vector.push_back(rowArray);
    }
    fclose(fp);
    return 1;
}

//将采样点对应的行号/序号、坐标值、误差、距离存入vector中
void storeData(double rows, double x, double ulpErr, double distance, vector<double *> &points) {
    double *point = new double[4];
    point[0] = rows;
    point[1] = x;
    point[2] = ulpErr;
    point[3] = distance;
    points.push_back(point);
}

//打印区间数据
void printRegime(const int &num, vector<double *> &vector, string fintervalName, string fdistanceName, string fwidthName) {
    ofstream finterval, fdistance, fwidth;
    finterval.open(fintervalName);
    fdistance.open(fdistanceName);
    fwidth.open(fwidthName);
    for (int i = 0; i < num; i += 2) {
        // if (vector[i][1] == vector[i + 1][1]) {
        //     continue; // 注释掉后，单个突出也会记录
        // }
        // finterval << "lineNo: [" << vector[i][0] << ", " << vector[i + 1][0] << "]; input: " << "[" << vector[i][1] << ", " << vector[i + 1][1] << "]" << endl;
        finterval << "[" << vector[i][1] << ", " << vector[i + 1][1] << "]\n"; // print intervals data to file fintervalName
        // fdistance << vector[i][3] << endl; // print intervals' distance data to file "distanceTmp.txt"
        // fwidth << vector[i+1][3] << endl;  // print intervals' width data to file "widthTmp.txt"
    }
    finterval.close();
    fdistance.close();
    fwidth.close();
}

void ifRegimeulp(const int &num, vector<double *> &vector, std::vector<double *> &store_vector, const double &threshold) {
    double distance;
    double k, b, tmp;

    for (int i = 0; i < num - 1; ++i) {
        // find the interval start point and set the distance value. The distance value is the distance to the previous interval
        if (vector[i][2] > threshold) {
            if (i == 0) {
                // if 1st interval, the distance value is 0
                distance = 0;
                storeData(0, vector[i][1], vector[i][2], distance, store_vector);
            } else if (vector[i - 1][2] <= threshold) {
                if(store_vector.size() == 0) {
                    distance = 0;
                } else {
                    distance = i - store_vector.back()[0];
                }
                k = (vector[i][1] - vector[i - 1][1]) / (vector[i][2] - vector[i - 1][2]);
                b = vector[i][1] - k * vector[i][2];
                tmp = k * threshold + b;
                // storeData(i, vector[i][1], vector[i][2], distance, store_vector); // 保存的是误差大于阈值的最左边点
                storeData(i - 0.5, tmp, threshold, distance, store_vector); // 保存的左端点连线与阈值横线的交点，这也是为何i-0.5
            }
        }
        // find the interval end point and set the distance value. The distance value is the width of the current interval
        if (vector[i][2] > threshold) {
            if (vector[i + 1][2] <= threshold) {
                distance = i - store_vector.back()[0];
                k = (vector[i + 1][1] - vector[i][1]) / (vector[i + 1][2] - vector[i][2]);
                b = vector[i][1] - k * vector[i][2];
                tmp = k * threshold + b;
                // storeData(i, vector[i][1], vector[i][2], distance, store_vector); // 保存的是误差大于阈值的最右边点
                storeData(i + 0.5, tmp, threshold, distance, store_vector); // 保存的是最右边点与阈值横线的交点，这也是为何i+0.5
            }
            else if (i == num - 2) {
                // the last point's ULP error > threshold, so store the last point, that is vector[num - 1]
                distance = i - store_vector.back()[0];
                storeData(num - 1, vector[num - 1][1], vector[num - 1][2], distance, store_vector);
            }
        }
    }
}

// 函数的轮廓线数据，即每个轮廓点对应的输入、误差，是基于扩展后的投影面误差采样数据，利用matlab的boundary函数得到的，保存在matlab_vector向量中。
// 基于轮廓线数据，调用ifRegimeulp函数，初步得到精度优化区间的起止点信息，保存在store_vector向量中。
// store_vector  保存的是精度优化区间的起止点信息，即其在matlab_vector向量中对应的序号、输入、误差、距离（matlab序号）。
// store_vector  每相邻的2个元素，分别对应区间的起点和止点信息，构成一个精度优化区间。每个元素中又有3个信息，即其在matlab_vector向量中对应的序号、输入、误差。
// output_vector 保存的是精度优化区间的起止点信息，即其在matlab_vector向量中对应的序号、输入、误差、距离（真实输入）。
int findMinMax(vector<double *> &matlab_vector, vector<double *> &store_vector, vector<double *> &output_vector) {
    int vectorNum = store_vector.size();
    double start, end, startInput, endInput, currentInput, startError, endError, distance;
    double maxInput, minInput, lastmaxInput, minIdx, maxIdx;

    for (int i = 0; i < vectorNum; i += 2) {
        start = store_vector[i][0];
        end = store_vector[i+1][0];
        startInput = store_vector[i][1];
        endInput = store_vector[i+1][1];
        startError = store_vector[i][2];
        endError = store_vector[i+1][2];
        minIdx = start;
        maxIdx = end;
        minInput = startInput;
        maxInput = endInput;

        // Considering that the boundary line may protrude on both sides, the start and end points stored in 'store_vector' that intersect with the threshold are not necessarily the endpoints (leftmost and rightmost points) of the interval.
        // Therefore, it is necessary to traverse the coordinate information of all points within the interval and update the endpoints of the interval to the leftmost and rightmost points found.
        // 考虑到轮廓线可能向两边突出去，所以store_vector中存储的与阈值相交的起止点并不一定是区间的端点（最左最右点）。因此，需要遍历区间内所有点的坐标信息，将区间端点更新为找到的最小值和最大值。
        for (int j = ceil(start); j <= floor(end); j++) {
            currentInput = matlab_vector[j][1];
            if(currentInput < minInput) {
                minIdx = j;
                minInput = currentInput;
            } else if(currentInput > maxInput) {
                maxIdx = j;
                maxInput = currentInput;
            }
        }
        // store the start point to output vector
        if(minInput != startInput) {
            #ifdef DEBUG
            printf("\tFor No.%d: minInput != startInput. startInput: %g, minInput: %g.\n", i / 2, startInput, minInput);
            #endif
            startInput = matlab_vector[minIdx][1];
            startError = matlab_vector[minIdx][2];
        }
        // 更新distance，即当前区间左端点到其右侧区间右端点的距离
        if(i != 0) {
            distance = minInput - lastmaxInput;
        } else {
            distance = 0;
        }
        storeData(minIdx, startInput, startError, distance, output_vector);
        // store the end point to output vector
        if(maxInput != endInput) {
            #ifdef DEBUG
            printf("\tFor No.%d: maxInput != endInput. endInput: %g, maxInput: %g.\n", i / 2, endInput, maxInput);
            #endif
            endInput = matlab_vector[maxIdx][1];
            endError = matlab_vector[maxIdx][2];
        }
        distance = endInput - startInput;
        storeData(maxIdx, endInput, endError, distance, output_vector);

        // update the lastmaxInput
        lastmaxInput = maxInput;

        // cout << i / 2 << ": " << minIdx << " " << minInput << " " << startInput << " " << maxIdx << " " << maxInput << " " << endInput << endl;
        // cout << "No." << i/2+1 << ": the index is [" << minInput << ", " << maxInput << "], the corresponding input is [" << startInput << ", " << endInput << "]" << endl;
    }
    
    // the output vector is from edge, a Matlab variable which stores the index, the input and the error of the sample data
    // minIdx is the index of the element in the output vector.
    // minInput is the index of the element in the sample data.
    // startInput is the corresponding input value in the sample data.
    // cout << "i: minIdx minInput startInput maxIdx maxInput endInput" << endl;
    // cout << i << ": " << minIdx << " " << minInput << " " << startInput << " " << maxIdx << " " << maxInput << " " << endInput << endl;
    // cout << i << ": the output vector index is " << minIdx << " " << maxIdx << endl;
    // cout << i << ": the sampleData index is " << minInput << " " << maxInput << endl;
    // cout << i << ": the corresponding input to the new sampleData index is " << startInput << " " << endInput << endl;
    // cout << i << ": the width is " << maxInput - minInput + 1 << "\n" << endl;

    return 1;
}

// add_zzy interval_merge
// TODO-done: Just compare the input value but not the idx of input. Because the idx is for the boundary line which is useless.
void interval_merge(vector<double *> in_vector, vector<double *> &out_vector, const double &thresholdCombine) {
    if(in_vector.size() == 0) {
        cout << "NOTE: interval_merge: none intervals" << endl;
        return;
    }
    if (in_vector.size() == 2) {
        double *temp = in_vector[0];
        out_vector.push_back(temp);
        double *temp1 = in_vector[1];
        out_vector.push_back(temp1);
        return;
    }
    double *temp = in_vector.front();
    out_vector.push_back(temp);
    // cout << out_vector.size() << endl;
    for (int i = 2; i < in_vector.size(); i += 2) {
        // cout << i << " " << out_vector.size() << endl;
        // if (in_vector.at(i)[3] < thresholdCombine && in_vector.at(i - 1)[3] > thresholdCombine && in_vector.at(i + 1)[3] > thresholdCombine) {
        if (in_vector.at(i)[3] < thresholdCombine) {
            if (i == in_vector.size() - 2) {
                temp = in_vector.at(i + 1);
                out_vector.push_back(temp);
            } else {
                continue;
            }
        } else {
            temp = in_vector.at(i-1);
            out_vector.push_back(temp);
            temp = in_vector.at(i);
            out_vector.push_back(temp);
            // temp = in_vector.at(i+1);
            // out_vector.push_back(temp);
            if (i == in_vector.size() - 2) {
                temp = in_vector.at(i + 1);
                out_vector.push_back(temp);
            }
        }
    }
    // cout << out_vector.size() << endl;
    // for (int i = 0; i < out_vector.size(); i += 2) {
    //     cout << "[";
    //     cout << out_vector.at(i)[1] << "," << out_vector.at(i + 1)[1] << "]" <<endl;
    // }
    // for (int i = 0; i < out_vector.size(); ++i) {
    //     for (int j = 0; j < 4; ++j) {
    //         cout << out_vector[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // printRegime(out_vector.size(), out_vector);
    return;
}

vector<double> extractInfo(vector<double *> &origin) {
    vector<double> destination;
    for (size_t i = 0; i < origin.size(); i++) {
        destination.push_back(origin[i][1]);
    }
    return destination;
}

vector<double> devideUpEdgeData(string upEdgeFileName, double threshold, double thresholdCombine) {
    vector<double> intervalData;
    // thresholdCombine = 0; // TODO: set thresholdCombine
    // upEdgeFileName 文件中有3列，分别是序号、输入、误差。该文件是由matlab生成，故文件中的序号从1开始，而非0。
    // matlab_vector可视为二维数组，保存的是 upEdgeFileName 中的边界点数据，包括序号、输入、误差;输出数组元素：  
    int columns = 3;
    vector<double *> matlab_vector;
    if (!read_scanf(upEdgeFileName, columns, matlab_vector, threshold)) {
        fprintf(stderr, "ERROR: devideUpEdgeData: read file error\n");
        exit(EXIT_FAILURE);
    }
    
    // 初步获取区间信息
    vector<double *> store_vector;
    ifRegimeulp(matlab_vector.size(), matlab_vector, store_vector, threshold);
    // printRegime(store_vector.size(), store_vector, "intervalTmp.txt", "distanceTmp.txt", "widthTmp.txt");
    
    // 进一步优化区间信息
    vector<double *> output_vector;
    findMinMax(matlab_vector, store_vector, output_vector);
    // printRegime(output_vector.size(), output_vector, "intervalTmp_beforeCombine.txt", "distanceTmp_beforeCombine.txt", "widthTmp_beforeCombine.txt");
    
    // Merge intervals that are close in distance.
    vector<double *> merge_vector;
    interval_merge(output_vector, merge_vector, thresholdCombine);
    // printRegime(merge_vector.size(), merge_vector, intervalFileName, "distanceTmp_afterCombine.txt", "widthTmp_afterCombine.txt");

    // generate the interval data vector
    intervalData = extractInfo(merge_vector);
    return intervalData;
}

// void devideKernel(const string &filePath, const string &intervalFileName, const double &threshold, const double &thresholdCombine) {
//     // filePath文件中有3列，分别是序号、输入、误差。该文件是由matlab生成，故文件中的序号从1开始，而非0。
//     // matlab_vector可视为二维数组，保存的是filePath中的边界点数据，包括序号、输入、误差;输出数组元素：  
//     vector<double *> matlab_vector;
//     int columns = 3;
//     if (!read_scanf(filePath, columns, matlab_vector, threshold)) {
//         return;
//     }    
//     // 初步获取区间信息
//     vector<double *> store_vector;
//     ifRegimeulp(matlab_vector.size(), matlab_vector, store_vector, threshold);
//     // printRegime(store_vector.size(), store_vector, "intervalTmp.txt", "distanceTmp.txt", "widthTmp.txt");
//     // 进一步优化区间信息
//     vector<double *> output_vector;
//     findMinMax(matlab_vector, store_vector, output_vector);
//     // printRegime(output_vector.size(), output_vector, "intervalTmp_beforeCombine.txt", "distanceTmp_beforeCombine.txt", "widthTmp_beforeCombine.txt");  
//     // Merge intervals that are close in distance.
//     vector<double *> merge_vector;
//     interval_merge(output_vector, merge_vector, thresholdCombine);
//     printRegime(merge_vector.size(), merge_vector, intervalFileName, "distanceTmp_afterCombine.txt", "widthTmp_afterCombine.txt");
// }
// void devideUpEdgeData(string upEdgeFileName, string intervalFileName, double threshold, double thresholdCombine) {
//     // cout << upEdgeFileName << " " << threshold << "\n";
//     // start = clock();
//     devideKernel(upEdgeFileName, intervalFileName, threshold, thresholdCombine);
//     // end = clock();
//     // double endtime = (double)(end - start)/CLOCKS_PER_SEC;
//     // cout << "Total time:" << endtime << endl;
//     // cout << "Total time:" << endtime * 1000 << "ms" << endl;
// }