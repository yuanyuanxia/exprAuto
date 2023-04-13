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

double EXPRESSIONMPFR(double, double, double, double, double, mpfr_t);
double EXPRESSIONMINE(double, double, double, double, double);

int computeOrcle5param(double x0, double x1, double x2, double x3, double x4, mpfr_t orcle) {
    return EXPRESSIONMPFR(x0, x1, x2, x3, x4, orcle);
}

int computeResult5param(double x0, double x1, double x2, double x3, double x4, mpfr_t mpfrResult) {
    int status = 1;

    double result = EXPRESSIONMINE(x0, x1, x2, x3, x4);
    mpfr_set_d(mpfrResult, result, MPFR_RNDN);

    return status;
}

void sample5paramX(DL xStart, DL xEnd, DL yStart, DL yEnd, DL zStart, DL zEnd, DL uStart, DL uEnd, DL vStart, DL vEnd, unsigned long int testNumX, unsigned long int testNumY, unsigned long int testNumZ, unsigned long int testNumU, unsigned long int testNumV, const char* uniqueLabel, const char *fileNameKernel, int myid, int iStartLocal, int iEndLocal)
{
    DL i, j, k, l, m;
    // DL maxInputX, maxInputY, maxInputZ, orcle, result;
    int ii, jj, kk, ll, mm;
    double x, y, z, u, v, reUlp, maxReUlp, lenX, lenY, lenZ, lenU, lenV;

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
    lenZ = zEnd.d - zStart.d;
    lenU = uEnd.d - uStart.d;
    lenV = vEnd.d - vStart.d;
    double stepX = lenX / (double)testNumX;
    double stepY = lenY / (double)testNumY;
    double stepZ = lenZ / (double)testNumZ;
    double stepU = lenU / (double)testNumU;
    double stepV = lenV / (double)testNumV;
    
    // Real number average
    // 固定x值，求不同y值下的最大误差
    for(ii = iStartLocal; ii <= iEndLocal; ii++) {
        i.d = xStart.d + stepX * ii;
        x = i.d;
        maxReUlp = 0;
        for(jj = 0; jj <= testNumY; jj++) {
            j.d = yStart.d + stepY * jj;
            y = j.d;
            for(kk = 0; kk <= testNumZ; kk++) {
                k.d = zStart.d + stepZ * kk;
                z = k.d;
                for(ll = 0; ll <= testNumU; ll++) {
                    l.d = uStart.d + stepU * ll;
                    u = l.d;
                    for(mm = 0; mm <= testNumV; mm++) {
                        m.d = vStart.d + stepV * mm;
                        v = m.d;
                        computeResult5param(x, y, z, u, v, mpfrResult);
                        computeOrcle5param(x, y, z, u, v, mpfrOrcle);
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
                }
            }
        }
        fprintf(f, "%.16le\t%.16e\n", x, maxReUlp);
    }
    fclose(f);
}

void sample5paramY(DL xStart, DL xEnd, DL yStart, DL yEnd, DL zStart, DL zEnd, DL uStart, DL uEnd, DL vStart, DL vEnd, unsigned long int testNumX, unsigned long int testNumY, unsigned long int testNumZ, unsigned long int testNumU, unsigned long int testNumV, const char* uniqueLabel, const char *fileNameKernel, int myid, int iStartLocal, int iEndLocal)
{
    DL i, j, k, l, m;
    // DL maxInputX, maxInputY, maxInputZ, orcle, result;
    int ii, jj, kk, ll, mm;
    double x, y, z, u, v, reUlp, maxReUlp, lenX, lenY, lenZ, lenU, lenV;

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
    lenZ = zEnd.d - zStart.d;
    lenU = uEnd.d - uStart.d;
    lenV = vEnd.d - vStart.d;
    double stepX = lenX / (double)testNumX;
    double stepY = lenY / (double)testNumY;
    double stepZ = lenZ / (double)testNumZ;
    double stepU = lenU / (double)testNumU;
    double stepV = lenV / (double)testNumV;

    // Real number average
    // 固定y值，求不同x值下的最大误差
    for(ii = iStartLocal; ii <= iEndLocal; ii++) {
        i.d = yStart.d + stepY * ii;
        y = i.d;
        maxReUlp = 0;
        for(jj = 0; jj <= testNumX; jj++) {
            j.d = xStart.d + stepX * jj;
            x = j.d;
            for(kk = 0; kk <= testNumZ; kk++) {
                k.d = zStart.d + stepZ * kk;
                z = k.d;
                for(ll = 0; ll <= testNumU; ll++) {
                    l.d = uStart.d + stepU * ll;
                    u = l.d;
                    for(mm = 0; mm <= testNumV; mm++) {
                        m.d = vStart.d + stepV * mm;
                        v = m.d;
                        computeResult5param(x, y, z, u, v, mpfrResult);
                        computeOrcle5param(x, y, z, u, v, mpfrOrcle);
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
                }
            }
        }
        fprintf(f, "%.16le\t%.16e\n", y, maxReUlp);
    }
    fclose(f);
}

void sample5paramZ(DL xStart, DL xEnd, DL yStart, DL yEnd, DL zStart, DL zEnd, DL uStart, DL uEnd, DL vStart, DL vEnd, unsigned long int testNumX, unsigned long int testNumY, unsigned long int testNumZ, unsigned long int testNumU, unsigned long int testNumV, const char* uniqueLabel, const char *fileNameKernel, int myid, int iStartLocal, int iEndLocal)
{
    DL i, j, k, l, m;
    // DL maxInputX, maxInputY, maxInputZ, orcle, result;
    int ii, jj, kk, ll, mm;
    double x, y, z, u, v, reUlp, maxReUlp, lenX, lenY, lenZ, lenU, lenV;

    // mpfr
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // write error data to file
    char *directory = "outputs";
    char *suffix = "sample_Z";
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
    lenZ = zEnd.d - zStart.d;
    lenU = uEnd.d - uStart.d;
    lenV = vEnd.d - vStart.d;
    double stepX = lenX / (double)testNumX;
    double stepY = lenY / (double)testNumY;
    double stepZ = lenZ / (double)testNumZ;
    double stepU = lenU / (double)testNumU;
    double stepV = lenV / (double)testNumV;

    // Real number average
    // 固定z值，求不同x值下的最大误差
    for(ii = iStartLocal; ii <= iEndLocal; ii++) {
        i.d = zStart.d + stepZ * ii;
        z = i.d;
        maxReUlp = 0;
        for(jj = 0; jj <= testNumX; jj++) {
            j.d = xStart.d + stepX * jj;
            x = j.d;
            for(kk = 0; kk <= testNumY; kk++) {
                k.d = yStart.d + stepY * kk;
                y = k.d;
                for(ll = 0; ll <= testNumU; ll++) {
                    l.d = uStart.d + stepU * ll;
                    u = l.d;
                    for(mm = 0; mm <= testNumV; mm++) {
                        m.d = vStart.d + stepV * mm;
                        v = m.d;
                        computeResult5param(x, y, z, u, v, mpfrResult);
                        computeOrcle5param(x, y, z, u, v, mpfrOrcle);
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
                }
            }
        }
        fprintf(f, "%.16le\t%.16e\n", z, maxReUlp);
    }
    fclose(f);
}

void sample5paramU(DL xStart, DL xEnd, DL yStart, DL yEnd, DL zStart, DL zEnd, DL uStart, DL uEnd, DL vStart, DL vEnd, unsigned long int testNumX, unsigned long int testNumY, unsigned long int testNumZ, unsigned long int testNumU, unsigned long int testNumV, const char* uniqueLabel, const char *fileNameKernel, int myid, int iStartLocal, int iEndLocal)
{
    DL i, j, k, l, m;
    // DL maxInputX, maxInputY, maxInputZ, orcle, result;
    int ii, jj, kk, ll, mm;
    double x, y, z, u, v, reUlp, maxReUlp, lenX, lenY, lenZ, lenU, lenV;

    // mpfr
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // write error data to file
    char *directory = "outputs";
    char *suffix = "sample_U";
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
    lenZ = zEnd.d - zStart.d;
    lenU = uEnd.d - uStart.d;
    lenV = vEnd.d - vStart.d;
    double stepX = lenX / (double)testNumX;
    double stepY = lenY / (double)testNumY;
    double stepZ = lenZ / (double)testNumZ;
    double stepU = lenU / (double)testNumU;
    double stepV = lenV / (double)testNumV;

    // Real number average
    // 固定u值，求不同x值下的最大误差
    for(ii = iStartLocal; ii <= iEndLocal; ii++) {
        i.d = uStart.d + stepU * ii;
        u = i.d;
        maxReUlp = 0;
        for(jj = 0; jj <= testNumX; jj++) {
            j.d = xStart.d + stepX * jj;
            x = j.d;
            for(kk = 0; kk <= testNumY; kk++) {
                k.d = yStart.d + stepY * kk;
                y = k.d;
                for(ll = 0; ll <= testNumZ; ll++) {
                    l.d = uStart.d + stepZ * ll;
                    z = l.d;
                    for(mm = 0; mm <= testNumV; mm++) {
                        m.d = vStart.d + stepV * mm;
                        v = m.d;
                        computeResult5param(x, y, z, u, v, mpfrResult);
                        computeOrcle5param(x, y, z, u, v, mpfrOrcle);
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
                }
            }
        }
        fprintf(f, "%.16le\t%.16e\n", u, maxReUlp);
    }
    fclose(f);
}

void sample5paramV(DL xStart, DL xEnd, DL yStart, DL yEnd, DL zStart, DL zEnd, DL uStart, DL uEnd, DL vStart, DL vEnd, unsigned long int testNumX, unsigned long int testNumY, unsigned long int testNumZ, unsigned long int testNumU, unsigned long int testNumV, const char* uniqueLabel, const char *fileNameKernel, int myid, int iStartLocal, int iEndLocal)
{
    DL i, j, k, l, m;
    // DL maxInputX, maxInputY, maxInputZ, orcle, result;
    int ii, jj, kk, ll, mm;
    double x, y, z, u, v, reUlp, maxReUlp, lenX, lenY, lenZ, lenU, lenV;

    // mpfr
    mpfr_t mpfrOrcle, mpfrResult;
    mpfr_inits2(PRECISION, mpfrOrcle, mpfrResult, (mpfr_ptr) 0);

    // write error data to file
    char *directory = "outputs";
    char *suffix = "sample_V";
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
    lenZ = zEnd.d - zStart.d;
    lenU = uEnd.d - uStart.d;
    lenV = vEnd.d - vStart.d;
    double stepX = lenX / (double)testNumX;
    double stepY = lenY / (double)testNumY;
    double stepZ = lenZ / (double)testNumZ;
    double stepU = lenU / (double)testNumU;
    double stepV = lenV / (double)testNumV;

    // Real number average
    // 固定u值，求不同x值下的最大误差
    for(ii = iStartLocal; ii <= iEndLocal; ii++) {
        i.d = vStart.d + stepV * ii;
        v = i.d;
        maxReUlp = 0;
        for(jj = 0; jj <= testNumX; jj++) {
            j.d = xStart.d + stepX * jj;
            x = j.d;
            for(kk = 0; kk <= testNumY; kk++) {
                k.d = yStart.d + stepY * kk;
                y = k.d;
                for(ll = 0; ll <= testNumZ; ll++) {
                    l.d = uStart.d + stepZ * ll;
                    z = l.d;
                    for(mm = 0; mm <= testNumU; mm++) {
                        m.d = uStart.d + stepU * mm;
                        u = m.d;
                        computeResult5param(x, y, z, u, v, mpfrResult);
                        computeOrcle5param(x, y, z, u, v, mpfrOrcle);
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
                }
            }
        }
        fprintf(f, "%.16le\t%.16e\n", v, maxReUlp);
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
    DL xStart, xEnd, yStart, yEnd, zStart, zEnd, uStart, uEnd, vStart, vEnd;
    unsigned long int drawNum, findMaxNum;
    xStart.d = 0.1;
    xEnd.d = 100;
    yStart.d = 0.1;
    yEnd.d = 100;
    zStart.d = 0.1;
    zEnd.d = 100;
    uStart.d = 0.1;
    uEnd.d = 100;
    vStart.d = 0.1;
    vEnd.d = 100;
    drawNum = TESTNUMX;
    findMaxNum = TESTNUMY;

    char *fileNameKernel;
    fileNameKernel = calloc(256, sizeof(char));
    char *uniqueLabel;
    uniqueLabel = calloc(256, sizeof(char));

    if(argc == 15) {
        xStart.d = strtod(argv[1], NULL);
        xEnd.d = strtod(argv[2], NULL);
        yStart.d = strtod(argv[3], NULL);
        yEnd.d = strtod(argv[4], NULL);
        zStart.d = strtod(argv[5], NULL);
        zEnd.d = strtod(argv[6], NULL);
        uStart.d = strtod(argv[7], NULL);
        uEnd.d = strtod(argv[8], NULL);
        vStart.d = strtod(argv[9], NULL);
        vEnd.d = strtod(argv[10], NULL);
        drawNum = strtod(argv[11], NULL);
        findMaxNum = strtod(argv[12], NULL);
        strcpy(fileNameKernel, argv[13]);
        strcpy(uniqueLabel, argv[14]);
    } else {
        printf("Usage: ./sample4paramParallel.out xStart xEnd yStart yEnd zStart zEnd uStart uEnd vStart vEnd drawNum findMaxNum fileNameKernel uniqueLabel\n");
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

    sample5paramX(xStart, xEnd, yStart, yEnd, zStart, zEnd, uStart, uEnd, vStart, vEnd, drawNum, findMaxNum, findMaxNum, findMaxNum, findMaxNum, uniqueLabel, fileNameKernel, myid, iStartLocal, iEndLocal);
    sample5paramY(xStart, xEnd, yStart, yEnd, zStart, zEnd, uStart, uEnd, vStart, vEnd, findMaxNum, drawNum, findMaxNum, findMaxNum, findMaxNum, uniqueLabel, fileNameKernel, myid, iStartLocal, iEndLocal);
    sample5paramZ(xStart, xEnd, yStart, yEnd, zStart, zEnd, uStart, uEnd, vStart, vEnd, findMaxNum, findMaxNum, drawNum, findMaxNum, findMaxNum, uniqueLabel, fileNameKernel, myid, iStartLocal, iEndLocal);
    sample5paramU(xStart, xEnd, yStart, yEnd, zStart, zEnd, uStart, uEnd, vStart, vEnd, findMaxNum, findMaxNum, findMaxNum, drawNum, findMaxNum, uniqueLabel, fileNameKernel, myid, iStartLocal, iEndLocal);
    sample5paramV(xStart, xEnd, yStart, yEnd, zStart, zEnd, uStart, uEnd, vStart, vEnd, findMaxNum, findMaxNum, findMaxNum, findMaxNum, drawNum, uniqueLabel, fileNameKernel, myid, iStartLocal, iEndLocal);

    // clear
    free(uniqueLabel);
    free(fileNameKernel);
    MPI_Finalize();
    return 0;
}