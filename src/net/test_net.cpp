#include "../../include/net/Net.h"
#include <vector>
#include <string>

using namespace std;

void train_and_test_file(const string& file_path) {
    cout << "\nProcessing file: " << file_path << endl;
    
    //Set neuron number of every layer
    vector<int> layer_neuron_num = { 784,100,10 };

    // Initialise Net and weights
    Net net;
    net.initNet(layer_neuron_num);
    net.initWeights(1, 0., 0.1);

    //Get test samples and test samples
    Matrix input, label, test_input, test_label;
    int sample_number = 800;
    get_input_label(file_path.c_str(), input, label, sample_number);
    get_input_label(file_path.c_str(), test_input, test_label, 200, 800);

    cout <<" input : \n" <<input._row << ", " << input._col << endl;
    //Set loss threshold,learning rate and activation function
    float loss_threshold = 0.02;
    net.learning_rate = 0.01; // 减小学习率以避免梯度爆炸
    net.output_interval = 2;
    net.activation_function = "tanh2"; // 改用tanh2激活函数

    //Train,and draw the loss curve(cause the last parameter is ture) and test the trained net
    net.train(input, label, loss_threshold, true, file_path);
    net.test(test_input, test_label);

    //Save the model
    //net.save("models/model_sigmoid_800_200.xml");
}

int main() {
    // 手动指定data目录下的XML文件
    vector<string> data_files = {
        "D:/MyC++/simple_mat/data/input_label_0-9_1000.xml",
        "D:/MyC++/simple_mat/data/input_label_1000.xml",
        "D:/MyC++/simple_mat/data/input_label_500.xml"
    };
    
    // 对每个文件进行训练和测试
    for (const auto& file : data_files) {
        train_and_test_file(file);
    }
    return 0;
}