#include "golay.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// constructor
Golay::Golay() {
    create_generator_matrix();
}

// creates a 12 x 23 matrix G = [I, B]
void Golay::create_generator_matrix() {
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            G[i][j] = I[i][j];
        }
    }
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 11; ++j) {
            G[i][j + 12] = B[i][j];
        }
    }
}

// prints G matrix
void Golay::print_generator_matrix() {
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 23; ++j) {
            cout << G[i][j] << " ";
        }
        cout << endl;
    }
}

// binary addition (mod 2)
int Golay::bin_add(int a, int b) {
    return (a + b) % 2;
}

// binary multiplication (mod 2)
int Golay::bin_mult(int a, int b) {
    return (a * b) % 2;
}

void Golay::encode(int *message) {
    for (int i = 0; i < 23; ++i) {
        encoded[i] = 0;
    }

    // encode the message by performing matrix multiplication
    for (int i = 0; i < 23; ++i) {
        for (int j = 0; j < 12; ++j) {
            encoded[i] = bin_add(encoded[i], bin_mult(message[j], G[j][i]));
        }
    }
}

void Golay::print_encoded_message() {
    for(int i = 0; i < 23; ++i) {
        cout << encoded[i] << " ";
    }
    cout << endl;
}

double Golay::input_probability() {
    double p;
    cout << "Please enter error probability as a floating point number (e.g., 0.0001): ";

    if (!(cin >> p)) {
        cerr << "ERROR: Invalid input. Please enter a valid floating-point number." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return input_probability();
    }

    if (p < 0.0 || p > 1.0) {
        cerr << "ERROR: Probability must be between 0 and 1." << endl;
        return input_probability();
    }

    return p;
}

void Golay::print_received() {
    for(int i = 0; i < 23; ++i) {
        cout << received[i] << " ";
    }
    cout << endl;
}

void Golay::send_through_channel(double p) {
    srand(time(0));

    for (int i = 0; i < 23; ++i) {
        double a = (double)rand() / RAND_MAX;

        if (a < p) {
            received[i] = 1 - encoded[i];
        }
        else {
            received[i] = encoded[i];
        }
    }
}