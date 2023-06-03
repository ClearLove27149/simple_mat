//
// Created by 26685 on 2023/6/3.
//
#pragma once
#ifndef SIMPLE_MAT_NET_H
#define SIMPLE_MAT_NET_H

#include "Function.h"
#include "../common/tinyxml2.h"
#include <iostream>
#include <vector>
#include <string>

using namespace tinyxml2;
using namespace std;

class Net
{
public:
    //Integer vector specifying the number of neurons in each layer including the input and output layers.
    std::vector<int> layer_neuron_num;
    std::string activation_function = "sigmoid";
    int output_interval = 10;
    float learning_rate;
    float accuracy = 0.;
    std::vector<float> loss_vec;
    float fine_tune_factor = 1.01;

protected:
    std::vector<Matrix> layer;
    std::vector<Matrix> weights;
    std::vector<Matrix> bias;
    std::vector<Matrix> delta_err;

    Matrix output_error;
    Matrix target;
    Matrix board;
    float loss;

public:
    Net() {};
    ~Net() {};

    //Initialize net:genetate weights matrices、layer matrices and bias matrices
    // bias default all zero
    void initNet(std::vector<int>& layer_neuron_num_);

    //Initialise the weights matrices.
    void initWeights(int type = 0, double a = 0., double b = 0.1);

    //Initialise the bias matrices.
    void initBias(Matrix& bias);

    //Forward
    void forward();

    //Forward
    void backward();

    //Train,use accuracy_threshold
    void train(Matrix& input, Matrix& target, float accuracy_threshold);

    //Train,use loss_threshold
    void train(Matrix& input, Matrix& target_, float loss_threshold, bool draw_loss_curve = false);

    //Test
    void test(Matrix &input, Matrix &target_);

    //Predict,just one sample
    int predict_one(Matrix &input);

    //Predict,more  than one samples
    std::vector<int> predict(Matrix &input);

    //Save model;
    void save(std::string filename);

    //Load model;
    void load(std::string filename);

protected:
    //initialise the weight matrix.if type =0,Gaussian.else uniform.
    void initWeight(Matrix &dst, int type, double a, double b);

    //Activation function
    Matrix activationFunction(Matrix &x, std::string func_type);

    //Compute delta error
    void deltaError();

    //Update weights
    void updateWeights();
};

//Get sample_number samples in XML file,from the start column.
void get_input_label(std::string filename, Matrix& input, Matrix& label, int sample_num, int start = 0);

// Draw loss curve
void draw_curve(Matrix& board, std::vector<double> points);

// utils
vector<string> split(const string& str, const string& delim);

#endif //SIMPLE_MAT_NET_H
