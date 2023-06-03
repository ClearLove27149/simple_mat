#include "../../include/matrix/matrix.h"
#include <cstring>

std::ostream& operator<< (std::ostream& os, const Matrix& self) {
    for (int i=0;i<self._row;i++) {
        for (int j=0;j<self._col;j++) {
            os << self._arr[i*self._col+j] << " ";
        }
        os << std::endl;
    }
    return os;
}
//
void Matrix::add(const Matrix &other) {
    if (_row != other._row || _col != other._col) {
        printf_s("Error : Matrix::add() row, col is not equal\n");
        return;
    }
#pragma omp parallel for
    for (int i=0;i<_row*_col;i+=4) {
        _mm_store_ps(_arr+i, _mm_add_ps(_mm_load_ps(_arr+i), _mm_load_ps(other._arr+i)));
    }
}
Matrix Matrix::add1(const Matrix &other) {
    if (_row != other._row || _col != other._col) {
        printf_s("Error : Matrix::add1() row, col is not equal\n");
        throw std::exception();
    }
    Matrix tmp(_row, _col);
    int yu = (_row*_col)%4;

#pragma omp parallel for
    for (int i=0;i<_row*_col-yu;i+=4) {
        _mm_store_ps(tmp._arr+i, _mm_add_ps(_mm_load_ps(_arr+i), _mm_load_ps(other._arr+i)));
    }
//
    for (int i=0;i<yu;i++) {
        tmp._arr[i+_row*_col-yu] = _arr[i+_row*_col-yu] + other._arr[i+_row*_col-yu];
    }
    return tmp;
}
void Matrix::minus(const Matrix &other) {
    if (_row != other._row || _col != other._col) {
        printf_s("Error : Matrix::minus() row, col is not equal\n");
        return;
    }
#pragma omp parallel for
    for (int i=0;i<_row*_col;i+=4) {
        _mm_store_ps(_arr+i, _mm_sub_ps(_mm_load_ps(_arr+i), _mm_load_ps(other._arr+i)));
    }
}
Matrix Matrix::minus1(const Matrix &other) {
    if (_row != other._row || _col != other._col) {
        printf_s("Error : Matrix::minus1() row, col is not equal\n");
        throw std::exception();
    }
    Matrix tmp(_row, _col);
    int yu = (_row*_col)%4;

#pragma omp parallel for
    for (int i=0;i<_row*_col-yu;i+=4) {
        _mm_store_ps(tmp._arr+i, _mm_sub_ps(_mm_load_ps(_arr+i), _mm_load_ps(other._arr+i)));
    }
//
    for (int i=0;i<yu;i++) {
        tmp._arr[i+_row*_col-yu] = _arr[i+_row*_col-yu] - other._arr[i+_row*_col-yu];
    }
    return tmp;
}
void Matrix::mul(Matrix &other) {
    if (_col != other._row) {
        printf_s("Error : Matrix::add() row, col is not equal\n");
        return;
    }
    alignas(16) float* tmp = new float [_row * other._col];
    memset(tmp, 0, _row*other._col*sizeof(float));
    for (int i=0;i<_row;i++) {
        for (int j=0;j<_col;j++) {
            for (int k=0;k<other._col;k++) {
                tmp[i*other._col+k] += _arr[i*_col+j] * other._arr[j*other._col+k];
            }
        }
    }
    std::swap(tmp, _arr);
    delete[] tmp;
}
Matrix Matrix::mul1(Matrix &other) {
    if (_col != other._row) {
        printf_s("Error : Matrix::mul1() row, col is not equal\n");
        throw std::exception();
    }
    Matrix tmp(_row, other._col);
    for (int i=0;i<_row;i++) {
        for (int j=0;j<_col;j++) {
            for (int k=0;k<other._col;k++) {
                tmp._arr[i*other._col+k] += _arr[i*_col+j] * other._arr[j*other._col+k];
            }
        }
    }
    return tmp;
}
// only 1 col
Matrix Matrix::mul11(Matrix &other) {
    Matrix tmp(_row, _col);
    for (int i=0;i<_row;i++) {
        tmp._arr[i] = _arr[i] * other._arr[i];
    }
    return tmp;
}
// new version mul
void Matrix::mul2(Matrix &other) {
    if (_col != other._row) {
        printf_s("Error : Matrix::mul2() row, col is not equal\n");
        return;
    }
    // 先转置
    alignas(16) float* tmp = new float [_row*other._col];
    memset(tmp, 0, _row*other._col*sizeof(float));
    other.self_transpose();
    for (int i=0;i<_row;i++) {
        for (int j=0;j<other._row;j++) {
            for (int k=0;k<_col;k+=4) {
                __m128 a = _mm_load_ps(_arr+i*_col+k);
                __m128 b = _mm_load_ps(other._arr+j*other._col+k);
                __m128 c = _mm_mul_ps(a, b);
                tmp[i*other._col+j] += c[0];
                tmp[i*other._col+j] += c[1];
                tmp[i*other._col+j] += c[2];
                tmp[i*other._col+j] += c[3];
            }
        }
    }
    std::swap(tmp, _arr);
    delete[] tmp;
}
//
void Matrix::M_E_trans(EtransStruct &etransStruct, int type) {
    if (type == _ETRANS_ROW__) {
        if (etransStruct.scale) {
            for (int i=0;i<_col;i++) {
                _arr[(etransStruct.minuendLine-1)*_col + i] -=
                        (etransStruct.scale)*(_arr[(etransStruct.subtractorLine-1)*_col + i]);
            }
        } else {
            if (etransStruct.subtractorLine<0 && etransStruct.minuendLine<0) {
                M_E_swap(-etransStruct.minuendLine, -etransStruct.subtractorLine, type);
            }
        }
    } else {
        if (etransStruct.scale) {
            for (int i=0;i<_col;i++) {
                _arr[etransStruct.minuendLine-1+ _col*i] -=
                        (etransStruct.scale)*(_arr[etransStruct.subtractorLine-1+ _col*i]);
            }
        } else {
            if (etransStruct.subtractorLine<0 && etransStruct.minuendLine<0) {
                M_E_swap(-etransStruct.minuendLine, -etransStruct.subtractorLine, type);
            }
        }
    }
}

void Matrix::M_E_swap(int line1, int line2, int type) {
    line1--;
    line2--;
    if (type == _ETRANS_ROW__) {
        if (line1<0  || line1>=_row  || line2<0 || line2>=_row) {
            printf_s("Error : Matrix::M_E_swap() line1, line2 is not correct\n");
        } else {
            // todo sse
            for (int i=0;i<_col;i++) {
                std::swap(_arr[line1*_col+i], _arr[line2*_col+i]);
            }
        }
    } else {
        if (line1<0  || line1>=_col  || line2<0 || line2>=_col) {
            printf_s("Error : Matrix::M_E_swap() line1, line2 is not correct\n");
        } else {
            // todo sse
            for (int i=0;i<_row;i++) {
                std::swap(_arr[line1+_col*i], _arr[line2+_col*i]);
            }
        }
    }
}
void Matrix::M_dot(float num) {
    for (int i=0;i<_row;i++) {
        for (int j=0;j<_col;j++) {
            _arr[i*_col+j] *= num;
        }
    }
    // todo simd
}