#include "../../include/net/Net.h"
#include <vector>

using namespace std;

void tanh2_train() {
    //Set neuron number of every layer
    vector<int> layer_neuron_num = { 784,100,10 };

    // Initialise Net and weights
    Net net;
    net.initNet(layer_neuron_num);
    net.initWeights(1, 0., 0.1);

    //Get test samples and test samples
    Matrix input, label, test_input, test_label;
    int sample_number = 800;
    get_input_label("D:\\MyC++\\simple_mat\\data\\input_label_1000.xml", input, label, sample_number);
    get_input_label("D:\\MyC++\\simple_mat\\data\\input_label_1000.xml", test_input, test_label, 200, 800);

    cout <<" input : \n" <<input._row << ", " << input._col << endl;
    //Set loss threshold,learning rate and activation function
    float loss_threshold = 0.2;
    net.learning_rate = 0.05;
    net.output_interval = 2;
    net.activation_function = "tanh2";

    //Train,and draw the loss curve(cause the last parameter is ture) and test the trained net
    net.train(input, label, loss_threshold, false);
    //net.test(test_input, test_label);

    //Save the model
    //net.save("models/model_sigmoid_800_200.xml");
}

int main() {
    tanh2_train();
    return 0;
}