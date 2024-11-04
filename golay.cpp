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
    int u[24] = {0};  // Error vector
    bool decodable = true;
    int sB[12] = {0};

    // Step 1: Form received vector with parity bit
    form_received();

    // Step 2: Compute syndrome
    get_syndrome();

    // Step 2: If weight of syndrome <= 3, use it as error pattern [s, 0]
    if (get_weight(syndrome, 12) <= 3) {
        for (int i = 0; i < 12; ++i) {
            u[i] = syndrome[i];
        }
        for (int i = 12; i < 24; ++i) {
            u[i] = 0;
        }
    }
    else {
        bool found = false;

        // Step 3: Check if weight(s + b_i) <= 2 for some row b_i of B
        for (int i = 0; i < 12; ++i) {
            int temp_syndrome[12];
            for (int j = 0; j < 12; ++j) {
                temp_syndrome[j] = bin_add(syndrome[j], B[i][j]);
            }

            if (get_weight(temp_syndrome, 12) <= 2) {
                // Set u = [s + b_i, e_i]
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
            // Step 4: Compute second syndrome sB = syndrome * B
            for (int i = 0; i < 12; ++i) {
                sB[i] = 0;
                for (int j = 0; j < 12; ++j) {
                    sB[i] ^= (syndrome[j] * B[j][i]);
                }
            }

            // Step 5: If weight of sB <= 3, use [0, sB] as error pattern
            if (get_weight(sB, 12) <= 3) {
                for (int i = 0; i < 12; ++i) {
                    u[i] = 0;
                    u[i + 12] = sB[i];
                }
            }
            else {
                // Step 6: Check if weight(sB + b_i) <= 2 for some row b_i of B
                bool corrected = false;
                for (int i = 0; i < 12; ++i) {
                    int temp_sB[12];
                    for (int j = 0; j < 12; ++j) {
                        temp_sB[j] = bin_add(sB[j], B[i][j]);
                    }

                    if (get_weight(temp_sB, 12) <= 2) {
                        // Set u = [e_i, sB + b_i]
                        for (int k = 0; k < 12; ++k) {
                            u[k] = (k == i) ? 1 : 0; // Set u[k] to 1 for error pattern
                            u[k + 12] = temp_sB[k];
                        }
                        corrected = true;
                        break;
                    }
                }

                if (!corrected) {
                    decodable = false;  // Unable to decode
                }
            }
        }
    }

    cout << "Error vector: ";
    for (int i = 0; i < 24; ++i) {
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
