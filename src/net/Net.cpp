#include "../../include/net/Net.h"


//Activation function
Matrix Net::activationFunction(Matrix &x, std::string func_type) {
    activation_function = func_type;
    if (func_type == "sigmoid") {
        Matrix fx = sigmoid(x);
        return fx;
    }
    if (func_type == "tanh2") {
        Matrix fx= tanh2(x);
        return fx;
    }
    // ....
}

//Initialize net
void Net::initNet(std::vector<int>& layer_neuron_num_) {
    layer_neuron_num = layer_neuron_num_;
    //Generate every layer.
    layer.resize(layer_neuron_num.size());
    for (int i = 0; i < layer.size(); i++) {
        layer[i] = Matrix(layer_neuron_num[i], 1);
    }
    std::cout << "Generate layers, successfully!" << std::endl;

    //Generate every weights matrix and bias
    weights.resize(layer.size() - 1);
    bias.resize(layer.size() - 1);
    for (int i = 0; i < (layer.size() - 1); ++i) {
        weights[i] = Matrix(layer[i + 1]._row, layer[i]._row);
        //bias[i].create(layer[i + 1].rows, 1, CV_32FC1);
        bias[i] = Matrix(layer[i + 1]._row, 1);
    }
    delta_err.resize(layer.size()-1);
    std::cout << "Generate weights matrices and bias, successfully!" << std::endl;
    std::cout << "Initialise Net, done!" << std::endl;
}

//initialise the weights Matrixrix.if type =0,Gaussian.else uniform.
void Net::initWeight(Matrix &dst, int type, double a, double b) {
    if (type == 0) {
        randn(dst, a, b);
    } else {
        randu(dst, a, b);
    }
}

//initialise the weights matrix.
void Net::initWeights(int type, double a, double b) {
    //Initialise weights matrices and bias
    for (int i = 0; i < weights.size(); ++i) {
        initWeight(weights[i], type, a, b);
    }
}

//Initialise the bias matrices.
void Net::initBias(Matrix& bias_) {
    for (int i = 0; i < bias.size(); i++) {
        bias[i] = bias_;
    }
}

//Forward
void Net::forward() {
    for (int i = 0; i < layer_neuron_num.size() - 1; ++i) {
        Matrix product = weights[i] * layer[i] + bias[i];
        layer[i + 1] = activationFunction(product, activation_function);
    }
    //calcLoss(layer[layer.size() - 1], target, output_error, loss);
    output_error.resize(target._row, target._col);
    for (int i=0;i<target._row;i++) {
        output_error._arr[i] = target._arr[i] - layer[layer.size()-1]._arr[i];
    }
    //Matrix err_sqrare = M_pow(output_error, 2.);
    float err_sqr_sum = M_sum(M_pow(output_error, 2.));
    loss = err_sqr_sum*1.0 / (float)(layer[layer.size()-1]._row * 1.0);
}

//Compute delta error
void Net::deltaError() {
    for (int i = delta_err.size() - 1; i >= 0; i--) {
        // todo ?
        Matrix dx = derivativeFunction(layer[i+1], activation_function);
        //Output layer delta error
        if (i == delta_err.size() - 1) {
            delta_err[i] = dx.mul11(output_error);
        } else {//Hidden layer delta error
            Matrix delta_err_1 = weights[i+1].transpose() * delta_err[i+1];
            delta_err[i] = dx.mul11(delta_err_1);
        }
    }
}

//Update weights
void Net::updateWeights() {
    //Matrix delta_weights;
    //Matrix delta_bias;
    for (int i = 0; i < weights.size(); ++i) {
        Matrix delta_weights = (delta_err[i] * layer[i].transpose()) * learning_rate;
        Matrix delta_bias = delta_err[i] * learning_rate;
        weights[i] += delta_weights;
        bias[i] += delta_bias;
    }
}

//Forward
void Net::backward()
{
    //move this function to the end of the forward().
    //calcLoss(layer[layer.size() - 1], target, output_error, loss);
    deltaError();
    updateWeights();
}

//Train,use accuracy_threshold
void Net::train(Matrix& input, Matrix& target_, float accuracy_threshold) {
    if (input.empty()) {
        std::cout << "Input is empty!" << std::endl;
        return;
    }

    std::cout << "Train,begin!" << std::endl;

    //Matrix sample;
    if (input._row == (layer[0]._row) && input._col == 1) {
        target = target_;
        Matrix sample = input;
        layer[0] = sample;
        forward();
        //backward();
        int num_of_train = 0;
        while (accuracy < accuracy_threshold) {
            backward();
            forward();
            num_of_train++;
            if (num_of_train % 500 == 0) {
                std::cout << "Train " << num_of_train << " times" << std::endl;
                std::cout << "Loss: " << loss << std::endl;
            }
        }
        std::cout << std::endl << "Train " << num_of_train << " times" << std::endl;
        std::cout << "Loss: " << loss << std::endl;
        std::cout << "Train sucessfully!" << std::endl;
    } else if (input._row == (layer[0]._row) && input._col > 1) {
        double batch_loss = 0.;
        int epoch = 0;
        while (accuracy < accuracy_threshold) {
            batch_loss = 0.;
            for (int i = 0; i < input._col; ++i) {
                target = col(target_, i);
                Matrix sample = col(input, i);

                layer[0] = sample;
                forward();
                batch_loss += loss;
                backward();
            }
            test(input, target_);
            epoch++;
            if (epoch % 10 == 0) {
                std::cout << "Number of epoch: " << epoch << std::endl;
                std::cout << "Loss sum: " << batch_loss << std::endl;
            }
            //if (epoch % 100 == 0)
            //{
            //	learning_rate*= 1.01;
            //}
        }
        std::cout << std::endl << "Number of epoch: " << epoch << std::endl;
        std::cout << "Loss sum: " << batch_loss << std::endl;
        std::cout << "Train sucessfully!" << std::endl;
    } else {
        std::cout << "Rows of input don't Matrixch the number of input!" << std::endl;
    }
}

//Train,use loss_threshold
void Net::train(Matrix& input, Matrix& target_, float loss_threshold, bool draw_loss_curve) {
    if (input.empty()) {
        std::cout << "Input is empty!" << std::endl;
        return;
    }

    std::cout << "Train,begin!" << std::endl;

    //Matrix sample;
    if (input._row == (layer[0]._row) && input._col == 1) {
        target = target_;
        Matrix sample = input;
        layer[0] = sample;
        forward();
        //backward();
        int num_of_train = 0;
        while (loss > loss_threshold) {
            backward();
            forward();
            num_of_train++;
            if (num_of_train % 500 == 0) {
                std::cout << "Train " << num_of_train << " times" << std::endl;
                std::cout << "Loss: " << loss << std::endl;
            }
        }
        std::cout << std::endl << "Train " << num_of_train << " times" << std::endl;
        std::cout << "Loss: " << loss << std::endl;
        std::cout << "Train sucessfully!" << std::endl;
    } else if (input._row == (layer[0]._row) && input._col > 1) {
        float batch_loss = loss_threshold + 0.01;
        int epoch = 0;
        while (batch_loss > loss_threshold) {
            batch_loss = 0.;
            for (int i = 0; i < input._col; ++i) {
                target = col(target_, i);
                Matrix sample = col(input, i);
                layer[0] = sample;

                forward();
                backward();

                batch_loss += loss;
            }

            loss_vec.push_back(batch_loss);
            // todo draw
            if (loss_vec.size() >= 2 && draw_loss_curve) {
                //draw_curve(board, loss_vec);
            }
            epoch++;
            if (epoch % output_interval == 0) {
                std::cout << "Number of epoch: " << epoch << std::endl;
                std::cout << "Loss sum: " << batch_loss << std::endl;
            }
            if (epoch % 100 == 0) {
                learning_rate *= fine_tune_factor;
            }
        }
        std::cout << std::endl << "Number of epoch: " << epoch << std::endl;
        std::cout << "Loss sum: " << batch_loss << std::endl;
        std::cout << "Train sucessfully!" << std::endl;
    } else {
        std::cout << "Rows of input don't Matrixch the number of input!" << std::endl;
    }
}

//Test
void Net::test(Matrix &input, Matrix &target_) {
    if (input.empty()) {
        std::cout << "Input is empty!" << std::endl;
        return;
    }
    std::cout << std::endl << "Predict,begain!" << std::endl;

    if (input._row == (layer[0]._row) && input._col == 1) {
        int predict_number = predict_one(input);

        Point<int> target_maxLoc;
        maxLoc(target_, nullptr, &target_maxLoc);
        int target_number = target_maxLoc.y;

        std::cout << "Predict: " << predict_number << std::endl;
        std::cout << "Target:  " << target_number << std::endl;
        std::cout << "Loss: " << loss << std::endl;
    } else if (input._row == (layer[0]._row) && input._col > 1) {
        float loss_sum = 0;
        int right_num = 0;
        Matrix sample;
        for (int i = 0; i < input._col; ++i) {
            sample = col(input, i);
            int predict_number = predict_one(sample);
            loss_sum += loss;

            target = col(target_, i);
            Point<int> target_maxLoc;
            maxLoc(target, nullptr, &target_maxLoc);
            int target_number = target_maxLoc.y;

            std::cout << "Test sample: " << i << "   " << "Predict: " << predict_number << std::endl;
            std::cout << "Test sample: " << i << "   " << "Target:  " << target_number << std::endl << std::endl;
            if (predict_number == target_number) {
                right_num++;
            }
        }
        accuracy = (float)right_num / input._col;
        std::cout << "Loss sum: " << loss_sum << std::endl;
        std::cout << "accuracy: " << accuracy << std::endl;
    } else {
        std::cout << "Rows of input don't Matrixch the number of input!" << std::endl;
        return;
    }
}

//Predict
int Net::predict_one(Matrix &input) {
    if (input.empty()) {
        std::cout << "Input is empty!" << std::endl;
        return -1;
    }

    if (input._row == (layer[0]._row) && input._col == 1) {
        layer[0] = input;
        forward();

        Matrix layer_out = layer[layer.size() - 1];
        Point<int> predict_maxLoc;

        maxLoc(layer_out, nullptr, &predict_maxLoc);
        return predict_maxLoc.y;
    } else {
        std::cout << "Please give one sample alone and ensure input.rows = layer[0].rows" << std::endl;
        return -1;
    }
}

//Predict,more  than one samples
std::vector<int> Net::predict(Matrix &input) {
    std::vector<int> predicted_labels;
    if (input._row == (layer[0]._row) && input._col > 1) {
        for (int i = 0; i < input._col; ++i) {
            Matrix sample = col(input, i);
            int predicted_label = predict_one(sample);
            predicted_labels.push_back(predicted_label);
        }
    }
    return predicted_labels;
}

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



//Save model;
void Net::save(std::string filename)
{
}

//Load model;
void Net::load(std::string filename)
{
}

//Get sample_number samples in XML file,from the start column. 
void get_input_label(std::string filename, Matrix& input, Matrix& label, int sample_num, int start) {
    cout << filename << endl;
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str())) {
        doc.PrintError();
        ::exit(1);
    }
    XMLElement* opencv_storage = doc.RootElement();
    //Matrix mat1, mat2;
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
                    input.resize(rows, cols);
                    string tmp(inputChild->GetText());
                    auto vec = split(tmp, " ");
                    int j=0;
                    for (int i=0;i<vec.size();i++) {
                        if (vec[i] == "" || vec[i] == "\n") continue;
                        input._arr[j++] = std::stof(vec[i]);
                    }
                } else {
                    label.resize(rows, cols);
                    string tmp(inputChild->GetText());
                    auto vec = split(tmp, " ");
                    int j=0;
                    for (int i=0;i<vec.size();i++) {
                        if (vec[i] == "" || vec[i] == "\n") continue;
                        label._arr[j++] = std::stof(vec[i]);
                    }
                }
            } else { }
            inputChild = inputChild->NextSiblingElement();
        }
        std::cout << std::endl;
        input_f = false;
        input_node = input_node->NextSiblingElement("target");
    }
}
