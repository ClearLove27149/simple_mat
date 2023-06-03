#ifndef SIMPLEMAT_MATRIX_H
#define SIMPLEMAT_MATRIX_H

#include <xmmintrin.h>
#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include <cstring>
#include <list>
#include <iostream>

#define _ETRANS_ROW__ 0
#define _ETRANS_COL__ 1

struct EtransStruct {
    int minuendLine; // 被减数行
    int subtractorLine; //减数行
    float scale; //系数
    EtransStruct() = delete;
    EtransStruct(int _minuendLine, int _subtractorLine, int _scale) :
    minuendLine(_minuendLine), subtractorLine(_subtractorLine), scale(_scale) {}
};

class Matrix {
public:
    int _row, _col;
    alignas(16) float* _arr = nullptr;

public:
    Matrix() = default;
    Matrix(int row, int col) : _row(row), _col(col) {
        if (_arr == nullptr) {
            _arr = new float [_row * _col];
        }
        memset(_arr, 0, _row*_col*sizeof(float));
    }
    Matrix(float* input, int row, int col) : _row(row), _col(col) {
        if (_arr == nullptr) {
            _arr = new float [_row * _col];
        }
        for (int i=0;i<_row*_col;++i) _arr[i] = input[i];
    }
    ~Matrix() {
        delete[] _arr;
        _arr = nullptr;
    }
    Matrix(const Matrix& other) {
        _row = other._row;
        _col = other._col;
        if (_arr == nullptr) {
            _arr = new float [_row*_col];
        }
        memcpy_s(_arr, _row*_col* sizeof(float), other._arr, _row*_col*sizeof(float));
    }
    //
    Matrix& operator= (const Matrix& other) {
        if (this == &other) return *this;
        this->_row = other._row;
        this->_col = other._col;
        if (this->_arr == nullptr) {
            this->_arr = new float [_row*_col];
        }
        memcpy_s(_arr, _row*_col* sizeof(float), other._arr, _row*_col*sizeof(float));
        return *this;
    }
    Matrix& operator= (Matrix&& other) {
        if (this == &other) return *this;
        this->_row = other._row;
        this->_col = other._col;
        // ?
        if (this->_arr == nullptr) {
            this->_arr = new float [_row*_col];
        }
        memcpy_s(_arr, _row*_col* sizeof(float), other._arr, _row*_col*sizeof(float));
        return *this;
    }
    bool empty() {return _row == 0 || _col == 0;};
    void resize(int row, int col) {
        _row = row;
        _col = col;
        if (_arr == nullptr) _arr = new float [_row*_col];
        memset(_arr, 0 ,_row*_col*sizeof(float));
    }
    // 转置
    Matrix transpose() {
        alignas(16) float* _arr_tmp = new float [_row*_col];
        for (int i=0;i<_row;i++) {
            for (int j=0;j<_col;j++) {
                _arr_tmp[j*_row+i] = _arr[i*_col+j];
            }
        }
        return Matrix(_arr_tmp, _col, _row);
    }
    void self_transpose() {
        alignas(16) float* _arr_tmp = new float [_row*_col];
        for (int i=0;i<_row;i++) {
            for (int j=0;j<_col;j++) {
                _arr_tmp[j*_row+i] = _arr[i*_col+j];
            }
        }
        std::swap(_arr, _arr_tmp);
        delete[] _arr_tmp;
    }
    void add(const Matrix& other);
    void minus(const Matrix& other);
    void mul(Matrix& other);
    void mul2(Matrix& other);

    Matrix add1(const Matrix& other);
    Matrix minus1(const Matrix& other);
    Matrix mul1(Matrix& other);
    Matrix mul11(Matrix& other);

    // operator []
    float operator[] (int i) {
        if (i<0 || i>=_row) {
            std::cout << "Error : index error" << std::endl;
            throw new std::exception();
        }
        return _arr[i];
    }
    float operator() (int i, int j) {
        if (i<0 || i>=_row || j<0 || j>=_col) {
            std::cout << "Error : index error" << std::endl;
            throw new std::exception();
        }
        return _arr[i*_col +j];
    }
    // self operator
    Matrix& operator+= (const Matrix& other) {
        add(other);
        return *this;
    }
    Matrix& operator-= (const Matrix& other) {
        minus(other);
        return *this;
    }
    Matrix& operator*= (Matrix& other) {
        mul(other);
        return *this;
    }
    //operator
    Matrix operator+ (const Matrix& other) {
        return add1(other);
    }
    Matrix operator+ (const Matrix&& other) {
        return add1(other);
    }
    Matrix operator- (const Matrix& other) {
        return minus1(other);
    }
    Matrix operator- (const Matrix&& other) {
        return minus1(other);
    }
    Matrix operator* (Matrix& other) {
        return mul1(other);
    }
    Matrix operator* (Matrix&& other) {
        return mul1(other);
    }
    // print
    friend std::ostream& operator<< (std::ostream& os, const Matrix& self);

    // utils, I matrix
    static Matrix I_M(int n) {
        Matrix tmp(n, n);
        for (int i=0;i<n;i++) tmp._arr[i*n+i] = 1;
        return tmp;
    }
    // 2. 初等变换
    void M_E_trans(EtransStruct& etransStruct, int type);
    void M_E_swap(int line1, int line2, int type);
    // 3. 切片
    Matrix M_splice(int row_s, int row_e, int col_s, int col_e);
    // 4. dot mul
    void M_dot(float num);
    void operator*= (float num) {
        M_dot(num);
    }
    Matrix operator* (const float num) {
        Matrix tmp(_row, _col);
        for (int i=0;i<_row;i++) {
            for (int j=0;j<_col;j++) {
                tmp._arr[i*_col+j] = _arr[i*_col+j] * num;
            }
        }
        return tmp;
    }
    Matrix operator+ (const float num) {
        Matrix tmp(_row, _col);
        for (int i=0;i<_row;i++) {
            for (int j=0;j<_col;j++) {
                tmp._arr[i*_col+j] = _arr[i*_col+j] + num;
            }
        }
        return tmp;
    }
};

///////////////////////////////////////////////////////////////
// uptri,lowtri，上下三角化
struct UptriStruct {
    Matrix trans_matrix;
    Matrix Uptri_matrix;
    UptriStruct(Matrix m1, Matrix m2) : trans_matrix(m1), Uptri_matrix(m2) {}
};
struct LowtriStruct {
    Matrix trans_matrix;
    Matrix Lowtri_matrix;
    LowtriStruct(Matrix m1, Matrix m2) : trans_matrix(m1), Lowtri_matrix(m2) {}
};
// 对角化结果
struct DiaStruct {
    Matrix trans_leftmatrix;
    Matrix trans_rightmatrix;
    Matrix Diatri_matrix;
    DiaStruct(Matrix m1, Matrix m2, Matrix m3) :
    trans_leftmatrix(m1), trans_rightmatrix(m2), Diatri_matrix(m3) {}
};
// 求逆运算结果
struct InverseStruct {
    Matrix inverse_matrix;
    std::list<EtransStruct> inverse_list;
    InverseStruct(Matrix m1, std::list<EtransStruct> list1) : inverse_matrix(m1),inverse_list(list1) {}
};
#endif //SIMPLEMAT_MATRIX_H
