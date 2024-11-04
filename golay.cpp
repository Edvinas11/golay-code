#include "golay.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// constructor
Golay::Golay() {
    create_generator_matrix();
    // create_parity_check_matrix();
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

void Golay::create_parity_check_matrix() {
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            H[i][j] = I[i][j];
        }
    }

    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            H[i + 12][j] = B[i][j];
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

void Golay::print_parity_check_matrix() {
    for (int i = 0; i < 24; ++i) {
        for (int j = 0; j < 12; ++j) {
            std::cout << H[i][j] << " ";
        }
        std::cout << std::endl;
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

void Golay::get_syndrome() {
    // get syndrome s = received * H
    for (int i = 0; i < 12; ++i) {
        syndrome[i] = 0;
    }

    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 24; ++j) {
            syndrome[i] = bin_add(syndrome[i], bin_mult(f_received[j], H[j][i]));
            // syndrome[i] ^= f_received[j] * H[j][i];   
        }
    }

    cout << "Syndrome: ";
    for (int i = 0; i < 12; ++i) {
        cout << syndrome[i];
    }
    cout << endl;
}

int Golay::get_weight(int *v, int size) {
    int weight = 0;
    for (int i = 0; i < size; ++i) {
        weight += v[i];
    }
    return weight;
}

void Golay::form_received() {
    for (int i = 0; i < 24; ++i) {
        f_received[i] = received[i];
    }

    int weight = get_weight(received, 23);

    if (weight % 2 == 0) {
        f_received[23] = 1;
    }
    else {
        f_received[23] = 0;
    }
}

void Golay::decode() {
    int u[24];
    bool decodable = true;
    int sB[11] = {0};

    // 1. form w0 or w1, whichever has odd weight
    form_received();

    // 2. compute syndrome
    get_syndrome();

    // if w(s) <= 3 then set u = [s, 0]
    if (get_weight(syndrome, 12) <= 3) {
        for (int i = 0; i < 24; ++i) {
            if (i < 12) {
                u[i] = syndrome[i];
            } 
            else {
                u[i] = 0;
            }
        }
    }
    // if w(s + bi) <= 2 for some row bi of B then u = [s + bi, ei] 
    else {
        bool found = false;

        for (int i = 0; i < 12; ++i) {
            int temp_syndrome[12];

            for (int j = 0; j < 12; ++j) {
                temp_syndrome[j] = bin_add(syndrome[j], B[i][j]);
            }

            if (get_weight(temp_syndrome, 12) >= 2) {
                for (int k = 0; k < 12; ++k) {
                    u[k] = temp_syndrome[k];
                }
                for (int k = 12; k < 24; ++k) {
                    u[k] = (k - 12 == i) ? 1 : 0;
                }

                found = true;
                break;
            }
        }

        if (!found) {
            // compute second syndrome sB
            for (int i = 0; i < 11; ++i) {
                for (int j = 0; j < 12; ++j) {
                    sB[i] = bin_add(sB[i], bin_mult(syndrome[j], B[j][i]));
                }
            }

            // if sB weight <= 3, set u = [0, sB]
            if (get_weight(sB, 11) <= 3) {
                for (int i = 0; i < 13; ++i) {
                    u[i] = 0;
                }
                for (int i = 13; i < 24; ++i) {
                    u[i] = sB[i - 13];
                }
            }
            // if sB + bi has weight <= 2 for some row bi of B then u = [ei, sB + bi]
            else {
                bool corrected = false;
                for (int i = 0; i < 11; ++i) {
                    int temp_syndrome_sB[11];
                    for (int j = 0; j < 11; ++j) {
                        temp_syndrome_sB[j] = bin_add(sB[j], B[i][j]);
                    }

                    if (get_weight(temp_syndrome_sB, 11) <= 2) {
                        for (int k = 0; k < 13; ++k) {
                            u[k] = (k == i) ? 1 : 0; // Set u[k] to 1 for error pattern
                        }
                        for (int k = 13; k < 24; ++k) {
                            u[k] = temp_syndrome_sB[k - 13];
                        }
                        corrected = true;
                        break;
                    }
                }

                if (!corrected) {
                    decodable = false;
                }
            }
        }
    }

    cout << "Error vector: ";
    for (int i = 0; i < 12; ++i) {
        cout << u[i] << " ";
    }
    cout << endl;

    // Decode if possible
    if (decodable) {
        int decoded[23];
        for (int i = 0; i < 23; ++i) {
            decoded[i] = bin_add(f_received[i], u[i]);
        }

        // Output the decoded message
        cout << "Decoded message: ";
        for (int i = 0; i < 23; ++i) {
            cout << decoded[i] << " ";
        }
        cout << endl;
    } else {
        cout << "ERROR: Message undecodable..." << endl;
    }
}