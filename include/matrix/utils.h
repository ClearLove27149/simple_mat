#ifndef SIMPLEMAT_UTILS_H
#define SIMPLEMAT_UTILS_H

#include "matrix.h"
#include "point.h"
#include <list>
#include <cmath>
#include <random>

// 基本变换转矩阵
Matrix Etrans_2_Matrix(std::list<EtransStruct>& etransStructs, int order, int type);
// 基本变换转矩阵的转置
Matrix Etrans_4_Inverse(Matrix& matrix_result, std::list<EtransStruct>& etransStructs, int type);
//上三角化
UptriStruct M_Uptri_(Matrix& matrix);
// 下三角化
LowtriStruct M_Lowtri_(Matrix& matrix);
//对角化
DiaStruct M_Diatri_(Matrix& matrix);

// 求逆相关
// 上\下三角化-求逆使用
InverseStruct M_Uptri_4inv(Matrix& matrix);
InverseStruct M_Lowtri_4inv(Matrix& matrix);
// 对角矩阵求逆
Matrix M_Diatri_Inv(Matrix& matrix);
// 矩阵求逆
Matrix M_Inverse(Matrix& matrix);
Matrix M_Inverse(Matrix&& matrix);
// 矩阵的秩
int M_rank(Matrix& matrix);
// 矩阵的迹
float M_tr(Matrix& matrix);
// 行列式

// math
Matrix M_exp(Matrix& matrix);
Matrix M_exp(Matrix&& matrix);

Matrix M_pow(Matrix& matrix, float y);
Matrix M_pow(Matrix&& matrix, float y);

float M_sum(Matrix& matrix);
float M_sum(Matrix&& matrix);

//random
// gauss
void randn(Matrix& mat, float a, float b);
// 均匀分布
void randu(Matrix& mat, float a, float b);

//min max
#define FLOAT_MAX 3.402823466e+38
#define FLOAT_MIN 1.175494351e-38

void minLoc(const Matrix& src, float* minVal, Point<int>* minLoc=0);
void maxLoc(const Matrix& src, float* maxVal, Point<int>* maxLoc=0);

// 分列，分行
Matrix row(Matrix& mat, int index);
Matrix col(Matrix& mat, int index);

#endif //SIMPLEMAT_UTILS_H
