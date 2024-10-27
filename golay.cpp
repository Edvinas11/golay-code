#include "golay.h"
#include <iostream>

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