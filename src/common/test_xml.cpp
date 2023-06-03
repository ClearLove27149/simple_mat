//
// Created by 26685 on 2023/6/3.
//
#include "../../include/common/tinyxml2.h"
#include "../../include/matrix/matrix.h"
#include <vector>
#include <string>

using namespace std;
using namespace tinyxml2;

vector<string> split(const string& str, const string& delim) {
    vector<string> res;
    if ("" == str) return res;
    string strs = str + delim;
    size_t pos;
    size_t size = strs.length();
    for (int i=0;i<size;i++) {
        pos = strs.find(delim, i);
        if (pos < size) {
            string s = strs.substr(i, pos - i);
            res.emplace_back(s);
            i = pos + delim.size() - 1;
        }
    }
    return res;
}
void read3(const string& filename) {
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str())) {
        doc.PrintError();
        ::exit(1);
    }
    XMLElement* opencv_storage = doc.RootElement();
    Matrix mat1, mat2;
    XMLElement* input_node = opencv_storage->FirstChildElement("input");
    bool input_f = true;
    while (input_node) {
        XMLElement* inputChild = input_node->FirstChildElement();
        int rows=0, cols=0;
        string type = "float";
        while (inputChild) {
            if (string(inputChild->Name()) == "rows") {
                rows = atoi(inputChild->GetText());
            } else if (string(inputChild->Name()) == "cols") {
                cols = atoi(inputChild->GetText());
            } else if (string(inputChild->Name()) == "dt") {
                type = inputChild->GetText();
            } else if (string(inputChild->Name()) == "data") {
                if (input_f) {
                    mat1.resize(rows, cols);
                    string tmp(inputChild->GetText());
                    auto vec = split(tmp, " ");
                    int j=0;
                    for (int i=0;i<vec.size();i++) {
                        if (vec[i] == "" || vec[i] == "\n") continue;
                        mat1._arr[j++] = std::stof(vec[i]);
                    }
                } else {
                    mat2.resize(rows, cols);
                    string tmp(inputChild->GetText());
                    auto vec = split(tmp, " ");
                    int j=0;
                    for (int i=0;i<vec.size();i++) {
                        if (vec[i] == "" || vec[i] == "\n") continue;
                        mat2._arr[j++] = std::stof(vec[i]);
                    }
                }
            } else { }
            inputChild = inputChild->NextSiblingElement();
        }
        std::cout << std::endl;
        input_f = false;
        input_node = input_node->NextSiblingElement("target");

    }
    cout << "mat1:\n" << mat1 << endl;
}

int write() {
    //
}

int main() {
    read3("D:\\C++_code\\simple_mat\\data\\input_label_1000.xml");
    return 0;
}