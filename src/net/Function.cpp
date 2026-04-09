#include "../../include/net/Function.h"

Matrix sigmoid(Matrix& matrix) {
    int row = matrix._row;
    int col = matrix._col;
    Matrix res(row, col);
    for (int i=0;i<row;i++) {
        for (int j=0;j<col;j++) {
            res._arr[i*col+j] = 1.0/(1.0+exp(matrix._arr[i*col+j]));
        }
    }
    return res;
}
Matrix tanh(Matrix& matrix) {
    int row = matrix._row;
    int col = matrix._col;
    auto exp1 = M_exp(matrix);
    auto exp2 = M_exp(matrix*((float)(-1.0)));
    auto mat_inverse = M_Inverse(exp1 + exp2);
    auto exp3 = exp1 - exp2;
    auto res = exp3 * mat_inverse;
    return res;
}
Matrix tanh2(Matrix& matrix) {
    int row = matrix._row;
    int col = matrix._col;
    Matrix res(row, col);
    for (int i=0;i<row;i++) {
        for (int j=0;j<col;j++) {
            float tmp = matrix._arr[i*col+j];
            res._arr[i*col+j] = (exp(tmp)-exp(-tmp))*1.0/(exp(tmp)+exp(-tmp))*1.0;
        }
    }
    return res;
}


Matrix ReLU(Matrix& matrix) {
    Matrix res(matrix._row, matrix._col);
    for (int i = 0; i < matrix._row; i++) {
        for (int j = 0; j < matrix._col; j++) {
            int index = i * matrix._col + j;
            res._arr[index] = matrix._arr[index] > 0 ? matrix._arr[index] : 0;
        }
    }
    return res;
}

Matrix derivativeFunction(Matrix& fx, std::string& func_type) {
    Matrix dx(fx._row, fx._col);
    if (func_type == "sigmoid") {
        Matrix ddx = sigmoid(fx) * (-1) + 1;
        Matrix dxx = sigmoid(fx);
        // 遍历所有元素
        for (int i=0;i<fx._row;i++) {
            for (int j=0;j<fx._col;j++) {
                int index = i * fx._col + j;
                dx._arr[index] = dxx._arr[index] * ddx._arr[index];
            }
        }
    } else if (func_type == "tanh2") {
        Matrix tanh_2 = M_pow(tanh2(fx), 2.);
        dx = tanh_2 * (-1) + 1;
    }
    return dx;
}
void calcLoss(Matrix output, Matrix& target, Matrix& output_error, float* loss) {
    //
}