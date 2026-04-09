#include "../../include/matrix/utils.h"

using namespace std;

// 基本变换转矩阵
Matrix Etrans_2_Matrix(std::list<EtransStruct>& etransStructs, int order, int type) {
    Matrix matrix_result = Matrix::I_M(order);
    if (!etransStructs.empty()) {
        for (auto it = etransStructs.rbegin();it!=etransStructs.rend();it++) {
            it->scale *= -1;
            matrix_result.M_E_trans(*it, type);
        }
    }
    return matrix_result;
}
// 基本变换转矩阵的转置
Matrix Etrans_4_Inverse(Matrix& matrix_result, std::list<EtransStruct>& etransStructs, int type) {
    if (!etransStructs.empty()) {
        for (auto it = etransStructs.rbegin();it!=etransStructs.rend();it++) {
            std::swap(it->minuendLine, it->subtractorLine);
            matrix_result.M_E_trans(*it, type);
        }
    }
    return matrix_result;
}
//上三角化
UptriStruct M_Uptri_(Matrix& matrix) {
    Matrix _mat = matrix;
    std::list<EtransStruct> etransStructs;
    int flag = 0;
    // 初等变换
    for (int i=0;i<_mat._col;i++) {
        for (int j=i+1;j<_mat._row;j++) {
            flag = 0;
            EtransStruct etrans_tmp(j+1, i+1, 0);
            if (_mat._arr[i*_mat._col+i] != 0) {
                etrans_tmp.scale = (_mat._arr[(_mat._col)*j+i]) / (_mat._arr[i*(_mat._col)+i]);
            } else {
                etrans_tmp.scale = 0;
                for (int k=i+1;k<_mat._row;k++) {
                    flag = 1; // 无可替代
                    if (_mat._arr[(_mat._col)*k+i] != 0) {
                        etrans_tmp.minuendLine = -(i+1);
                        etrans_tmp.subtractorLine = -(k+1);
                        flag = 2; // 能够作为替换行
                        break;
                    }
                }
                if (flag == 1) {
                    break;
                }
            }
            etransStructs.push_back(etrans_tmp);
            cout << "M_Uptri_etrans : " << etrans_tmp.scale << ","<<etrans_tmp.minuendLine<<","<<etrans_tmp.subtractorLine<<endl;
            _mat.M_E_trans(etrans_tmp, _ETRANS_ROW__);
            if (flag == 2) {
                i=i-1;
                break;
            }
        }
    }
    Matrix trans_mat = Etrans_2_Matrix(etransStructs, _mat._row, _ETRANS_ROW__);
    UptriStruct _Uptri(trans_mat, _mat);
    return _Uptri;
}
// 下三角化
LowtriStruct M_Lowtri_(Matrix& matrix) {
    Matrix _mat = matrix;
    std::list<EtransStruct> etransStructs;
    int flag = 0;
    // 初等变换
    for (int i=0;i<_mat._row;i++) {
        for (int j=i+1;j<_mat._col;j++) {
            flag = 0;
            EtransStruct etrans_tmp(j+1, i+1, 0);
            if (_mat._arr[i*_mat._col+i] != 0) {
                etrans_tmp.scale = (_mat._arr[(_mat._col)*i+j]) / (_mat._arr[i*(_mat._col)+i]);
            } else {
                etrans_tmp.scale = 0;
                for (int k=i+1;k<_mat._row;k++) {
                    flag = 1; // 无可替代
                    if (_mat._arr[(_mat._col)*k+i] != 0) {
                        etrans_tmp.minuendLine = -(i+1);
                        etrans_tmp.subtractorLine = -(k+1);
                        flag = 2; // 能够作为替换行
                        break;
                    }
                }
                if (flag == 1) {
                    break;
                }
            }
            etransStructs.push_back(etrans_tmp);
            cout << "M_Lowtri_etrans : " << etrans_tmp.scale << ","<<etrans_tmp.minuendLine<<","<<etrans_tmp.subtractorLine<<endl;
            _mat.M_E_trans(etrans_tmp, _ETRANS_COL__);
            if (flag == 2) {
                i=i-1;
                break;
            }
        }
    }
    Matrix trans_mat = Etrans_2_Matrix(etransStructs, _mat._row, _ETRANS_COL__);
    LowtriStruct _Lowtri(trans_mat, _mat);
    return _Lowtri;
}
//对角化
DiaStruct M_Diatri_(Matrix& matrix) {
    UptriStruct _Uptri = M_Uptri_(matrix);
    LowtriStruct _Lowtri = M_Lowtri_(_Uptri.Uptri_matrix);
    return DiaStruct(_Uptri.trans_matrix, _Lowtri.trans_matrix, _Lowtri.Lowtri_matrix);
}

// 求逆相关
// 上\下三角化-求逆使用
InverseStruct M_Uptri_4inv(Matrix& matrix) {
    Matrix _mat = matrix;
    std::list<EtransStruct> etransStructs;
    int flag = 0;
    // 初等变换
    for (int i=0;i<_mat._col;i++) {
        for (int j=i+1;j<_mat._row;j++) {
            flag = 0;
            EtransStruct etrans_tmp(j+1, i+1, 0);
            if (_mat._arr[i*_mat._col+i] != 0) {
                etrans_tmp.scale = (_mat._arr[(_mat._col)*j+i]) / (_mat._arr[i*(_mat._col)+i]);
            } else {
                etrans_tmp.scale = 0;
                for (int k=i+1;k<_mat._row;k++) {
                    flag = 1; // 无可替代
                    if (_mat._arr[(_mat._col)*k+i] != 0) {
                        etrans_tmp.minuendLine = -(i+1);
                        etrans_tmp.subtractorLine = -(k+1);
                        flag = 2; // 能够作为替换行
                        break;
                    }
                }
                if (flag == 1) {
                    break;
                }
            }
            etransStructs.push_back(etrans_tmp);
            cout << "M_Uptri_4inv etrans : " << etrans_tmp.scale << ","<<etrans_tmp.minuendLine<<","<<etrans_tmp.subtractorLine<<endl;
            _mat.M_E_trans(etrans_tmp, _ETRANS_ROW__);
            if (flag == 2) {
                i=i-1;
                break;
            }
        }
    }
    InverseStruct inverseStruct(_mat, etransStructs);
    return inverseStruct;
}
InverseStruct M_Lowtri_4inv(Matrix& matrix) {
    Matrix _mat = matrix;
    std::list<EtransStruct> etransStructs;
    int flag = 0;
    // 初等变换
    for (int i=0;i<_mat._row;i++) {
        for (int j=i+1;j<_mat._col;j++) {
            flag = 0;
            EtransStruct etrans_tmp(j+1, i+1, 0);
            if (_mat._arr[i*_mat._col+i] != 0) {
                etrans_tmp.scale = (_mat._arr[(_mat._col)*i+j]) / (_mat._arr[i*(_mat._col)+i]);
            } else {
                etrans_tmp.scale = 0;
                for (int k=i+1;k<_mat._row;k++) {
                    flag = 1; // 无可替代
                    if (_mat._arr[(_mat._col)*k+i] != 0) {
                        etrans_tmp.minuendLine = -(i+1);
                        etrans_tmp.subtractorLine = -(k+1);
                        flag = 2; // 能够作为替换行
                        break;
                    }
                }
                if (flag == 1) {
                    break;
                }
            }
            etransStructs.push_back(etrans_tmp);
            cout << "M_Lowtri_4inv etrans : " << etrans_tmp.scale << ","<<etrans_tmp.minuendLine<<","<<etrans_tmp.subtractorLine<<endl;
            _mat.M_E_trans(etrans_tmp, _ETRANS_COL__);
            if (flag == 2) {
                i=i-1;
                break;
            }
        }
    }
    InverseStruct inverseStruct(_mat, etransStructs);
    return inverseStruct;
}
// 对角矩阵求逆
Matrix M_Diatri_Inv(Matrix& matrix) {
    Matrix _mat_inv = matrix;
    if (matrix._col != matrix._row) {
        printf_s("M_Diatri_Inv : _row, _col is not equal, %d, %d\n", matrix._row, matrix._col);
        throw std::exception();
    } else {
        int order = matrix._col;
        for (int i=0;i<order;i++) {
            if (matrix._arr[i*(order+1)] == 0) {
                // 不可逆
                printf_s("M_Diatri_Inv : 矩阵不可逆\n");
                throw std::exception();
                _mat_inv._arr[i*(order+1)] = 1.0/(_mat_inv._arr[i*(order+1)]);
            } else {
                _mat_inv._arr[i*(order+1)] = 1.0/(_mat_inv._arr[i*(order+1)]);
            }

        }
    }
    return _mat_inv;
}
// 矩阵求逆
Matrix M_Inverse(Matrix& matrix) {
    InverseStruct _Uptri = M_Uptri_4inv(matrix);
    printf_s("*******************_Uptri.inverse_matrix:\n");
    cout << _Uptri.inverse_matrix << endl;
    InverseStruct _Lowtri = M_Lowtri_4inv(_Uptri.inverse_matrix);
    printf_s("*******************_Lowtri.inverse_matrix:\n");
    cout << _Lowtri.inverse_matrix << endl;
    Matrix _mat_dia_inv = M_Diatri_Inv(_Lowtri.inverse_matrix);
    printf_s("*******************_mat_dia_inv:\n");
    cout << _mat_dia_inv << endl;
    Matrix _mat_inv = Etrans_4_Inverse(_mat_dia_inv, _Lowtri.inverse_list, _ETRANS_ROW__);
    printf_s("*******************_mat_inv:\n");
    cout << _mat_inv << endl;
    _mat_inv = Etrans_4_Inverse(_mat_inv, _Uptri.inverse_list, _ETRANS_COL__);
    return _mat_inv;
}
Matrix M_Inverse(Matrix&& matrix) {
    InverseStruct _Uptri = M_Uptri_4inv(matrix);
    printf_s("*******************_Uptri.inverse_matrix:\n");
    cout << _Uptri.inverse_matrix << endl;
    InverseStruct _Lowtri = M_Lowtri_4inv(_Uptri.inverse_matrix);
    printf_s("*******************_Lowtri.inverse_matrix:\n");
    cout << _Lowtri.inverse_matrix << endl;
    Matrix _mat_dia_inv = M_Diatri_Inv(_Lowtri.inverse_matrix);
    printf_s("*******************_mat_dia_inv:\n");
    cout << _mat_dia_inv << endl;
    Matrix _mat_inv = Etrans_4_Inverse(_mat_dia_inv, _Lowtri.inverse_list, _ETRANS_ROW__);
    printf_s("*******************_mat_inv:\n");
    cout << _mat_inv << endl;
    _mat_inv = Etrans_4_Inverse(_mat_inv, _Uptri.inverse_list, _ETRANS_COL__);
    return _mat_inv;
}
// 矩阵的秩
int M_rank(Matrix& matrix) {
    InverseStruct _Uptri = M_Uptri_4inv(matrix);
    Matrix tmp = _Uptri.inverse_matrix;
    int rank = 0;
    for (int i=0;i<tmp._row;i++) {
        for (int j=0;j<tmp._col;j++) {
            if (tmp._arr[i*(tmp._col)+j] != 0) {
                rank++;
                break;
            }
        }
    }
    return rank;
}
// 矩阵的迹
float M_tr(Matrix& matrix) {
    float _tr_mat = 0;
    if (matrix._col != matrix._row) {
        printf_s("Error : M_tr() col is not equal row\n");
        throw std::exception();
    }
    for (int i=0;i<matrix._row;i++) {
        _tr_mat += matrix._arr[i*(matrix._col+1)];
    }
    return _tr_mat;
}
// 行列式

// math
Matrix M_exp(Matrix& matrix) {
    int row = matrix._row;
    int col = matrix._col;
    Matrix mat(row, col);
    for (int i=0;i<row;i++) {
        for (int j=0;j<col;j++) {
            mat._arr[i*col+j] = exp(matrix._arr[i*col+j]);
        }
    }
    return mat;
}
Matrix M_exp(Matrix&& matrix) {
    int row = matrix._row;
    int col = matrix._col;
    Matrix mat(row, col);
    for (int i=0;i<row;i++) {
        for (int j=0;j<col;j++) {
            mat._arr[i*col+j] = exp(matrix._arr[i*col+j]);
        }
    }
    return mat;
}

Matrix M_pow(Matrix& matrix, float y) {
    int row = matrix._row;
    int col = matrix._col;
    Matrix mat(row, col);
    for (int i=0;i<row;i++) {
        for (int j=0;j<col;j++) {
            mat._arr[i*col+j] = powf(matrix._arr[i*col+j], y);
        }
    }
    return mat;
}
Matrix M_pow(Matrix&& matrix, float y) {
    int row = matrix._row;
    int col = matrix._col;
    Matrix mat(row, col);
    for (int i=0;i<row;i++) {
        for (int j=0;j<col;j++) {
            mat._arr[i*col+j] = powf(matrix._arr[i*col+j], y);
        }
    }
    return mat;
}

float M_sum(Matrix& matrix) {
    int row = matrix._row;
    int col = matrix._col;
    float res = 0;
    for (int i=0;i<row;i++) {
        for (int j=0;j<col;j++) {
           res += matrix._arr[i*col+j];
        }
    }
    return res;
}
float M_sum(Matrix&& matrix) {
    int row = matrix._row;
    int col = matrix._col;
    float res = 0;
    for (int i=0;i<row;i++) {
        for (int j=0;j<col;j++) {
            res += matrix._arr[i*col+j];
        }
    }
    return res;
}

//random
void randn(Matrix& mat, float a, float b) {
    random_device seed; // 硬件生成随机数种子
    ranlux48 engine(seed());
    normal_distribution<float> distrib(a, b);
    for (int i=0;i<mat._row;i++) {
        for (int j=0;j<mat._col;j++) {
            mat._arr[i*mat._col+j] = distrib(engine);
        }
    }
}
void randu(Matrix& mat, float a, float b) {
    random_device seed; // 硬件生成随机数种子
    ranlux48 engine(seed());
    uniform_real_distribution<float> distrib(a, b);
    for (int i=0;i<mat._row;i++) {
        for (int j=0;j<mat._col;j++) {
            mat._arr[i*mat._col+j] = distrib(engine);
        }
    }
}

//min max
#define FLOAT_MAX 3.402823466e+38
#define FLOAT_MIN 1.175494351e-38

void minLoc(const Matrix& src, float* minVal, Point<int>* minLoc) {
    float tmp = FLOAT_MAX;
    int ii=0,jj=0;
    for (int i=0;i<src._row;i++) {
        for (int j=0;j<src._col;j++) {
            if (src._arr[i*src._col+j] < tmp) {
                tmp = src._arr[i*src._col+j];
                ii=i;
                jj=j;
            }
        }
    }
    if (minVal) *minVal = tmp;
    minLoc->x = ii;
    minLoc->y = jj;
}
void maxLoc(const Matrix& src, float* maxVal, Point<int>* maxLoc) {
    float tmp = FLOAT_MIN;
    int ii=0,jj=0;
    for (int i=0;i<src._row;i++) {
        for (int j=0;j<src._col;j++) {
            if (src._arr[i*src._col+j] > tmp) {
                tmp = src._arr[i*src._col+j];
                ii=i;
                jj=j;
            }
        }
    }
    if (maxVal) *maxVal = tmp;
    maxLoc->x = ii;
    maxLoc->y = jj;
}

// 分列，分行
Matrix row(Matrix& mat, int index) {
    Matrix tmp(1, mat._col);
    for (int i=0;i<mat._col;i++) {
        tmp._arr[i] = mat._arr[index*mat._col+i];
    }
    return tmp;
}
Matrix col(Matrix& mat, int index) {
    Matrix tmp(mat._row, 1);
    for (int i=0;i<mat._row;i++) {
        tmp._arr[i] = mat._arr[i*mat._col+index];
    }
    return tmp;
}