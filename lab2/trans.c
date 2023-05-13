/* 
 * trans.c - 矩阵转置B=A^T
 *每个转置函数都必须具有以下形式的原型：
 *void trans（int M，int N，int a[N][M]，int B[M][N]）；
 *通过计算，块大小为32字节的1KB直接映射缓存上的未命中数来计算转置函数。
 */ 
#include <stdio.h>
#include "cachelab.h"
int is_transpose(int M, int N, int A[N][M], int B[M][N]);
char transpose_submit_desc[] = "Transpose submission";  //请不要修改“Transpose_submission”


void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{

//                          请在此处添加代码
//*************************************Begin********************************************************
    int i, j, k;
    int t0, t1, t2, t3, t4, t5, t6, t7;
    if (M == 32 && N == 32) {
        for (i = 0; i < 32; i += 8) {
            for (j = 0; j < 32; j += 8) {
                for (k = i; k < i + 8; k++) {
                    t0 = A[k][j];
                    t1 = A[k][j + 1];
                    t2 = A[k][j + 2];
                    t3 = A[k][j + 3];
                    t4 = A[k][j + 4];
                    t5 = A[k][j + 5];
                    t6 = A[k][j + 6];
                    t7 = A[k][j + 7];
                    B[j][k] = t0;
                    B[j + 1][k] = t1;
                    B[j + 2][k] = t2;
                    B[j + 3][k] = t3;
                    B[j + 4][k] = t4;
                    B[j + 5][k] = t5;
                    B[j + 6][k] = t6;
                    B[j + 7][k] = t7;
                }
            }
        }
    } else if (M == 64 && N == 64) {
        for (i = 0; i < 64; i += 8) {
            for (j = 0; j < 64; j += 8) {
                for (k = i; k < i + 4; k++) {
                    // A(1, 1) -- T --> B(1, 1)
                    // A(1, 2) -- T --> B(1, 2)
                    t0 = A[k][j];
                    t1 = A[k][j + 1];
                    t2 = A[k][j + 2];
                    t3 = A[k][j + 3];
                    t4 = A[k][j + 4];
                    t5 = A[k][j + 5];
                    t6 = A[k][j + 6];
                    t7 = A[k][j + 7];
                    B[j][k] = t0;
                    B[j + 1][k] = t1;
                    B[j + 2][k] = t2;
                    B[j + 3][k] = t3;
                    B[j][k + 4] = t4;
                    B[j + 1][k + 4] = t5;
                    B[j + 2][k + 4] = t6;
                    B[j + 3][k + 4] = t7;
                }
                for (k = j; k < j + 4; k++) {
                    // B(1, 2) -------> B(2, 1)
                    // A(2, 1) -- T --> B(1, 2)                    
                    t0 = A[i + 4][k];
                    t1 = A[i + 5][k];
                    t2 = A[i + 6][k];
                    t3 = A[i + 7][k];
                    t4 = B[k][i + 4];
                    t5 = B[k][i + 5];
                    t6 = B[k][i + 6];
                    t7 = B[k][i + 7];
                    B[k][i + 4] = t0;
                    B[k][i + 5] = t1;
                    B[k][i + 6] = t2;
                    B[k][i + 7] = t3;
                    B[k + 4][i] = t4;
                    B[k + 4][i + 1] = t5;
                    B[k + 4][i + 2] = t6;
                    B[k + 4][i + 3] = t7;
                }
                for (k = i + 4; k < i + 8; k++) {
                    // A(2, 2) -- T --> B(2, 2)
                    t0 = A[k][j + 4];
                    t1 = A[k][j + 5];
                    t2 = A[k][j + 6];
                    t3 = A[k][j + 7];
                    B[j + 4][k] = t0;
                    B[j + 5][k] = t1;
                    B[j + 6][k] = t2;
                    B[j + 7][k] = t3;
                }
            }
        }
    } else if (M == 61 && N == 67) {
        for (i = 0; i < N; i += 16) {
            for (j = 0; j < M; j += 16) {
                for (k = i; k < i + 16 && k < N; k++) {
                    for (t0 = j; t0 < j + 16 && t0 < M; t0++)
                        B[t0][k] = A[k][t0];
                }
            }
        }
    }



//**************************************End**********************************************************
}

/* 
 * 我们在下面定义了一个简单的方法来帮助您开始，您可以根据下面的例子把上面值置补充完整。
 */ 

/* 
 * 简单的基线转置功能，未针对缓存进行优化。
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions-此函数向驱动程序注册转置函数。
 *在运行时，驱动程序将评估每个注册的函数并总结它们的性能。这是一种试验不同转置策略的简便方法。
 */
void registerFunctions()
{
    /* 注册解决方案函数  */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* 注册任何附加转置函数 */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - 函数检查B是否是A的转置。在从转置函数返回之前，可以通过调用它来检查转置的正确性。
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

