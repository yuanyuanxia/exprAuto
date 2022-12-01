#include "common.h"

#ifndef _MULTI
#define _MULTI

#ifndef PARA_NUM
#define PARA_NUM 1
#endif

#define DIMENSION PARA_NUM

#define INPUTX0 x0
#define INPUTX1 x1
#define INPUTX2 x2
#define INPUTX3 x3
#define INPUTX4 x4
#define INPUTX5 x5
#define INPUTX6 x6
#define INPUTX7 x7
#define INPUTX8 x8
#define INPUTX9 x9

#define PREFIX maxInput
#define MAXINPUTX0 ADDPREFIX(PREFIX, INPUTX0)
#define MAXINPUTX1 ADDPREFIX(PREFIX, INPUTX1)
#define MAXINPUTX2 ADDPREFIX(PREFIX, INPUTX2)
#define MAXINPUTX3 ADDPREFIX(PREFIX, INPUTX3)
#define MAXINPUTX4 ADDPREFIX(PREFIX, INPUTX4)
#define MAXINPUTX5 ADDPREFIX(PREFIX, INPUTX5)
#define MAXINPUTX6 ADDPREFIX(PREFIX, INPUTX6)
#define MAXINPUTX7 ADDPREFIX(PREFIX, INPUTX7)
#define MAXINPUTX8 ADDPREFIX(PREFIX, INPUTX8)
#define MAXINPUTX9 ADDPREFIX(PREFIX, INPUTX9)

#if PARA_NUM == 1
#define INPUT INPUTX0
#define PARA double INPUTX0
#define MAXINPUT_PARA double MAXINPUTX0
#define PARA_DEF double INPUTX0;
#define INPUTDATA_TO_INPUT INPUTX0 = inputData[idxData++];
#define INPUT_TO_MAXINPUT MAXINPUTX0 = INPUTX0;
#define MAXINPUT_PARA_DEF double MAXINPUTX0;
#define MAXINPUT_TO_ERR err.MAXINPUTX0 = MAXINPUTX0;
#define ERR_TO_MAXINPUT MAXINPUTX0 = errs[maxErrorIdx].MAXINPUTX0;
#define MAXINPUT_PRINT printf("max error input: [%lg]\n", MAXINPUTX0);

#elif PARA_NUM == 2
#define INPUT INPUTX0, INPUTX1
#define PARA double INPUTX0, double INPUTX1
#define MAXINPUT_PARA double MAXINPUTX0, double MAXINPUTX1
#define PARA_DEF    \
    double INPUTX0; \
    double INPUTX1;
#define INPUTDATA_TO_INPUT          \
    INPUTX0 = inputData[idxData++]; \
    INPUTX1 = inputData[idxData++];
#define INPUT_TO_MAXINPUT \
    MAXINPUTX0 = INPUTX0; \
    MAXINPUTX1 = INPUTX1;
#define MAXINPUT_PARA_DEF \
    double MAXINPUTX0;    \
    double MAXINPUTX1;
#define MAXINPUT_TO_ERR          \
    err.MAXINPUTX0 = MAXINPUTX0; \
    err.MAXINPUTX1 = MAXINPUTX1;
#define ERR_TO_MAXINPUT                        \
    MAXINPUTX0 = errs[maxErrorIdx].MAXINPUTX0; \
    MAXINPUTX1 = errs[maxErrorIdx].MAXINPUTX1;
#define MAXINPUT_PRINT printf("max error input: [%lg, %lg]\n", MAXINPUTX0, MAXINPUTX1);

#elif PARA_NUM == 3
#define INPUT INPUTX0, INPUTX1, INPUTX2
#define PARA double INPUTX0, double INPUTX1, double INPUTX2
#define MAXINPUT_PARA double MAXINPUTX0, double MAXINPUTX1, double MAXINPUTX2
#define PARA_DEF    \
    double INPUTX0; \
    double INPUTX1; \
    double INPUTX2;
#define INPUTDATA_TO_INPUT          \
    INPUTX0 = inputData[idxData++]; \
    INPUTX1 = inputData[idxData++]; \
    INPUTX2 = inputData[idxData++];
#define INPUT_TO_MAXINPUT \
    MAXINPUTX0 = INPUTX0; \
    MAXINPUTX1 = INPUTX1; \
    MAXINPUTX2 = INPUTX2;
#define MAXINPUT_PARA_DEF \
    double MAXINPUTX0;    \
    double MAXINPUTX1;    \
    double MAXINPUTX2;
#define MAXINPUT_TO_ERR          \
    err.MAXINPUTX0 = MAXINPUTX0; \
    err.MAXINPUTX1 = MAXINPUTX1; \
    err.MAXINPUTX2 = MAXINPUTX2;
#define ERR_TO_MAXINPUT                        \
    MAXINPUTX0 = errs[maxErrorIdx].MAXINPUTX0; \
    MAXINPUTX1 = errs[maxErrorIdx].MAXINPUTX1; \
    MAXINPUTX2 = errs[maxErrorIdx].MAXINPUTX2;
#define MAXINPUT_PRINT printf("max error input: [%lg, %lg, %lg]\n", MAXINPUTX0, MAXINPUTX1, MAXINPUTX2);

#elif PARA_NUM == 4
#define INPUT INPUTX0, INPUTX1, INPUTX2, INPUTX3
#define PARA double INPUTX0, double INPUTX1, double INPUTX2, double INPUTX3
#define MAXINPUT_PARA double MAXINPUTX0, double MAXINPUTX1, double MAXINPUTX2, double MAXINPUTX3
#define PARA_DEF    \
    double INPUTX0; \
    double INPUTX1; \
    double INPUTX2; \
    double INPUTX3;
#define INPUTDATA_TO_INPUT          \
    INPUTX0 = inputData[idxData++]; \
    INPUTX1 = inputData[idxData++]; \
    INPUTX2 = inputData[idxData++]; \
    INPUTX3 = inputData[idxData++];
#define INPUT_TO_MAXINPUT \
    MAXINPUTX0 = INPUTX0; \
    MAXINPUTX1 = INPUTX1; \
    MAXINPUTX2 = INPUTX2; \
    MAXINPUTX3 = INPUTX3;
#define MAXINPUT_PARA_DEF \
    double MAXINPUTX0;    \
    double MAXINPUTX1;    \
    double MAXINPUTX2;    \
    double MAXINPUTX3;
#define MAXINPUT_TO_ERR          \
    err.MAXINPUTX0 = MAXINPUTX0; \
    err.MAXINPUTX1 = MAXINPUTX1; \
    err.MAXINPUTX2 = MAXINPUTX2; \
    err.MAXINPUTX3 = MAXINPUTX3;
#define ERR_TO_MAXINPUT                        \
    MAXINPUTX0 = errs[maxErrorIdx].MAXINPUTX0; \
    MAXINPUTX1 = errs[maxErrorIdx].MAXINPUTX1; \
    MAXINPUTX2 = errs[maxErrorIdx].MAXINPUTX2; \
    MAXINPUTX3 = errs[maxErrorIdx].MAXINPUTX3;
#define MAXINPUT_PRINT printf("max error input: [%lg, %lg, %lg, %lg]\n", MAXINPUTX0, MAXINPUTX1, MAXINPUTX2, MAXINPUTX3);

#elif PARA_NUM == 5
#define INPUT INPUTX0, INPUTX1, INPUTX2, INPUTX3, INPUTX4
#define PARA double INPUTX0, double INPUTX1, double INPUTX2, double INPUTX3, double INPUTX4, double INPUTX5
#define MAXINPUT_PARA double MAXINPUTX0, double MAXINPUTX1, double MAXINPUTX2, double MAXINPUTX3, double MAXINPUTX4
#define PARA_DEF    \
    double INPUTX0; \
    double INPUTX1; \
    double INPUTX2; \
    double INPUTX3; \
    double INPUTX4;
#define INPUTDATA_TO_INPUT          \
    INPUTX0 = inputData[idxData++]; \
    INPUTX1 = inputData[idxData++]; \
    INPUTX2 = inputData[idxData++]; \
    INPUTX3 = inputData[idxData++]; \
    INPUTX4 = inputData[idxData++];
#define INPUT_TO_MAXINPUT \
    MAXINPUTX0 = INPUTX0; \
    MAXINPUTX1 = INPUTX1; \
    MAXINPUTX2 = INPUTX2; \
    MAXINPUTX3 = INPUTX3; \
    MAXINPUTX4 = INPUTX4;
#define MAXINPUT_PARA_DEF \
    double MAXINPUTX0;    \
    double MAXINPUTX1;    \
    double MAXINPUTX2;    \
    double MAXINPUTX3;    \
    double MAXINPUTX4;
#define MAXINPUT_TO_ERR          \
    err.MAXINPUTX0 = MAXINPUTX0; \
    err.MAXINPUTX1 = MAXINPUTX1; \
    err.MAXINPUTX2 = MAXINPUTX2; \
    err.MAXINPUTX3 = MAXINPUTX3; \
    err.MAXINPUTX4 = MAXINPUTX4;
#define ERR_TO_MAXINPUT                        \
    MAXINPUTX0 = errs[maxErrorIdx].MAXINPUTX0; \
    MAXINPUTX1 = errs[maxErrorIdx].MAXINPUTX1; \
    MAXINPUTX2 = errs[maxErrorIdx].MAXINPUTX2; \
    MAXINPUTX3 = errs[maxErrorIdx].MAXINPUTX3; \
    MAXINPUTX4 = errs[maxErrorIdx].MAXINPUTX4;
#define MAXINPUT_PRINT printf("max error input: [%lg, %lg, %lg, %lg, %lg]\n", MAXINPUTX0, MAXINPUTX1, MAXINPUTX2, MAXINPUTX3, MAXINPUTX4);

#elif PARA_NUM == 6
#define INPUT INPUTX0, INPUTX1, INPUTX2, INPUTX3, INPUTX4, INPUTX5
#define PARA double INPUTX0, double INPUTX1, double INPUTX2, double INPUTX3, double INPUTX4, double INPUTX5
#define MAXINPUT_PARA double MAXINPUTX0, double MAXINPUTX1, double MAXINPUTX2, double MAXINPUTX3, double MAXINPUTX4, double MAXINPUTX5
#define PARA_DEF    \
    double INPUTX0; \
    double INPUTX1; \
    double INPUTX2; \
    double INPUTX3; \
    double INPUTX4; \
    double INPUTX5;
#define INPUTDATA_TO_INPUT          \
    INPUTX0 = inputData[idxData++]; \
    INPUTX1 = inputData[idxData++]; \
    INPUTX2 = inputData[idxData++]; \
    INPUTX3 = inputData[idxData++]; \
    INPUTX4 = inputData[idxData++]; \
    INPUTX5 = inputData[idxData++];
#define INPUT_TO_MAXINPUT \
    MAXINPUTX0 = INPUTX0; \
    MAXINPUTX1 = INPUTX1; \
    MAXINPUTX2 = INPUTX2; \
    MAXINPUTX3 = INPUTX3; \
    MAXINPUTX4 = INPUTX4; \
    MAXINPUTX5 = INPUTX5;
#define MAXINPUT_PARA_DEF \
    double MAXINPUTX0;    \
    double MAXINPUTX1;    \
    double MAXINPUTX2;    \
    double MAXINPUTX3;    \
    double MAXINPUTX4;    \
    double MAXINPUTX5;
#define MAXINPUT_TO_ERR          \
    err.MAXINPUTX0 = MAXINPUTX0; \
    err.MAXINPUTX1 = MAXINPUTX1; \
    err.MAXINPUTX2 = MAXINPUTX2; \
    err.MAXINPUTX3 = MAXINPUTX3; \
    err.MAXINPUTX4 = MAXINPUTX4; \
    err.MAXINPUTX5 = MAXINPUTX5;
#define ERR_TO_MAXINPUT                        \
    MAXINPUTX0 = errs[maxErrorIdx].MAXINPUTX0; \
    MAXINPUTX1 = errs[maxErrorIdx].MAXINPUTX1; \
    MAXINPUTX2 = errs[maxErrorIdx].MAXINPUTX2; \
    MAXINPUTX3 = errs[maxErrorIdx].MAXINPUTX3; \
    MAXINPUTX4 = errs[maxErrorIdx].MAXINPUTX4; \
    MAXINPUTX5 = errs[maxErrorIdx].MAXINPUTX5;
#define MAXINPUT_PRINT printf("max error input: [%lg, %lg, %lg, %lg, %lg, %lg]\n", MAXINPUTX0, MAXINPUTX1, MAXINPUTX2, MAXINPUTX3, MAXINPUTX4, MAXINPUTX5);

#elif PARA_NUM == 7
#define INPUT INPUTX0, INPUTX1, INPUTX2, INPUTX3, INPUTX4, INPUTX5, INPUTX6
#define PARA double INPUTX0, double INPUTX1, double INPUTX2, double INPUTX3, double INPUTX4, double INPUTX5, double INPUTX6
#define MAXINPUT_PARA double MAXINPUTX0, double MAXINPUTX1, double MAXINPUTX2, double MAXINPUTX3, double MAXINPUTX4, double MAXINPUTX5, double MAXINPUTX6
#define PARA_DEF    \
    double INPUTX0; \
    double INPUTX1; \
    double INPUTX2; \
    double INPUTX3; \
    double INPUTX4; \
    double INPUTX5; \
    double INPUTX6;
#define INPUTDATA_TO_INPUT          \
    INPUTX0 = inputData[idxData++]; \
    INPUTX1 = inputData[idxData++]; \
    INPUTX2 = inputData[idxData++]; \
    INPUTX3 = inputData[idxData++]; \
    INPUTX4 = inputData[idxData++]; \
    INPUTX5 = inputData[idxData++]; \
    INPUTX6 = inputData[idxData++];
#define INPUT_TO_MAXINPUT \
    MAXINPUTX0 = INPUTX0; \
    MAXINPUTX1 = INPUTX1; \
    MAXINPUTX2 = INPUTX2; \
    MAXINPUTX3 = INPUTX3; \
    MAXINPUTX4 = INPUTX4; \
    MAXINPUTX5 = INPUTX5; \
    MAXINPUTX6 = INPUTX6;
#define MAXINPUT_PARA_DEF \
    double MAXINPUTX0;    \
    double MAXINPUTX1;    \
    double MAXINPUTX2;    \
    double MAXINPUTX3;    \
    double MAXINPUTX4;    \
    double MAXINPUTX5;    \
    double MAXINPUTX6;
#define MAXINPUT_TO_ERR          \
    err.MAXINPUTX0 = MAXINPUTX0; \
    err.MAXINPUTX1 = MAXINPUTX1; \
    err.MAXINPUTX2 = MAXINPUTX2; \
    err.MAXINPUTX3 = MAXINPUTX3; \
    err.MAXINPUTX4 = MAXINPUTX4; \
    err.MAXINPUTX5 = MAXINPUTX5; \
    err.MAXINPUTX6 = MAXINPUTX6;
#define ERR_TO_MAXINPUT                        \
    MAXINPUTX0 = errs[maxErrorIdx].MAXINPUTX0; \
    MAXINPUTX1 = errs[maxErrorIdx].MAXINPUTX1; \
    MAXINPUTX2 = errs[maxErrorIdx].MAXINPUTX2; \
    MAXINPUTX3 = errs[maxErrorIdx].MAXINPUTX3; \
    MAXINPUTX4 = errs[maxErrorIdx].MAXINPUTX4; \
    MAXINPUTX5 = errs[maxErrorIdx].MAXINPUTX5; \
    MAXINPUTX6 = errs[maxErrorIdx].MAXINPUTX6;
#define MAXINPUT_PRINT \
    printf("max error input: [%lg, %lg, %lg, %lg, %lg, %lg, %lg]\n", MAXINPUTX0, MAXINPUTX1, MAXINPUTX2, MAXINPUTX3, MAXINPUTX4, MAXINPUTX5, MAXINPUTX6);

#elif PARA_NUM == 8
#define INPUT INPUTX0, INPUTX1, INPUTX2, INPUTX3, INPUTX4, INPUTX5, INPUTX6, INPUTX7
#define PARA double INPUTX0, double INPUTX1, double INPUTX2, double INPUTX3, double INPUTX4, double INPUTX5, double INPUTX6, double INPUTX7
#define MAXINPUT_PARA \
    double MAXINPUTX0, double MAXINPUTX1, double MAXINPUTX2, double MAXINPUTX3, double MAXINPUTX4, double MAXINPUTX5, double MAXINPUTX6, double MAXINPUTX7
#define PARA_DEF    \
    double INPUTX0; \
    double INPUTX1; \
    double INPUTX2; \
    double INPUTX3; \
    double INPUTX4; \
    double INPUTX5; \
    double INPUTX6; \
    double INPUTX7;
#define INPUTDATA_TO_INPUT          \
    INPUTX0 = inputData[idxData++]; \
    INPUTX1 = inputData[idxData++]; \
    INPUTX2 = inputData[idxData++]; \
    INPUTX3 = inputData[idxData++]; \
    INPUTX4 = inputData[idxData++]; \
    INPUTX5 = inputData[idxData++]; \
    INPUTX6 = inputData[idxData++]; \
    INPUTX7 = inputData[idxData++];
#define INPUT_TO_MAXINPUT \
    MAXINPUTX0 = INPUTX0; \
    MAXINPUTX1 = INPUTX1; \
    MAXINPUTX2 = INPUTX2; \
    MAXINPUTX3 = INPUTX3; \
    MAXINPUTX4 = INPUTX4; \
    MAXINPUTX5 = INPUTX5; \
    MAXINPUTX6 = INPUTX6; \
    MAXINPUTX7 = INPUTX7;
#define MAXINPUT_PARA_DEF \
    double MAXINPUTX0;    \
    double MAXINPUTX1;    \
    double MAXINPUTX2;    \
    double MAXINPUTX3;    \
    double MAXINPUTX4;    \
    double MAXINPUTX5;    \
    double MAXINPUTX6;    \
    double MAXINPUTX7;
#define MAXINPUT_TO_ERR          \
    err.MAXINPUTX0 = MAXINPUTX0; \
    err.MAXINPUTX1 = MAXINPUTX1; \
    err.MAXINPUTX2 = MAXINPUTX2; \
    err.MAXINPUTX3 = MAXINPUTX3; \
    err.MAXINPUTX4 = MAXINPUTX4; \
    err.MAXINPUTX5 = MAXINPUTX5; \
    err.MAXINPUTX6 = MAXINPUTX6; \
    err.MAXINPUTX7 = MAXINPUTX7;
#define ERR_TO_MAXINPUT                        \
    MAXINPUTX0 = errs[maxErrorIdx].MAXINPUTX0; \
    MAXINPUTX1 = errs[maxErrorIdx].MAXINPUTX1; \
    MAXINPUTX2 = errs[maxErrorIdx].MAXINPUTX2; \
    MAXINPUTX3 = errs[maxErrorIdx].MAXINPUTX3; \
    MAXINPUTX4 = errs[maxErrorIdx].MAXINPUTX4; \
    MAXINPUTX5 = errs[maxErrorIdx].MAXINPUTX5; \
    MAXINPUTX6 = errs[maxErrorIdx].MAXINPUTX6; \
    MAXINPUTX7 = errs[maxErrorIdx].MAXINPUTX7;
#define MAXINPUT_PRINT                                                                                                                                        \
    printf("max error input: [%lg, %lg, %lg, %lg, %lg, %lg, %lg, %lg]\n", MAXINPUTX0, MAXINPUTX1, MAXINPUTX2, MAXINPUTX3, MAXINPUTX4, MAXINPUTX5, MAXINPUTX6, \
           MAXINPUTX7);

#elif PARA_NUM == 9
#define INPUT INPUTX0, INPUTX1, INPUTX2, INPUTX3, INPUTX4, INPUTX5, INPUTX6, INPUTX7, INPUTX8
#define PARA double INPUTX0, double INPUTX1, double INPUTX2, double INPUTX3, double INPUTX4, double INPUTX5, double INPUTX6, double INPUTX7, double INPUTX8
#define MAXINPUT_PARA                                                                                                                                       \
    double MAXINPUTX0, double MAXINPUTX1, double MAXINPUTX2, double MAXINPUTX3, double MAXINPUTX4, double MAXINPUTX5, double MAXINPUTX6, double MAXINPUTX7, \
        double MAXINPUTX8
#define PARA_DEF    \
    double INPUTX0; \
    double INPUTX1; \
    double INPUTX2; \
    double INPUTX3; \
    double INPUTX4; \
    double INPUTX5; \
    double INPUTX6; \
    double INPUTX7; \
    double INPUTX8;
#define INPUTDATA_TO_INPUT          \
    INPUTX0 = inputData[idxData++]; \
    INPUTX1 = inputData[idxData++]; \
    INPUTX2 = inputData[idxData++]; \
    INPUTX3 = inputData[idxData++]; \
    INPUTX4 = inputData[idxData++]; \
    INPUTX5 = inputData[idxData++]; \
    INPUTX6 = inputData[idxData++]; \
    INPUTX7 = inputData[idxData++]; \
    INPUTX8 = inputData[idxData++];
#define INPUT_TO_MAXINPUT \
    MAXINPUTX0 = INPUTX0; \
    MAXINPUTX1 = INPUTX1; \
    MAXINPUTX2 = INPUTX2; \
    MAXINPUTX3 = INPUTX3; \
    MAXINPUTX4 = INPUTX4; \
    MAXINPUTX5 = INPUTX5; \
    MAXINPUTX6 = INPUTX6; \
    MAXINPUTX7 = INPUTX7; \
    MAXINPUTX8 = INPUTX8;
#define MAXINPUT_PARA_DEF \
    double MAXINPUTX0;    \
    double MAXINPUTX1;    \
    double MAXINPUTX2;    \
    double MAXINPUTX3;    \
    double MAXINPUTX4;    \
    double MAXINPUTX5;    \
    double MAXINPUTX6;    \
    double MAXINPUTX7;    \
    double MAXINPUTX8;
#define MAXINPUT_TO_ERR          \
    err.MAXINPUTX0 = MAXINPUTX0; \
    err.MAXINPUTX1 = MAXINPUTX1; \
    err.MAXINPUTX2 = MAXINPUTX2; \
    err.MAXINPUTX3 = MAXINPUTX3; \
    err.MAXINPUTX4 = MAXINPUTX4; \
    err.MAXINPUTX5 = MAXINPUTX5; \
    err.MAXINPUTX6 = MAXINPUTX6; \
    err.MAXINPUTX7 = MAXINPUTX7; \
    err.MAXINPUTX8 = MAXINPUTX8;
#define ERR_TO_MAXINPUT                        \
    MAXINPUTX0 = errs[maxErrorIdx].MAXINPUTX0; \
    MAXINPUTX1 = errs[maxErrorIdx].MAXINPUTX1; \
    MAXINPUTX2 = errs[maxErrorIdx].MAXINPUTX2; \
    MAXINPUTX3 = errs[maxErrorIdx].MAXINPUTX3; \
    MAXINPUTX4 = errs[maxErrorIdx].MAXINPUTX4; \
    MAXINPUTX5 = errs[maxErrorIdx].MAXINPUTX5; \
    MAXINPUTX6 = errs[maxErrorIdx].MAXINPUTX6; \
    MAXINPUTX7 = errs[maxErrorIdx].MAXINPUTX7; \
    MAXINPUTX8 = errs[maxErrorIdx].MAXINPUTX8;
#define MAXINPUT_PRINT                                                                                                                                 \
    printf("max error input: [%lg, %lg, %lg, %lg, %lg, %lg, %lg, %lg, %lg]\n", MAXINPUTX0, MAXINPUTX1, MAXINPUTX2, MAXINPUTX3, MAXINPUTX4, MAXINPUTX5, \
           MAXINPUTX6, MAXINPUTX7, MAXINPUTX8);
#endif

#endif