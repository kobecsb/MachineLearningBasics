/*
Program: Logistic Regression CPP
Description: 
Shanbo Cheng: cshanbo@gmail.com
Date: 2016-07-19 16:11:48
Last modified: 2016-08-29 14:10:07
GCC version: 4.9.3
*/

#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <cassert>
#include <algorithm>
#include "../include/LR.h"
#include "../include/utils.h"
using namespace std;

LogisticRegression::LogisticRegression() {}

LogisticRegression::LogisticRegression(matrix<double> input, int in_d, int out_d) {
    //init weights as all 0

    n_in = in_d;
    n_out = out_d;
    this->input = input;
    weights = vector<vector<double>>(n_in, vector<double>(n_out, 0));
    bias = vector<double>(n_out, 0);
    dot(input, weights, y_given_x, bias);
    softmax(y_given_x);
    y_pred = vector<int>(input.size(), 0);
    for(unsigned int i = 0; i < y_pred.size(); ++i)
        y_pred[i] = maxIndex(y_given_x[i]);
}

LogisticRegression::~LogisticRegression() {}

void LogisticRegression::sigmoid(matrix<double>& mat) {
    if(mat.empty())
        return;
    for(auto& vec: mat)
        for(auto& d: vec)
            d = 1.0 / (1 + exp(-1 * d));
}

void LogisticRegression::softmax(matrix<double>& mat) {
    if(mat.empty())
        return;

    for(auto& vec: mat) {
        double sum = 0;
        for(auto& d: vec)
            sum += exp(d);
        for(auto& d: vec)
            d = exp(d) / sum;
    }
}

double LogisticRegression::negativeLogLikelihood(vector<int> y) {
    if(y.empty())
        return 0;
    double sum = 0;
    for(unsigned int i = 0; i < y.size(); ++i)
        sum += log(y_given_x[i][y[i]]);
    return -1 * sum / y.size();
}

double LogisticRegression::calcError(vector<int> y) {
    assert(y.size() == y_pred.size());
    double ret = 0;
    for(unsigned int i = 0; i < y.size(); ++i)
        ret = y[i] == y_pred[i]? ret: ret + 1.0;
    return ret / y.size();
}

void LogisticRegression::update(double rate, vector<int> y) {
    assert(!y.empty());
    double dy = 0;
    for(unsigned int k = 0; k < input.size(); ++k) {
        for(int i = 0; i < n_out; ++i) {
            //the gradient of soft max is this line
            //dy = y[k] == i? 1 - y_given_x[k][i]: -1 * y_given_x[k][i];
            //while the gradient of negativeloglikelihood is simply y_i - o_i (before softmax)
            //o_i is weight dot input
            //so the gradient of weight is easy to calculate
            dy = y[k] == i? y_given_x[k][i] - 1: y_given_x[k][i];
            //mini batch, average
            
            for(int j = 0; j < n_in; ++j) {
                weights[j][i] -= rate * dy * input[k][j] / input.size();
            }

            //bias doesn't need to product with input
            bias[i] -= rate * dy / input.size();
        }
    }
    dot(input, weights, y_given_x, bias);
    softmax(y_given_x);
    for(unsigned int i = 0; i < y_pred.size(); ++i)
        y_pred[i] = maxIndex(y_given_x[i]);
}

void LogisticRegression::train(matrix<double> miniBatch, vector<int> y, int epoch, double rate) {
    if(miniBatch.empty())
        return;
    for(int i = 0; i < epoch; ++i) {
        update(rate, y);
        cout << negativeLogLikelihood(y) << endl;
        cout << endl;
    }
}

vector<int> LogisticRegression::test(matrix<double> testSet, vector<int> y) {
    vector<int> label;
    vector<vector<double>> ygx;
    dot(testSet, weights, ygx, bias);
    softmax(ygx);
    for(auto v: ygx)
        label.push_back(maxIndex(v));
    double precision = 0;
    for(unsigned int i = 0; i < y.size(); ++i)
        precision = y[i] != label[i] ? precision: precision + 1;
    precision /= y.size();
    cout << "Precision:" << precision << endl;
    return label;
}


/*int main() {
    vector<vector<double>> input{
        {1, 1, 1, 0, 0, 0},
        {1, 0, 1, 0, 0, 0},
        {1, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 1, 0},
        {0, 0, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0}
    };

    vector<int> ytrain{0, 0, 0, 1, 1, 1};

    vector<vector<double>> test{
        {1, 0, 1, 0, 0, 0},
        {0, 0, 1, 1, 1, 0}, 
        {0, 0, 0, 1, 1, 1}, 
        {0, 1, 0, 0, 0, 1}, 
    };

    vector<int> ytest{0, 1, 1, 0};
    LogisticRegression lr(input, 6, 2);

    lr.train(input, ytrain, 5000);
    auto v = lr.test(test, ytest);

    return 0;
}*/


//int main() {
//
//}










//

