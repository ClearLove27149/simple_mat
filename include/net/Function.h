#pragma once
#ifndef SIMPLE_MAT_FUNCTION_H
#define SIMPLE_MAT_FUNCTION_H

#include "../matrix/utils.h"
#include <iostream>

Matrix sigmoid(Matrix& matrix);
Matrix tanh(Matrix& matrix);
Matrix tanh2(Matrix& matrix);

Matrix ReLU(Matrix& matrix);
Matrix derivativeFunction(Matrix& fx, std::string& func_type);
void calcLoss(Matrix output, Matrix& target, Matrix& output_error, float* loss);

#endif //SIMPLE_MAT_FUNCTION_H
