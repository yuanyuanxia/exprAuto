#include "common.h"
#include "mpi.h"

#ifndef SUFFIX
#define SUFFIX orgin
#endif
#ifndef EXPRESSION
#define EXPRESSION sum
#endif

#define EXPRESSIONMINE ADDSUFFIX(EXPRESSION, SUFFIX)
#define SUFFIX1 mpfr
#define EXPRESSIONMPFR ADDSUFFIX(EXPRESSION, SUFFIX1)

#define TESTNUMX 2048
#define TESTNUMY 2048

double EXPRESSIONMPFR(double, double, mpfr_t);
double EXPRESSIONMINE(double, double);

int computeOrcle2param(double x0, double x1, mpfr_t orcle)
{
    return EXPRESSIONMPFR(x0, x1, orcle);
}

int computeResult2param(double x0, double x1, mpfr_t mpfrResult)
{
    int status = 1;
    
    double result = EXPRESSIONMINE(x0, x1);
    mpfr_set_d(mpfrResult, result, MPFR_RNDN);

    return status;
}

void sample2paramX(DL xStart, DL xEnd, DL yStart, DL yEnd, unsigned long int testNumX, unsigned long int testNumY, const char* uniqueLabel, const char *fileNameKernel, int myid, int iStartLocal, int iEndLocal)
{
    DL i, j;
    // DL maxInputX, maxInputY, orcle, result;
    int ii, jj;
    double x, y, reUlp, maxReUlp, lenX, lenY;

    // mpfr
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // write error data to file
    char *directory = "outputs";
    char *suffix = "sample_X";
    char *fileNameSample;
    FILE *f;
    fileNameSample = (char *) calloc(strlen(directory) + strlen(uniqueLabel) + strlen(fileNameKernel) + strlen(suffix) + 128, sizeof(char));
    sprintf(fileNameSample, "%s/%s/%s_%s_%02d.txt", directory, uniqueLabel, fileNameKernel, suffix, myid);
    // printf("%s\n", fileNameSample);
    if ((f = fopen(fileNameSample, "w")) == NULL) {
        printf("Error opening file %s.\n", fileNameSample);
        exit(0);
    }

    // loop boundary
    lenX = xEnd.d - xStart.d;
    lenY = yEnd.d - yStart.d;
    double stepX = lenX / (double)testNumX;
    double stepY = lenY / (double)testNumY;
    
    // Real number average
    // 固定x值，求不同y值下的最大误差
    for(ii = iStartLocal; ii <= iEndLocal; ii++) {
        i.d = xStart.d + stepX * ii;
        x = i.d;
        maxReUlp = 0;
        for(jj = 0; jj <= testNumY; jj++) {
            j.d = yStart.d + stepY * jj;
            y = j.d;
            // constraint: ((-2 * ((x1 * x1) * (x1 * x1))) + 2) >= x2;
            // if (x + y > 2) {
            //     continue;
            // }
            computeResult2param(x, y, mpfrResult);
            computeOrcle2param(x, y, mpfrOrcle);
            // orcle.d = mpfr_get_d(mpfrOrcle, MPFR_RNDN);
            // result.d = mpfr_get_d(mpfrResult, MPFR_RNDN);
            #ifdef SINGLE
            reUlp = computeUlpDiffF(mpfrOrcle, mpfrResult);
            #else   // compute Double ULP as default
            reUlp = computeUlpDiff(mpfrOrcle, mpfrResult);
            #endif

            if (reUlp > maxReUlp) {
                // maxInputX = j;
                maxReUlp = reUlp;
            }
        }
        fprintf(f, "%.16le\t%.16e\n", x, maxReUlp);
    }
    fclose(f);
}

void sample2paramY(DL xStart, DL xEnd, DL yStart, DL yEnd, unsigned long int testNumX, unsigned long int testNumY, const char* uniqueLabel, const char *fileNameKernel, int myid, int iStartLocal, int iEndLocal)
{
    DL i, j;
    // DL maxInputX, maxInputY, orcle, result;
    int ii, jj;
    double x, y, reUlp, maxReUlp, lenX, lenY;

    // mpfr
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // write error data to file
    char *directory = "outputs";
    char *suffix = "sample_Y";
    char *fileNameSample;
    FILE *f;
    fileNameSample = (char *) calloc(strlen(directory) + strlen(uniqueLabel) + strlen(fileNameKernel) + strlen(suffix) + 128, sizeof(char));
    sprintf(fileNameSample, "%s/%s/%s_%s_%02d.txt", directory, uniqueLabel, fileNameKernel, suffix, myid);
    // printf("%s\n", fileNameSample);
    if ((f = fopen(fileNameSample, "w")) == NULL) {
        printf("Error opening file %s.\n", fileNameSample);
        exit(0);
    }

    // loop boundary
    lenX = xEnd.d - xStart.d;
    lenY = yEnd.d - yStart.d;
    double stepX = lenX / (double)testNumX;
    double stepY = lenY / (double)testNumY;

    // Real number average
    // 固定y值，求不同x值下的最大误差
    for(ii = iStartLocal; ii <= iEndLocal; ii++) {
        i.d = yStart.d + stepY * ii;
        y = i.d;
        maxReUlp = 0;
        for(jj = 0; jj <= testNumX; jj++) {
            j.d = xStart.d + stepX * jj;
            x = j.d;
            // constraint: ((-2 * ((x1 * x1) * (x1 * x1))) + 2) >= x2;
            // if (x + y > 2) {
            //     continue;
            // }
            computeResult2param(x, y, mpfrResult);
            computeOrcle2param(x, y, mpfrOrcle);
            // orcle.d = mpfr_get_d(mpfrOrcle, MPFR_RNDN);
            // result.d = mpfr_get_d(mpfrResult, MPFR_RNDN);
            #ifdef SINGLE
            reUlp = computeUlpDiffF(mpfrOrcle, mpfrResult);
            #else   // compute Double ULP as default
            reUlp = computeUlpDiff(mpfrOrcle, mpfrResult);
            #endif

            if (reUlp > maxReUlp) {
                // maxInputX = j;
                maxReUlp = reUlp;
            }
        }
        fprintf(f, "%.16le\t%.16e\n", y, maxReUlp);
    }
    fclose(f);
}

int main(int argc, char *argv[])
{
    // parallel
    int myid, numProcs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    // parameters init
    DL xStart, xEnd, yStart, yEnd;
    unsigned long int drawNum, findMaxNum;
    xStart.d = 0.1;
    xEnd.d = 100;
    yStart.d = 0.1;
    yEnd.d = 100;
    drawNum = TESTNUMX;
    findMaxNum = TESTNUMY;

    char *fileNameKernel;
    fileNameKernel = calloc(256, sizeof(char));
    char *uniqueLabel;
    uniqueLabel = calloc(256, sizeof(char));

    if(argc == 9) {
        xStart.d = strtod(argv[1], NULL);
        xEnd.d = strtod(argv[2], NULL);
        yStart.d = strtod(argv[3], NULL);
        yEnd.d = strtod(argv[4], NULL);
        drawNum = strtod(argv[5], NULL);
        findMaxNum = strtod(argv[6], NULL);
        strcpy(fileNameKernel, argv[7]);
        strcpy(uniqueLabel, argv[8]);
    } else {
        printf("Usage: ./sample2paramParallel.out xStart xEnd yStart yEnd drawNum findMaxNum fileNameKernel uniqueLabel\n");
        exit(EXIT_FAILURE);
    }

    // local parameters init
    int lenLocal = drawNum / numProcs;
    int iStartLocal;
    iStartLocal = myid * lenLocal;
    int iEndLocal;
    if (myid != numProcs - 1)
    {
        iEndLocal = (myid + 1) * lenLocal - 1;
    }
    else
    {
        iEndLocal = drawNum;
    }

    sample2paramX(xStart, xEnd, yStart, yEnd, drawNum, findMaxNum, uniqueLabel, fileNameKernel, myid, iStartLocal, iEndLocal);
    sample2paramY(xStart, xEnd, yStart, yEnd, findMaxNum, drawNum, uniqueLabel, fileNameKernel, myid, iStartLocal, iEndLocal);

    // clear
    free(uniqueLabel);
    free(fileNameKernel);
    MPI_Finalize();
    return 0;
}