#include "../../include/matrix/utils.h"
#include "../../include/matrix/matrix.h"

using namespace std;

int main() {
    float f1[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    float f2[16] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    Matrix m1(f1,4,4), m2(f2,4,4);
    cout << m1.minus1(m2) << endl;
    cout << m1 <<endl;
    m1.mul(m2);
    cout << m1 << endl;

    m1.self_transpose();
    cout << m1 << endl;
    cout <<"上三角化" << endl;
    auto it = M_Uptri_(m1);
    cout << it.Uptri_matrix << endl;
    cout << it.trans_matrix << endl;
    cout << "下三角化"<<endl;
    auto it1 = M_Lowtri_(m1);
    cout << it1.Lowtri_matrix << endl;
    cout << it1.trans_matrix << endl;
    cout <<"对角化" << endl;
    auto it2 = M_Diatri_(m1);
    cout << it2.Diatri_matrix << endl;
    cout << it2.trans_leftmatrix << endl;
    cout << it2.trans_rightmatrix << endl;

    cout << "逆" << endl;
    float f3[16] = {0,2,3,0,2,1,2,0,4,1,3,0,1,1,1,1};
    Matrix m3(f3, 4,4);
    cout << m3 << endl;
    auto it3 = M_Inverse(m3);
    cout << it3 << endl;
    //
    randn(m3, 0 ,1);
    cout << m3 << endl;
    return 0;
}