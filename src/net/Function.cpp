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
    //
}

Matrix derivativeFunction(Matrix& fx, std::string& func_type) {
    Matrix dx;
    if (func_type == "sigmoid") {
        Matrix ddx = sigmoid(fx) * (-1) + 1;
        Matrix dxx = sigmoid(fx);
        if (dxx._row == 1) {
            dx.resize(dxx._row, dxx._col);
            for (int i=0;i<dxx._col;i++) {
                dx._arr[i] = dxx._arr[i] * ddx._arr[i];
            }
        } else {
            dx.resize(dxx._row, dxx._col);
            for (int i=0;i<dxx._row;i++) {
                dx._arr[i] = dxx._arr[i] * ddx._arr[i];
            }
        }
    }

    if (func_type == "tanh2") {
        Matrix tanh_2 = M_pow(tanh2(fx), 2.);
        dx = tanh_2 * (-1) + 1;
    }
    return dx;
}
void calcLoss(Matrix output, Matrix& target, Matrix& output_error, float* loss) {
    //
}