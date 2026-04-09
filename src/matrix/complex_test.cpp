#include "../../include/matrix/utils.h"
#include "../../include/matrix/matrix.h"

using namespace std;

int main() {
    // 测试基本运算
    cout << "=== 测试基本运算 ===" << endl;
    float f1[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    float f2[16] = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    Matrix m1(f1, 4, 4), m2(f2, 4, 4);
    
    cout << "m1:" << endl << m1 << endl;
    cout << "m2:" << endl << m2 << endl;
    
    // 测试加法
    cout << "m1 + m2:" << endl << m1.add1(m2) << endl;
    
    // 测试减法
    cout << "m1 - m2:" << endl << m1.minus1(m2) << endl;
    
    // 测试矩阵乘法
    cout << "m1 * m2:" << endl;
    Matrix m3 = m1.mul1(m2);
    cout << m3 << endl;
    
    // 测试元素级乘法
    cout << "m1 .* m2:" << endl << m1.mul11(m2) << endl;
    
    // 测试转置
    cout << "m1的转置:" << endl;
    Matrix m4 = m1.transpose();
    cout << m4 << endl;
    
    // 测试矩阵点乘
    cout << "m1 * 2:" << endl;
    Matrix m5 = m1;
    m5.M_dot(2);
    cout << m5 << endl;
    
    // 测试特殊矩阵运算
    cout << "=== 测试特殊矩阵运算 ===" << endl;
    
    // 测试上三角化
    cout << "m1的上三角化:" << endl;
    auto uptri_result = M_Uptri_(m1);
    cout << "上三角矩阵:" << endl << uptri_result.Uptri_matrix << endl;
    cout << "变换矩阵:" << endl << uptri_result.trans_matrix << endl;
    
    // 测试下三角化
    cout << "m1的下三角化:" << endl;
    auto lowtri_result = M_Lowtri_(m1);
    cout << "下三角矩阵:" << endl << lowtri_result.Lowtri_matrix << endl;
    cout << "变换矩阵:" << endl << lowtri_result.trans_matrix << endl;
    
    // 测试对角化
    cout << "m1的对角化:" << endl;
    auto diatri_result = M_Diatri_(m1);
    cout << "对角矩阵:" << endl << diatri_result.Diatri_matrix << endl;
    cout << "左变换矩阵:" << endl << diatri_result.trans_leftmatrix << endl;
    cout << "右变换矩阵:" << endl << diatri_result.trans_rightmatrix << endl;
    
    // 测试矩阵求逆
    cout << "=== 测试矩阵求逆 ===" << endl;
    float f4[9] = {1, 2, 3, 4, 5, 6, 7, 8, 10}; // 可逆矩阵
    Matrix m6(f4, 3, 3);
    cout << "原始矩阵:" << endl << m6 << endl;
    
    auto inv_result = M_Inverse(m6);
    cout << "逆矩阵:" << endl << inv_result << endl;
    
    // 验证逆矩阵的正确性 (A * A^-1 = I)
    Matrix identity = m6.mul1(inv_result);
    cout << "A * A^-1:" << endl << identity << endl;
    
    // 测试随机矩阵生成
    cout << "=== 测试随机矩阵生成 ===" << endl;
    Matrix m7(3, 3);
    randn(m7, 0, 1);
    cout << "随机矩阵 (均值0，标准差1):" << endl << m7 << endl;
    
    // 测试边界情况
    cout << "=== 测试边界情况 ===" << endl;
    
    // 测试空矩阵
    Matrix m8;
    cout << "空矩阵的行数: " << m8._row << "，列数: " << m8._col << endl;
    
    // 测试单行矩阵
    float f5[4] = {1, 2, 3, 4};
    Matrix m9(f5, 1, 4);
    cout << "单行矩阵:" << endl << m9 << endl;
    
    // 测试单列矩阵
    float f6[4] = {1, 2, 3, 4};
    Matrix m10(f6, 4, 1);
    cout << "单列矩阵:" << endl << m10 << endl;
    
    // 测试矩阵乘法的维度不匹配情况
    cout << "=== 测试错误处理 ===" << endl;
    try {
        Matrix m11(2, 3); // 2x3矩阵
        Matrix m12(4, 5); // 4x5矩阵
        m11.mul1(m12); // 维度不匹配，应该抛出异常
    } catch (const exception& e) {
        cout << "矩阵乘法维度不匹配异常: " << e.what() << endl;
    }
    
    cout << "=== 测试完成 ===" << endl;
    return 0;
}